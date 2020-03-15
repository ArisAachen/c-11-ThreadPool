//
//  thread_queue_class.cpp
//  ThreadPool
//
//  Created by Aris on 2020/2/22.
//  Copyright © 2020 Aris. All rights reserved.
//

#include <stdio.h>
#include "thread_queue_class.h"

//函数定义
template<typename T>
ThreadQueue<T>::ThreadQueue(int max_size):
max_size_(max_size),runState_(false)
{
    
}

//虚构函数
template<typename T>
ThreadQueue<T>::~ThreadQueue()
{
    //析构成员
    for_each(thread_queue_.front(), thread_queue_.back(), [](T& element){~element;});
    
    //清空队列
    thread_queue_ = std::queue<T>();
}

//是否为空
template<typename T>
bool ThreadQueue<T>::isEmpty()
{
    std::unique_lock<std::mutex> locker(mutex_);
    return thread_queue_.size() == 0;
}

//是否已满
template<typename T>
bool ThreadQueue<T>::isFull()
{
    std::unique_lock<std::mutex> locker(mutex_);
    return thread_queue_.size() >= max_size_;
}

template<typename T>
size_t ThreadQueue<T>::get_size()
{
    std::unique_lock<std::mutex> locker(mutex_);
    return thread_queue_.size();
}

template<typename T>
void ThreadQueue<T>::Run()
{
    std::unique_lock<std::mutex> locker(mutex_);
    runState_ = true;

    notEmpty_.notify_all();
    notFull_.notify_all();
}

template<typename T>
void ThreadQueue<T>::Stop()
{
    std::unique_lock<std::mutex> locker(mutex_);
    runState_ = false;
    
    notEmpty_.notify_all();
    notFull_.notify_all();
}


template<typename T>
bool ThreadQueue<T>::get_front_element(T &element)
{
    std::unique_lock<std::mutex> locker(mutex);
    
    notEmpty_.wait(locker,[this]{ !isEmpty() || runState_;});
    
    element = std::move(thread_queue_.front());
    
    thread_queue_.pop();
    
    notFull_.notify_one();
    
    return true;
}

template<typename T>
bool ThreadQueue<T>::get_queue_elements(std::queue<T> &elements)
{
    std::unique_lock<std::mutex> locker(mutex_);
    
    notEmpty_.wait(locker,[this]{ !isEmpty() || runState_;});
    
    elements = std::move(thread_queue_);
    
    notFull_.notify_one();
    
    return true;
}


template<typename T>
bool ThreadQueue<T>::add_back_element(T &&element)
{
    std::unique_lock<std::mutex> locker(mutex_);
    
    notFull_.wait(locker,[this]{ !isFull() || runState_;});
    
    thread_queue_.push(std::forward<T>(element));
    
    notEmpty_.notify_one();
    
    return true;
}

template<typename T>
bool ThreadQueue<T>::add_queue_elements(std::queue<T> &&elements)
{
    std::unique_lock<std::mutex> locker(mutex_);
    
    notFull_.wait(locker,[this]{ !isFull() || runState_;});

    thread_queue_.push(std::forward<std::queue<T>>(elements));
    
    notEmpty_.notify_one();
    
    return true;
}
