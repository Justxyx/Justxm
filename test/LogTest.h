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
}
#endif //JUSTXM_LOGTEST_H
