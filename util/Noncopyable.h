//
// Created by xm on 2022/7/8.
//

#ifndef JUSTXM_NONCOPYABLE_H
#define JUSTXM_NONCOPYABLE_H


/*
 * 禁止对象拷贝 赋值 构造的工具类
 */
namespace xm {
    class Noncopyable {
    public:
        Noncopyable() = default;
        ~Noncopyable() = default;
        // 禁止拷贝构造
        Noncopyable(Noncopyable&) = default;
        // 禁止复制构造
        Noncopyable operator=(Noncopyable&) = delete;
    };
};

#endif //JUSTXM_NONCOPYABLE_H
