//
//  thread_pool.cpp
//  ThreadPool
//
//  Created by Aris on 2020/2/22.
//  Copyright © 2020 Aris. All rights reserved.
//

#include <stdio.h>
#include "thread_pool.h"

ThreadPool* ThreadPool::instance_ = nullptr;

ThreadPool::ThreadPool(int thread_size)
:thread_size_(thread_size)
{
    runState_ = true;
    
    thread_queue_.Run();
    
    for(auto thread_index = 0;thread_index < thread_size_;thread_index++)
    {
        thread_group_.push_back(make_shared<std::thread>(std::bind(&ThreadPool::RunThread, this)));
    }
}


void ThreadPool::CreateThreadPool(int thread_size)
{
    if(instance_ != nullptr)
        throw std::logic_error("ThreadPool Already Exist");
    
    instance_ = new ThreadPool(thread_size);
}

ThreadPool* ThreadPool::getInstance()
{
    if(instance_ == nullptr)
        throw std::logic_error("Error : None ThreadPool");
    
    return instance_;
}

ThreadPool::~ThreadPool()
{
    Stop();
}

void ThreadPool::addTask(Task &&task)
{
    thread_queue_.add_back_element(std::forward<Task>(task));
}

void ThreadPool::RunThread()
{
    while (runState_ == true) {
        
        //取除任务
        std::queue<Task> elements;
        if(!thread_queue_.get_queue_elements(elements))
            continue;
        
        
        while (elements.size() != 0)
        {
            Task task = std::move(elements.front());
            elements.pop();
            
            task();
        }
    }
}

void ThreadPool::Stop()
{
    if(instance_ == nullptr)
        throw logic_error("Error : None ThreadPool");
    
    std::call_once(flag_, [this]{StopThreadGroup();});
}


void ThreadPool::StopThreadGroup()
{
    thread_queue_.Stop();
    
    runState_ = false;
    
    for(auto & thread_in : thread_group_)
    {
        if(thread_in)
            thread_in->join();
    }
    
    thread_group_.clear();
}
