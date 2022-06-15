//
// Created by 肖应雄 on 2022/6/14.
//

#ifndef JUSTXM_CONFIGTEST_H
#define JUSTXM_CONFIGTEST_H

#include "../config/Config.h"

namespace ConfigTest{
// ConfigVar 测试
void test01(){
    xm::ConfigVar<int>::ptr config(new xm::ConfigVar<int> ("xx","xx",123));
    cout << config->toString();
    config->fromString("123");
    cout << config->getMVal() << endl;
    ROOT_LOG_DEBUG(xm::LogLevel::DEBUG) << "hey" << endl;
}


}

#endif //JUSTXM_CONFIGTEST_H
