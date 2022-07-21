//
// Created by xm on 2022/7/12.
//

#ifndef JUSTXM_FIBER_H
#define JUSTXM_FIBER_H
#include <functional>
#include <memory>
#include <ucontext.h>
namespace xm {

    class Fiber : public std::enable_shared_from_this<Fiber> {
    public:
        enum State {
            INIT,  // 初始化窗台
            HOLD,  // 暂停状态
            EXEC,  // 执行中状态
            TERM,  // 结束状态
            READY, // 可执行状态
            EXCEPT  // 异常状态
        };
    private:
        Fiber();  // 每个线程运行的第一个函数  无参构造函数
    public:
        typedef std::shared_ptr<Fiber> ptr;
        /**
         *
         * @param cb
         * @param stacksize
         * @param use_caller   是否在 主协程上调度
         */
        Fiber(std::function<void()> cb, size_t stacksize = 0, bool use_caller = false);
        ~Fiber();
        void reset(std::function<void()> cb);  // 重置协程的运行函数 并重置状态
        void swapIn();  // 将当前协程切换到运行状态
        void swapOut(); // 将当前协程切换到后台
        void call();  // 将当前协程切换到执行状态  执行前为当前线程的主线程
        void back();  // 将当前线程返回到后台  执行后返回到该线程的主协程
        uint64_t getId(){ return m_id;}   // 返回协程id
        State getState() const {return m_state;}  // 返回状态
    public:
        static void SetThis(Fiber *f);  // 设置当前线程的运行的协程
        static Fiber::ptr GetThis();   // 返回当前所在协程  拿到当前协程
        static void YieldToReady();    // 将当前线程切换到后台 并设置为ready 状态  让出当前协程
        static void YieldToHold();     // 将当前线程切换到后台 并设置为HOLD状态
        static uint64_t TotalFibers();   // 总协程数
        static void MainFunc();    // 协程执行函数 执行完返回到线程的主协程
        static void CalleMainFunc();  // 协程执行函数 执行完返回到线程调度协程  1
        static uint64_t GetFiberId();    // 返回当前协程id
    private:
        uint64_t m_id = 0;  // 协程id
        uint64_t m_stacksize = 0;  // 协程运行栈大小
        State m_state = INIT;   // 协程状态
        ucontext_t m_ctx;   // 协程上下文
        void *m_stack = nullptr;    // 协程运行栈指针
        std::function<void()> m_cb; // 协程运行函数
    };

}


#endif //JUSTXM_FIBER_H
