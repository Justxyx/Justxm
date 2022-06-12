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
        std->setMLevel(xm::LogLevel::WARN);
        logger->addAppender(std);
        logger->addAppender(xm::FileLogAppender::ptr(new xm::FileLogAppender("/Users/xiaoyingxiong/projects/ClionProjects/Justxm/a.txt")));
        xm::LogEvent::ptr event(new xm::LogEvent(logger,xm::LogLevel::DEBUG,"zzz",12,
                                                 12,12,12,12,"xx"));
        logger->log(xm::LogLevel::DEBUG,event);
    }
}
#endif //JUSTXM_LOGTEST_H
