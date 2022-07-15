//
// Created by xm on 2022/7/12.
//

#include "Fiber.h"
using namespace std;
#include <atomic>
#include <iostream>
#include "../log/Log.h"
namespace xm {


    static atomic<int64_t> s_fiber_id{0};
    static atomic<int64_t> s_fiber_count{0};

    static thread_local Fiber *t_fiber = nullptr;   // 裸指针
    static thread_local Fiber::ptr t_threadFiber = nullptr;   // 智能指针  这个好像是主协程

    class MallocStackAllocator {
    public:
        static void* Alloc(size_t size) {
            return malloc(size);
        }
        static void Dealloc(void *vp, size_t size) {
            return free(vp);
        }

    };

    uint64_t Fiber::GetFiberId() {
        if (t_fiber)
            return t_fiber->getId();
        return 0;
    }

    Fiber::Fiber() {
        m_state = EXEC;
        SetThis(this);
        if (getcontext(&m_ctx))
            cout << "getcontext error" << endl;
        ++ s_fiber_count;
        ROOT_LOG(INFO()) << "Fiber::Fiber main";
    }

    Fiber::Fiber(std::function<void()> cb, size_t stacksize, bool use_caller)
    :m_id(++s_fiber_id),
    m_cb(cb)
    {
        ++ s_fiber_count;
        m_stacksize = stacksize;
        m_stack = MallocStackAllocator::Alloc(stacksize);
        if (getcontext(&m_ctx))
            cout << "getcontext error" << endl;
        m_ctx.uc_link = nullptr;
        m_ctx.uc_stack.ss_size = m_stacksize;
        m_ctx.uc_stack.ss_sp = m_stack;

        if (!use_caller)  // 不在主协程上调度
            makecontext(&m_ctx, &Fiber::MainFunc, 0);
        else   // 在主协程上调度
            makecontext(&m_ctx, &Fiber::CalleMainFunc, 0);

        ROOT_LOG(INFO()) << "Fiber::Fiber id = " << m_id ;
    }

    Fiber::~Fiber() {
        --s_fiber_count;
        if (m_stack){
            if (m_state == TERM || m_state == EXCEPT || m_state == INIT)
                cout << "is error" << endl;
            MallocStackAllocator::Dealloc(m_stack, m_stacksize);
        } else {
            Fiber *cur = t_fiber;
            if (cur == this)
                SetThis(nullptr);
        }
        ROOT_LOG(INFO()) << "Fiber::~Fiber id =" << m_id
                        << "total=" << s_fiber_count;
    }

    void Fiber::reset(std::function<void()> cb) {
        m_cb = cb;
        getcontext(&m_ctx);
        m_ctx.uc_link = nullptr;
        m_ctx.uc_stack.ss_sp =  m_stack;
        m_ctx.uc_stack.ss_size = m_stacksize;

        makecontext(&m_ctx, &Fiber::MainFunc, 0);
        m_state = INIT;
    }

    void Fiber::call() {
        SetThis(this);
        m_state = EXEC;
        if (swapcontext(&t_threadFiber->m_ctx, &m_ctx))
            cout << "swapcontext error" << endl;

    }

    void Fiber::back() {
        SetThis(t_threadFiber.get());
        if (swapcontext(&m_ctx, &t_threadFiber->m_ctx))
            cout << "swaperror " << endl;
    }

    // 这两个有点问题要看一下
    void Fiber::swapIn() {
        SetThis(this);
        m_state = EXEC;
        if (t_threadFiber == nullptr)
            cout << "1" << endl;
        if (swapcontext(&t_threadFiber->m_ctx, &m_ctx))
            cout << "swap error" << endl;
    }
    // 这个也有点问题
    void Fiber::swapOut() {
        SetThis(t_threadFiber.get());
        if (swapcontext(&m_ctx, &t_threadFiber->m_ctx))
            cout << "swap error" << endl;
    }

    void Fiber::SetThis(Fiber *f) {
        t_fiber = f;
    }

    // 返回当前协程
    Fiber::ptr Fiber::GetThis() {
        if (t_fiber)
            return t_fiber->shared_from_this();
        Fiber::ptr main_fiber(new Fiber);
        if (t_fiber != main_fiber.get())
            cout << "Fiber::GetThis error" << endl;
        t_threadFiber = main_fiber;
        return t_fiber->shared_from_this();
    }

    // 协程切换到后台 并设置为ready状态
    void Fiber::YieldToReady() {
        Fiber::ptr cur = GetThis();
        if (cur->m_state == EXEC)
            cout << "cur is ECEX" << endl;
        cur->m_state = READY;
        cur->swapOut();
    }

    void Fiber::YieldToHold() {
        Fiber::ptr  cur = GetThis();
        if (cur->m_state == EXEC)
            cout << "YieldToHold is EXEC" << endl;
        cur->swapOut();
    }

    uint64_t Fiber::TotalFibers() {
        return s_fiber_count;
    }

    void Fiber::MainFunc() {
        Fiber::ptr cur = GetThis();
        cur->m_cb();
        cur->m_cb = nullptr;
        cur->m_state = TERM;

        auto p = cur.get();
        cur.reset();   //  智能指针不再指向该对象  计数器--
        p->swapOut();
    }

    void Fiber::CalleMainFunc() {
        Fiber::ptr cur = GetThis();
        cur->m_cb();
        cur->m_cb = nullptr;
        cur->m_state = TERM;

        auto p = cur.get();
        cur.reset();
        p->back();
    }






}