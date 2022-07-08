//
// Created by xm on 2022/7/8.
//

#include "Mutex.h"
#include "../log/Log.h"

namespace xm {
    Semaphore::Semaphore(u_int32_t count) {
       if ( sem_init(&m_semaphore, 0, count)) {
           ROOT_LOG(LogLevel::INFO) << "sen_init error" << endl;
       }
    }

    Semaphore::~Semaphore() {
        if (sem_destroy(&m_semaphore)) {
            ROOT_LOG(LogLevel::INFO) << "sem_destory error" << endl;
        }
    }

    void Semaphore::wait() {
        if (sem_wait(&m_semaphore)) {
            ROOT_LOG(LogLevel::INFO) << "sem_wait error" << endl;
        }
    }

    void Semaphore::notify() {
        if (sem_post(&m_semaphore)) {
            ROOT_LOG(LogLevel::INFO) << "sem_post error" << endl;
        }
    }

    void Semaphore::trywait() {
        if (sem_trywait(&m_semaphore)) {
            ROOT_LOG(LogLevel::INFO) << "try wait error" << endl;
        }
    }

}