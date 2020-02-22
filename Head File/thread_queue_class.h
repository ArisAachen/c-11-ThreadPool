//
//  thread_queue_class.h
//  ThreadPool
//
//  Created by Aris on 2020/2/22.
//  Copyright © 2020 Aris. All rights reserved.
//

#ifndef thread_queue_class_h
#define thread_queue_class_h

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

#define MAX_SIZE 100


template <typename T>
class ThreadQueue
{
    //函数定义
public:
    ThreadQueue(int max_size = MAX_SIZE);
    
    virtual~ThreadQueue();
    
    //添加方法
public:
    //取得第一个元素
    bool get_front_element(T & element);
    //取得队列元素
    bool get_queue_elements(std::queue<T>& elements);
    
    //添加最后一个元素
    bool add_back_element(T && element);
    //添加最后一列元素
    bool add_queue_elements(std::queue<T>&& elements);
    
    //任务运行
    void Run();
    
    //任务终止
    void Stop();
    
    //任务队列个数
    size_t get_size();
    
    //成员变量
private:
    //任务队列是否为空
    bool isEmpty();
    //任务队列是否已满
    bool isFull();
    
    //成员方法
private:
    int max_size_;                      //最大个数
    std::queue<T> thread_queue_;        //线程队列
    bool runState_;                     //运行标识
    
    std::mutex mutex_;                  //互斥锁
    std::condition_variable notEmpty_;  //条件变量为空
    std::condition_variable notFull_;   //条件变量非空
};



#endif /* thread_queue_class_h */
