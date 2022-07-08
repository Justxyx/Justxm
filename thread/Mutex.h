//
// Created by xm on 2022/7/8.
//

#ifndef JUSTXM_MUTEX_H
#define JUSTXM_MUTEX_H
#include "../util/Noncopyable.h"
#include <semaphore.h>
#include <pthread.h>
namespace xm {

    // 条件变量
    class Semaphore : public Noncopyable {
    public:
        // 构造
        Semaphore(u_int32_t count = 0);
        // 析构
        ~Semaphore();

        void trywait();

        void wait();

        void notify();
    private:
      sem_t m_semaphore;
    };


    template<class T>
    struct ScopedLockImpl {
    public:
        ScopedLockImpl(T &mMutex) : m_mutex(mMutex) {
            m_mutex.lock();
            m_locked = true;
        }

        ~ScopedLockImpl() {
            unlock();
        }

        void lock() {
            if (!m_locked) {
                m_mutex.lock();
                m_locked = true;
            }
        }

        void unlock() {
            if (m_locked) {
                m_mutex.unlock();
                m_locked  = false;
            }
        }

    private:
        T &m_mutex;
        bool m_locked;
    };


    template<class T>
    struct ReadScopedLockImpl {
    public:
        ReadScopedLockImpl(T &mutex) : m_mutex(mutex) {
            m_mutex.rdlock();
            m_locked = true;
        }
        ~ReadScopedLockImpl() {
            unlock();
        }
        void lock() {
            if(!m_locked) {
                m_mutex.rdlock();
                m_locked = true;
            }
        }
        void unlock() {
            if (m_locked) {
                m_mutex.unlock();
                m_locked = false;
            }
        }
    private:
        T &m_mutex;
        bool m_locked = false;
    };

    template<class T>
    struct WriteScopedLockImpl {
    public:
        WriteScopedLockImpl(T &mutex) : m_mutex(mutex) {
            m_mutex.wrlock();
            m_locked = true;
        }
        ~WriteScopedLockImpl() {
            unlock();
        }
        void lock() {
            if(!m_locked) {
                m_mutex.wrlock();
                m_locked = true;
            }
        }
        void unlock() {
            if (m_locked) {
                m_mutex.unlock();
                m_locked = false;
            }
        }
    private:
        T &m_mutex;
        bool m_locked = false;
    };

    // 互斥锁
    class Mutex : public Noncopyable {
    public:
        Mutex() {
            pthread_mutex_init(&m_mutex, nullptr);
        }
        ~Mutex() {
            pthread_mutex_destroy(&m_mutex);
        }
        void lock() {
            pthread_mutex_lock(&m_mutex);
        }
        void unlock() {
            pthread_mutex_unlock(&m_mutex);
        }

    private:
         pthread_mutex_t m_mutex;
    };

    // 读写锁
    class RWMutex : Noncopyable {
    public:
        typedef ReadScopedLockImpl<RWMutex> ReadLock;
        typedef WriteScopedLockImpl<RWMutex> WriteLock;
        RWMutex() {
            pthread_rwlock_init(&m_lock, nullptr);
        }
        ~RWMutex() {
            pthread_rwlock_destroy(&m_lock);
        }
        void rdlock() {
            pthread_rwlock_rdlock(&m_lock);
        }
        void wrlock() {
            pthread_rwlock_wrlock(&m_lock);
        }
        void unlock() {
            pthread_rwlock_unlock(&m_lock);
        }

    private:
        pthread_rwlock_t m_lock;
    };

}





#endif //JUSTXM_MUTEX_H
