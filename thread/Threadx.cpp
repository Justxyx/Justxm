//
// Created by xm on 2022/6/25.
//

#include "Threadx.h"
#include "../log/Log.h"

namespace xm {

    static thread_local Threadx *t_thread = nullptr;
    static thread_local string t_thread_name = "UNKNOW";

    Threadx *Threadx::getThis() {
        return t_thread;
    }

    const string &Threadx::getName() {
        return t_thread_name;
    }

    const pid_t Threadx::getPid() {
        return t_thread->m_id;
    }
    void Threadx::setName(const string &name) {
        if (name.empty())
            return;
        if (t_thread)
            t_thread->m_name = name;
        t_thread_name = name;
    }

    Threadx::Threadx(function<void()> cb, const string &name)
    :m_cb(cb),m_name(name)
    {
        if (name.empty())
            m_name = "UNKNOW";
        int rt = pthread_create(&m_thread, nullptr,
                                &Threadx::run, this);
        if (rt) {
            ROOT_LOG(LogLevel::ERROR) << "pthread_create thread error" <<
                "name = " << name << endl;
        }
        m_sem.wait();
    }

    Threadx::~Threadx() {
        if (m_thread) {
            pthread_detach(m_thread);
        }
    }

    // 调用这个其实还是主线程调用的
    void Threadx::join() {
        if (m_thread) {
            int rt = pthread_join(m_thread, nullptr);
            if (rt) {
                ROOT_LOG(LogLevel::ERROR) << "pthread_join thread fail"
                << "name = "<< m_name << endl;
            }
            m_thread = 0;
        }
    }

    // 只有run 才是真正的跑到线程里面去了  好好理解一下这句话
    void *Threadx::run(void *arg) {
        Threadx *thread = (Threadx*)arg;
        t_thread = thread;
        t_thread_name = thread->m_name;
        t_thread->m_id = xm::GetThreadId();
        function<void()> cb;
        cb.swap(thread->m_cb);

        cb();
        thread->m_sem.notify();
        return 0;
    }


    pid_t Threadx::getMId() const {
        return m_id;
    }

    const string &Threadx::getMName() const {
        return m_name;
    }
}