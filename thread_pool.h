//
//  thread_pool.h
//  ThreadPool
//
//  Created by Aris on 2020/2/22.
//  Copyright © 2020 Aris. All rights reserved.
//

#ifndef thread_pool_h
#define thread_pool_h

#include <iostream>
#include <thread>
#include <atomic>
#include <memory>
#include <list>
#include <functional>

#include "thread_queue_class.h"

using namespace std;

class ThreadPool
{
    using Task = std::function<void()>;
    
    //函数定义
public:
    static void CreateThreadPool(int thread_size = std::thread::hardware_concurrency() * 2);
    
    virtual~ThreadPool();
    
    static ThreadPool* getInstance();
    
    void addTask(Task && task);
    
    void Stop();
    
private:
    ThreadPool(int thread_size);
    void RunThread();
    void StopThreadGroup();
    
private:
    static ThreadPool* instance_;                                    //线程池单例
    ThreadQueue<Task> thread_queue_;                                 //同步队列
    int thread_size_;                                                //线程队列
    std::once_flag flag_;                                            //计次标示
    std::atomic<bool> runState_{false};                              //运行标示
    std::list<std::shared_ptr<std::thread>> thread_group_;           //线程池
    
};



#endif /* thread_pool_h */
