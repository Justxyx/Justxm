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
        int rt = pthread_create(&m_thread, nullptr, &Threadx::run, this);
        if (rt) {
            ROOT_LOG(LogLevel::ERROR) << "pthread_create thread error" <<
                "name = " << name << endl;
        }
    }

    Threadx::~Threadx() {
        if (m_thread) {
            pthread_detach(m_thread);
        }
    }

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

    void *Threadx::run(void *arg) {
        Threadx *thread = (Threadx*)arg;
        t_thread = thread;
        t_thread_name = thread->m_name;
        thread->m_id = 1111;

        function<void()> cb;
        cb.swap(thread->m_cb);

        cb();
        return 0;
    }


    pid_t Threadx::getMId() const {
        return m_id;
    }

    const string &Threadx::getMName() const {
        return m_name;
    }
}