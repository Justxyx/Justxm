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

    static thread_local Fiber *t_fiber = nullptr;   // 裸指针  当前协程
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

    Fiber::Fiber() {   // 主协程调用  main 执行
        m_state = EXEC;
        SetThis(this);
        if (getcontext(&m_ctx))   // 保存主线程状态
            cout << "getcontext error" << endl;
        ++ s_fiber_count;  // 总协程 从 0 变到了 1
        ROOT_LOG(INFO()) << "Fiber::Fiber main";
    }

    // 这个才是真正的创建新的协程
    Fiber::Fiber(std::function<void()> cb, size_t stacksize, bool use_caller)
    :m_id(++s_fiber_id),
    m_cb(cb)
    {
        ++ s_fiber_count;
        m_stacksize = stacksize;
        if (stacksize == 0)
            stacksize = 1024 * 1024;
        m_stack = MallocStackAllocator::Alloc(stacksize);
        if (getcontext(&m_ctx))
            cout << "getcontext error" << endl;
        m_ctx.uc_link = nullptr;   // 我自己结束时候 自动切换到uc_link的上下文
        m_ctx.uc_stack.ss_size = m_stacksize;
        m_ctx.uc_stack.ss_sp = m_stack;

        if (!use_caller)  // 不在主协程上调度
            makecontext(&m_ctx, &Fiber::MainFunc, 0);
        else   // 在主协程上调度
            makecontext(&m_ctx, &Fiber::MainFunc, 0);

        ROOT_LOG(INFO()) << "Fiber::Fiber id = " << m_id ;
    }

    Fiber::~Fiber() {
        --s_fiber_count;
        if (m_stack){
            if (m_state == TERM || m_state == EXCEPT || m_state == INIT)
                cout << "is error" << endl;
            MallocStackAllocator::Dealloc(m_stack, m_stacksize);
        } else {   // 如果没有栈空间 那么就是主协程
            Fiber *cur = t_fiber;
            if (cur == this)
                SetThis(nullptr);
        }
        ROOT_LOG(INFO()) << "Fiber::~Fiber id =" << m_id
                        << "total=" << s_fiber_count;
    }

    void Fiber::reset(std::function<void()> cb) {
        if (m_stack == nullptr) {
            cout << "reset error" << endl;  // 主协程  不可以重置回调函数
            return;
        }
        if  (m_state == INIT || m_state == TERM) {  // 只有这两种执行完的状态才可以切换回调函数
        m_cb = cb;
        getcontext(&m_ctx);
        m_ctx.uc_link = nullptr;
        m_ctx.uc_stack.ss_sp =  m_stack;
        m_ctx.uc_stack.ss_size = m_stacksize;

        makecontext(&m_ctx, &Fiber::MainFunc, 0);
        m_state = INIT;
            return;
        } else {
            cout << "reset error" << endl;
            return;
        }
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

    // 把当前的协程拿出来， 运行这个协程 暂停主协程 运行这个子协程
    void Fiber::swapIn() {
        SetThis(this);
        m_state = EXEC;
        if (t_threadFiber == nullptr)
            cout << "1" << endl;
        if (swapcontext(&t_threadFiber->m_ctx, &m_ctx))
            cout << "swap error" << endl;
    }
    // 把当前协程挂到后台 运行main协程
    void Fiber::swapOut() {
        SetThis(t_threadFiber.get());
        if (swapcontext(&m_ctx, &t_threadFiber->m_ctx))
            cout << "swap error" << endl;
    }

    // 设置当前正在执行的协程
    void Fiber::SetThis(Fiber *f) {
        t_fiber = f;
    }

    // 返回当前正在执行的协程
    Fiber::ptr Fiber::GetThis() {
        if (t_fiber)
            return t_fiber->shared_from_this();
        Fiber::ptr main_fiber(new Fiber);  // 如果没有的话 就创建一个主协程
        if (t_fiber != main_fiber.get())
            cout << "Fiber::GetThis error" << endl;
        t_threadFiber = main_fiber;
        return t_fiber->shared_from_this();
    }

    // 当前线程切换到ready 状态 并切换到主协程上去
    void Fiber::YieldToReady() {
        Fiber::ptr cur = GetThis();
        if (cur->m_state == EXEC)
            cout << "cur is ECEX" << endl;
        cur->m_state = READY;
        cur->swapOut();
    }
    // 当前线程切换到hold 状态 并切换到主协程上去
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
//        try {
            cur->m_cb();
            cur->m_cb = nullptr;
            cur->m_state = TERM;
//        }catch (...) {
//            cur->m_state = EXCEPT;
//            cout << "Fiber EXcept" << endl;
//        }

//        auto p = cur.get();
//        cur.reset();   //  智能指针不再指向该对象  计数器--
//        p->swapOut();
    }

    // 暂时不用
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