//
// Created by xm on 2022/6/25.
//

#ifndef JUSTXM_THREADX_H
#define JUSTXM_THREADX_H

#include <thread>
#include <functional>
#include <memory>
#include <pthread.h>
#include "Mutex.h"
using namespace std;

namespace xm {

class Threadx {
public:
    typedef shared_ptr<Threadx> ptr;
    Threadx(function<void()> cb, const string &name);
    ~Threadx();

    pid_t getMId() const;
    const string &getMName() const;
    void join();

    // 获取当前线程指针
    static Threadx* getThis();
    // 获取当前线程名字
    static const string& getName();
    // 设置当前线程名字
    static void setName(const string &name);
    // 获得当前线程编号
    static const pid_t getPid();

    // 线程执行函数
    static void *run(void *arg);

private:
    // 线程id
    pid_t m_id = -1;
    // 线程数据结构
    pthread_t m_thread;
    // 回调函数
    function<void()> m_cb;
    // 线程名称
    string m_name;

    Semaphore m_sem;
};

}

#endif //JUSTXM_THREADX_H
