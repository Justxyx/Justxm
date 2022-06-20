//
// Created by 肖应雄 on 2022/6/14.
//

#ifndef JUSTXM_CONFIGTEST_H
#define JUSTXM_CONFIGTEST_H

#include "../config/Config.h"
#include <functional>

namespace ConfigTest{

// ConfigVar 测试
void test01(){
//    xm::ConfigVar<int>::ptr config(new xm::ConfigVar<int> ("xx","xx",123));
//    cout << config->toString();
//    config->fromString("123");
//    cout << config->getMVal() << endl;
}

// Yaml 测试
void test02(){
//    xm::ConfigVar<int>::ptr s = xm::Config::Lookup("system.port",8888);
//    cout << s->getMName() << ":" << s->getMVal() << endl;
//    YAML::Node root = YAML::LoadFile("/Users/xiaoyingxiong/projects/ClionProjects/Justxm/test.yaml");
//    xm::Config::LoadFromYaml(root);
//    cout << s->getMName() << ":" << s->getMVal() << endl;
}

// 支持复杂类型
void test03(){
//        xm::ConfigVar<vector<int>>::ptr s = xm::Config::Lookup("system.port",
//                                                               vector<int>{1,1,1,1,1});
//        cout << s->getMName() << ":" <<  s->toString()<< endl;
//        YAML::Node root = YAML::LoadFile("/Users/xiaoyingxiong/projects/ClionProjects/Justxm/test.yaml");
//        xm::Config::LoadFromYaml(root);
//        cout << s->getMName() << ":" << s->toString() << endl;
}

// c++ 11 functional 用法
int c_func(int a, int b){
        return a+b;
}
void test04(){
    // c 老用法
//    typedef  int(*Func)(int,int);
//    Func func = c_func;
//    cout << "c_func 其int值为：" << c_func << endl;
//    cout <<  func(1,2);

    // c11 新用法
//    function<int(int,int)> funcs = c_func;
//    cout << funcs(1,2);
}

    void test(const vector<int> v1, const vector<int> v2){
        cout << "cb is running" << endl;
    }
// 变更配置事件
void test05() {
        xm::ConfigVar<vector<int>>::ptr s = xm::Config::Lookup("system.port",
                                                               vector<int>{1,1,1,1,1});

        s->addListener(12,test);

        YAML::Node root = YAML::LoadFile("/Users/xiaoyingxiong/projects/ClionProjects/Justxm/test.yaml");
        xm::Config::LoadFromYaml(root);
        cout << s->toString() << endl;

}



}

#endif //JUSTXM_CONFIGTEST_H
