//
// Created by 肖应雄 on 2022/6/9.
//

#include "Log.h"
#include "map"
#include "FormatItem.h"
namespace xm{


    const char* LogLevel::ToString(LogLevel::Level level) {
        switch(level) {
#define XX(name) \
    case LogLevel::name: \
        return #name; \
        break;

            XX(DEBUG);
            XX(INFO);
            XX(WARN);
            XX(ERROR);
            XX(FATAL);
#undef XX
            default:
                return "UNKNOW";
        }
        return "UNKNOW";
    }

    LogLevel::Level LogLevel::FromString(const std::string& str) {
#define XX(level, v) \
    if(str == #v) { \
        return LogLevel::level; \
    }
        XX(DEBUG, debug);
        XX(INFO, info);
        XX(WARN, warn);
        XX(ERROR, error);
        XX(FATAL, fatal);

        XX(DEBUG, DEBUG);
        XX(INFO, INFO);
        XX(WARN, WARN);
        XX(ERROR, ERROR);
        XX(FATAL, FATAL);
        return LogLevel::UNKNOW;
#undef XX
    }

LogLevel::Level Logger::getMLevel() const {
    return m_level;
}

void Logger::setMLevel(xm::LogLevel::Level mLevel) {
    m_level = mLevel;
}

const string &Logger::getMName() const {
    return m_name;
}

void xm::Logger::setMName(const string &mName) {
    m_name = mName;
}

const xm::LogFormatter::ptr &xm::LogAppender::getMFormatter() const {
    return m_formatter;
}

void xm::LogAppender::setMFormatter(const xm::LogFormatter::ptr &mFormatter) {
    m_formatter = mFormatter;
}

xm::LogLevel::Level xm::LogAppender::getMLevel() const {
    return m_level;
}

void xm::LogAppender::setMLevel(xm::LogLevel::Level mLevel) {
    m_level = mLevel;
}

void StdoutLogAppender::log(shared_ptr<Logger> logger,
                                xm::LogLevel::Level level,
                                xm::LogEvent::ptr event) {
    if (level >= m_level){
        m_formatter->format(cout, logger,level,event);
    }
}



void FileLogAppender::log(shared_ptr<Logger> logger, xm::LogLevel::Level level, xm::LogEvent::ptr event) {
    if (level >= m_level){
        uint64_t now = event->getMTime();
        if (now >= (m_lastTime + 3)){
            reopen();
            m_lastTime = now;
        }
        if (!m_formatter->format(m_filestream, logger,level,event)){
            cout << "error" << endl;
        }
    }
}

xm::FileLogAppender::FileLogAppender(const string &mFilename) : m_filename(mFilename) {
    reopen();
}

xm::LogFormatter::LogFormatter(const string &mPattern) : m_pattern(mPattern) {
    init();
}


    string LogFormatter::format(shared_ptr<Logger> logger,
                                LogLevel::Level level,
                                LogEvent::ptr event) {
        stringstream ss;
        for (const auto &item : m_items)
            item->format(ss,logger,level,event);
        return ss.str();
    }


    ostream &LogFormatter::format(ostream &ofs,
                                  shared_ptr<Logger> logger,
                                  LogLevel::Level level, LogEvent::ptr event) {
        for (const auto &item : m_items)
            item->format(ofs,logger,level,event);
        return ofs;
    }

