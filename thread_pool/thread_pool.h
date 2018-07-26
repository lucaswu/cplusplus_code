#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include<thread>
#include<mutex>
#include<condition_variable>
#include<memory>
#include<vector>
#include<functional>
#include<list>
#include<cstdio>

class ThreadPool{
public:
    using Task = std::function<void(void)>;
    explicit ThreadPool(int num);
    ~ThreadPool();
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool& rhs) = delete;
    void append(const Task &task);
    void append(Task &&task);
    void start();
    void stop();

private:
    void work();
    bool isrunning_;
    int threadNum_;
   
    std::mutex m_;
    std::condition_variable cond_;
    std::list<Task> tasks_;
    std::vector<std::shared_ptr<std::thread>> threads_;
};
#endif