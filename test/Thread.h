//
// Created by xm on 2022/6/25.
//

#ifndef JUSTXM_THREADX_H
#define JUSTXM_THREAD_H

#include "../thread/Threadx.h"
#include <iostream>
#include <sys/types.h>
#include "../thread/Mutex.h"
using namespace std;



namespace threadTest {

    void method1() {
        cout << "name:"
        << xm::Threadx::getName()
        << ":"
        << xm::Threadx::getPid()
        << endl;
    }
    void method2() {
        cout << "test02" << endl;
    }
    void test01() {
        vector<xm::Threadx::ptr> thrs;
        for (int i = 0; i < 5; ++i) {
            xm::Threadx::ptr  thr(new xm::Threadx(&method1,
                                                  "xxx" +
                                                  to_string(i)));
            sleep(1);
            thrs.push_back(thr);
        }
        for (int i = 0; i < 5; ++i) {
            thrs[i]->join();
        }
        return;
    }

    void test02 () {
        xm::Semaphore sem;
        sem.wait();
    }

    int volatile count = 0;
    xm::RWMutex s_mutex;
    xm::Mutex mutex;
    void method3() {
        for (int i = 0; i < 10; ++i) {
//            xm::RWMutex::WriteLock lock(s_mutex);
//            cout << "-.-" << endl;
            xm::Mutex::Lock  lock(mutex);
//            sleep(1);
            ++count;
        }
        cout << "name:"
             << xm::Threadx::getName()
             << ":"
             << xm::Threadx::getPid()
             << "count:" << count
             << endl;
    }
    void test03() {
        vector<xm::Threadx::ptr> thrs;
        for (int i = 0; i < 100; ++i) {
            xm::Threadx::ptr thr(new xm::Threadx(&method3, "name_"+ to_string(i)));
            thrs.push_back(thr);
        }
        for (const auto &item : thrs)
            item->join();
    }

    void test05() {
        vector<xm::Threadx::ptr> thrs;
        for (int i = 0; i < 10; ++i) {
            xm::Threadx::ptr thr(new xm::Threadx(&method3, "name_"+ to_string(i)));
            thrs.push_back(thr);
        }
        for (const auto &item : thrs)
            item->join();
        cout << count << endl;

    }


}


#endif //JUSTXM_THREADX_H
