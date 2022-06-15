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
}

// Yaml 测试
void test02(){
    xm::ConfigVar<int>::ptr s = xm::Config::Lookup("system.port",8888);
    cout << s->getMName() << ":" << s->getMVal() << endl;
    YAML::Node root = YAML::LoadFile("/Users/xiaoyingxiong/projects/ClionProjects/Justxm/test.yaml");
    xm::Config::LoadFromYaml(root);
    cout << s->getMName() << ":" << s->getMVal() << endl;
}

// 支持复杂类型
void test03(){
        xm::ConfigVar<vector<int>>::ptr s = xm::Config::Lookup("system.port",
                                                               vector<int>{1,1,1,1,1});
        cout << s->getMName() << ":" <<  s->toString()<< endl;
        YAML::Node root = YAML::LoadFile("/Users/xiaoyingxiong/projects/ClionProjects/Justxm/test.yaml");
        xm::Config::LoadFromYaml(root);
        cout << s->getMName() << ":" << s->toString() << endl;
}




}

#endif //JUSTXM_CONFIGTEST_H
