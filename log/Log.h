//
// Created by 肖应雄 on 2022/6/9.
//

#ifndef JUSTXM_LOG_H
#define JUSTXM_LOG_H

#include <iostream>
#include <sstream>
#include <list>
#include <fstream>
#include <vector>
#include "../util/Util.h"
using namespace std;

namespace xm{

    class Logger;

/*
 * 日志等级
 */
class LogLevel{
public:
    enum Level{
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5,
    };
    static const char* ToString(LogLevel::Level level);
    static LogLevel::Level FromString(const string &str);
};

/*
 * 日志事件
 */
class LogEvent{
public:
    typedef shared_ptr<LogEvent> ptr;

    string getContent() const {return m_ss.str();};
    uint32_t getMElapse() const {return m_elapse;}
    const shared_ptr<Logger> &getMLogger() const {return m_logger;}
    uint32_t getMThreadId() const {return m_threadId;}
    uint32_t getMFiberId() const {return m_fiberId;}
    const string &getMThreadName() const {return m_threadName;}
    uint32_t getMTime() const {return m_time;}
    const char *getMFile() const {return m_file;}
    int32_t getMLine() const {return m_line;}

    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level
            ,const char* file, int32_t line, uint32_t elapse
            ,uint32_t thread_id, uint32_t fiber_id, uint64_t time
            ,const std::string& thread_name);


private:
    const char* m_file = nullptr;
    int32_t m_line = 0;
    uint32_t m_elapse = 0; // 程序启动到现在的毫秒数
    uint32_t m_threadId = 0;
    uint32_t m_fiberId = 0; // 携程id
    uint32_t m_time = 0;
    string m_threadName = 0; // 线程名称
    std::stringstream m_ss;  //日志内容流
    shared_ptr<Logger> m_logger;
    LogLevel::Level m_level;  // 日志等级
};

/*
 * 包装的日志类
 */
class LogEventWarp{
private:
    LogEvent::ptr m_event;
};

/*
 * 日志格式化
 */
class LogFormatter{
public:
    typedef shared_ptr<LogFormatter> ptr;
        /**
        *  %m 消息
        *  %p 日志级别
        *  %r 累计毫秒数
        *  %c 日志名称
        *  %t 线程id
        *  %n 换行
        *  %d 时间
        *  %f 文件名
        *  %l 行号
        *  %T 制表符
        *  %F 协程id
        *  %N 线程名称
        *
        *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
        */
    LogFormatter(const string &mPattern);

    string format(shared_ptr<Logger> logger,LogLevel::Level level, LogEvent::ptr event);
    ostream& format(ostream &ofs,shared_ptr<Logger> logger,LogLevel::Level level, LogEvent::ptr event);

public:
    class FormatItem{
    public:
        typedef shared_ptr<FormatItem> ptr;
        virtual ~FormatItem(){};
        virtual void format(ostream &os,shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;
    };

    void init();
    bool isError() const { return m_error;};
    const string &getMPattern() const;

private:
    string m_pattern;  // 日志格式模板
    vector<FormatItem::ptr> m_items;  // 解析后的日志格式
    bool m_error = false;   // 是否有错误
};

/*
 * 日志输出器
 */
class LogAppender{
public:
    friend class Logger;
    typedef shared_ptr<LogAppender> ptr;

    virtual ~LogAppender(){};

    virtual void log(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

    const LogFormatter::ptr &getMFormatter() const;  // Formatter
    void setMFormatter(const LogFormatter::ptr &mFormatter);

    LogLevel::Level getMLevel() const;      // get set level
    void setMLevel(LogLevel::Level mLevel);

protected:
    LogLevel::Level m_level = LogLevel::DEBUG;
    bool m_hasFormatter = false; // 是否有自己的日志格式器
    LogFormatter::ptr m_formatter;  // 日志格式器
};

/*
 * 输出到控制台的输出器
 */
class StdoutLogAppender : public  LogAppender{
public:
    typedef shared_ptr<StdoutLogAppender> ptr;

    void log(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
};

/*
 * 输出到文件
 */
class FileLogAppender : public LogAppender{
public:
    typedef shared_ptr<FileLogAppender> ptr;

    FileLogAppender(const string &mFilename);

    void log(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

    bool reopen();

private:
    string m_filename;   // 文件路径
    ofstream m_filestream;   // 文件流
    u_int64_t m_lastTime = 0;  // 上次重新打开的时间
};

/*
 * 日志器
 */
class Logger : public enable_shared_from_this<Logger>{
public:
    typedef shared_ptr<Logger> ptr;

    Logger(const string &name = "root");  // 构造

    void log(LogLevel::Level level,LogEvent::ptr event);  // 写日志
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    void addAppender(LogAppender::ptr appender);  // 增删输出器
    void deleteAppender(LogAppender::ptr appender);
    void clearAppender(LogAppender::ptr appender);

    LogLevel::Level getMLevel() const;
    void setMLevel(LogLevel::Level mLevel);

    const string &getMName() const;
    void setMName(const string &mName);

    void setFormatter(LogFormatter::ptr formatter); // Formatter
    void setFormatter(const string &formatter);
    const LogFormatter::ptr getFormatter ()const;


private:
    string m_name;
    LogLevel::Level m_level;
    list<LogAppender::ptr> m_appenders;
    LogFormatter::ptr m_formatter;  // 默认foramtter
    Logger::ptr m_root;
};


}
#endif //JUSTXM_LOG_H
