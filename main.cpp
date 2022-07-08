#include <iostream>
#include "log/Log.h"
#include "test/LogTest.h"
#include "test/ConfigTest.h"
#include "test/Thread.h"
#include <pthread.h>

void forLogTest();
void forConfigTest();
void forThreadTest();
int main() {
//    forLogTest();
//    forConfigTest();
    forThreadTest();
}

void forThreadTest(){
    threadTest::test05();
//    threadTest::test03();
//    threadTest::test01();
//    threadTest::test02();
}

void forConfigTest(){
//    ConfigTest::test01();
//    ConfigTest::test02();
//    ConfigTest::test03();
//    ConfigTest::test04();
//    ConfigTest::test05();
}

void forLogTest(){
//    logTest::test01();
//    logTest::test02();
//    logTest::test03();
//    logTest::test04();
//    logTest::test05();
//    logTest::test06();
}
