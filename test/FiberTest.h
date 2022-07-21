//
// Created by xm on 2022/7/15.
//

#ifndef JUSTXM_FIBERTEST_H
#define JUSTXM_FIBERTEST_H
#include <iostream>
#include "../Fiber/Fiber.h"
using namespace std;
namespace FiberTest {
    void m1() {
        cout << "m1 begin" << endl;
        xm::Fiber::YieldToHold();
        cout << "m1 end" << endl;
        xm::Fiber::YieldToHold();
    }

    void test01() {
        xm::Fiber::GetThis();
        cout << "begin" << endl;
        xm::Fiber::ptr fiber(new xm::Fiber(m1, 1024, true));
        fiber->getId();
        fiber->swapIn();
        cout << "main after swapin" << endl;
        fiber->swapIn();
        cout << "main after end" << endl;
        return;
    }
}


#endif //JUSTXM_FIBERTEST_H