    void LogFormatter::init() {
        //str, format, type
        // 默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n“
        std::vector<std::tuple<std::string, std::string, int> > vec;
        std::string nstr;
        for(size_t i = 0; i < m_pattern.size(); ++i) {
            if(m_pattern[i] != '%') {
                nstr.append(1, m_pattern[i]);
                continue;
            }

            if((i + 1) < m_pattern.size()) {
                if(m_pattern[i + 1] == '%') {
                    nstr.append(1, '%');
                    continue;
                }
            }

            size_t n = i + 1;
            int fmt_status = 0;
            size_t fmt_begin = 0;

            std::string str;
            std::string fmt;
            while(n < m_pattern.size()) {
                if(!fmt_status && (!isalpha(m_pattern[n]) && m_pattern[n] != '{'
                                   && m_pattern[n] != '}')) {
                    str = m_pattern.substr(i + 1, n - i - 1);
                    break;
                }
                if(fmt_status == 0) {
                    if(m_pattern[n] == '{') {
                        str = m_pattern.substr(i + 1, n - i - 1);
                        //std::cout << "*" << str << std::endl;
                        fmt_status = 1; //解析格式
                        fmt_begin = n;
                        ++n;
                        continue;
                    }
                } else if(fmt_status == 1) {
                    if(m_pattern[n] == '}') {
                        fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                        //std::cout << "#" << fmt << std::endl;
                        fmt_status = 0;
                        ++n;
                        break;
                    }
                }
                ++n;
                if(n == m_pattern.size()) {
                    if(str.empty()) {
                        str = m_pattern.substr(i + 1);
                    }
                }
            }

            if(fmt_status == 0) {
                if(!nstr.empty()) {
                    vec.push_back(std::make_tuple(nstr, std::string(), 0));
                    nstr.clear();
                }
                vec.push_back(std::make_tuple(str, fmt, 1));
                i = n - 1;
            } else if(fmt_status == 1) {
                std::cout << "pattern parse error: " << m_pattern << " - " << m_pattern.substr(i) << std::endl;
                m_error = true;
                vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
            }
        }

        if(!nstr.empty()) {
            vec.push_back(std::make_tuple(nstr, "", 0));
        }
        static std::map<std::string, std::function<FormatItem::ptr(const std::string& str)> > s_format_items = {
#define XX(str, C) \
        {#str, [](const std::string& fmt) { return FormatItem::ptr(new C(fmt));}}

                XX(m, MessageFormatItem),           //m:消息
                XX(p, LevelFormatItem),             //p:日志级别
                XX(r, ElapseFormatItem),            //r:累计毫秒数
                XX(c, NameFormatItem),              //c:日志名称
                XX(t, ThreadIdFormatItem),          //t:线程id
                XX(n, NewLineFormatItem),           //n:换行
                XX(d, DateTimeFormatItem),          //d:时间
                XX(f, FilenameFormatItem),          //f:文件名
                XX(l, LineFormatItem),              //l:行号
                XX(T, TabFormatItem),               //T:Tab
                XX(F, FiberIdFormatItem),           //F:协程id
                XX(N, ThreadNameFormatItem),        //N:线程名称
#undef XX
        };

        for(auto& i : vec) {
            if(std::get<2>(i) == 0) {
                m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
            } else {
                auto it = s_format_items.find(std::get<0>(i));
                if(it == s_format_items.end()) {
                    m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
                    m_error = true;
                } else {
                    m_items.push_back(it->second(std::get<1>(i)));
                }
            }

        }
        return;
    }




const string &xm::LogFormatter::getMPattern() const {
    return m_pattern;
}

bool xm::FileLogAppender::reopen() {
    if (m_filestream){
        m_filestream.close();
    }
    return FSUtil::OpenForWrite(m_filestream,m_filename,ios::app);
}

Logger::Logger(const string &name) :m_name(name),m_level(LogLevel::DEBUG){
        m_formatter.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    }

void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
        if (level >= m_level){
            auto self = shared_from_this();
            if (!m_appenders.empty()){
                for (const auto &item : m_appenders){
                    item->log(self,level,event);
                }
            } else if (m_root){
                m_root->log(level,event);
            }
        }
    }

    LogEvent::LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level
            ,const char* file, int32_t line, uint32_t elapse
            ,uint32_t thread_id, uint32_t fiber_id, uint64_t time
            ,const std::string& thread_name)
            :m_file(file)
            ,m_line(line)
            ,m_elapse(elapse)
            ,m_threadId(thread_id)
            ,m_fiberId(fiber_id)
            ,m_time(time)
            ,m_threadName(thread_name)
            ,m_logger(logger)
            ,m_level(level) {
    }

    void Logger::addAppender(LogAppender::ptr appender) {
        if (!appender->getMFormatter()){
            appender->setMFormatter(m_formatter);
        }
        m_appenders.push_back(appender);
    }



}
