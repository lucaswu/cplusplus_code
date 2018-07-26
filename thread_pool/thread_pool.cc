#include "thread_pool.h"
ThreadPool::ThreadPool(int num) 
: threadNum_(num)
, isrunning_(false)
, m_()
{
}

ThreadPool::~ThreadPool()
{
    if (isrunning_)
    {
        stop();
    }
}

void ThreadPool::start()
{
    isrunning_ = true;
    threads_.reserve(threadNum_);
    for (int i = 0; i < threadNum_; ++i)
    {
        threads_.push_back(std::make_shared<std::thread>(&ThreadPool::work, this));
    }
}

void ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex> locker(m_);
        
        cond_.notify_all();
        
        isrunning_ = false;
    }
    for (int i = 0; i < threads_.size(); ++i)
    {
        auto t = threads_[i];
        if (t->joinable())
            t->join();
    }
    
}

void ThreadPool::append(const Task &task)
{
    if(isrunning_){
        std::unique_lock<std::mutex> locker(m_);
        tasks_.push_back(task);
        cond_.notify_one();
    }
}
void ThreadPool::append(Task &&task)
{
    if(isrunning_){
        std::unique_lock<std::mutex> locker(m_);
        tasks_.push_back(std::move(task));
        cond_.notify_one();
    }

}

void ThreadPool::work()
{
    printf("isrunning=%d\n",isrunning_);
    while (isrunning_)
    {
        Task task;
        {
            std::unique_lock<std::mutex> locker(m_);
            if (isrunning_ && tasks_.empty())
            {
                cond_.wait(locker);
            }
            if (!tasks_.empty())
            {
                task = tasks_.front();
                tasks_.pop_front();
            }
        }
        if(task)
            task();
    }
}