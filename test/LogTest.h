//
// Created by 肖应雄 on 2022/6/9.
//

#ifndef JUSTXM_LOGTEST_H
#define JUSTXM_LOGTEST_H
#include "../log/Log.h"
namespace logTest {
    void test01() {
        xm::LogFormatter logFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");
        logFormatter.init();
    }

    void test02(){
        xm::FileLogAppender fileLogAppender("/Users/xiaoyingxiong/projects/ClionProjects/Justxm/temp/a.txt");
        fileLogAppender.reopen();
    }

    void test03(){
        xm::Logger::ptr logger(new xm::Logger);
        logger->addAppender(xm::LogAppender::ptr(new xm::StdoutLogAppender()));

        xm::LogEvent::ptr event(new xm::LogEvent(logger,xm::LogLevel::DEBUG,"xxx",12,
                                                       12,12,12,12,"xx"));
        logger->log(xm::LogLevel::DEBUG,event);
    }

    void test04(){
        xm::Logger::ptr logger(new xm::Logger);
//        logger->setMLevel(xm::LogLevel::WARN);
        xm::LogAppender::ptr std(new xm::StdoutLogAppender);
        std->setMLevel(xm::LogLevel::DEBUG);
        logger->addAppender(std);
        logger->addAppender(xm::FileLogAppender::ptr(new xm::FileLogAppender("/Users/xiaoyingxiong/projects/ClionProjects/Justxm/a.txt")));
        xm::LogEvent::ptr event(new xm::LogEvent(logger,xm::LogLevel::DEBUG,"zzz",12,
                                                 12,12,12,12,"xx"));
//        logger->log(xm::LogLevel::DEBUG,event);

//        xm::LogEventWarp lo(event);
        xm::LogEventWarp(xm::LogEvent::ptr(new xm::LogEvent(logger,xm::LogLevel::DEBUG,
                                                            __FILE__,__LINE__,0,xm::GetThreadId(),xm::GetFiberId(),
                                                            time(0),"thread name"))).getSS();
    }

    // 日志宏测试
    void test05(){
        xm::Logger::ptr logger(new xm::Logger);
        logger->addAppender(xm::LogAppender::ptr(new xm::StdoutLogAppender));
        XM_LOG_LEVEL(logger, xm::LogLevel::DEBUG);
//        logger->getMLevel();
        XM_LOG_DEBUG(logger);
        XM_LOG_INFO(logger);
    }
}
#endif //JUSTXM_LOGTEST_H
