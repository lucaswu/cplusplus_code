#include "thread_pool.h"
#include <iostream>
void fun(int i)
{
    std::cout<<"hello from:"<<i<<std::endl;
}

int main(int argc,char **argv)
{
  ThreadPool pool(2);
  pool.start();
  for(int i=0;i<20;i++){
      pool.append(std::bind(fun,i));
  }
    
 
  return 0;
}