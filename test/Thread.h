//
// Created by xm on 2022/6/25.
//

#ifndef JUSTXM_THREADX_H
#define JUSTXM_THREAD_H

#include "../thread/Threadx.h"
#include <iostream>
using namespace std;



namespace threadTest {

    void method1() {
        cout << "name:"
        << xm::Threadx::getName
        << endl;
    }
    void method2() {
        cout << "test02" << endl;
    }

    void test01() {
        vector<xm::Threadx::ptr> thrs;
        for (int i = 0; i < 5; ++i) {
            xm::Threadx::ptr  thr(new xm::Threadx(&method1,"xxx" + to_string(i)));
            thrs.push_back(thr);
        }
        for (int i = 0; i < 5; ++i) {
            thrs[i]->join();
        }
        return;
    }



}


#endif //JUSTXM_THREADX_H
