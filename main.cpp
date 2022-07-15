#include <iostream>
#include "log/Log.h"
#include "test/LogTest.h"
#include "test/ConfigTest.h"
#include "test/Thread.h"
#include <pthread.h>
#include "test/UcontextTest.h"
#include "test/FiberTest.h"
void forLogTest();
void forConfigTest();
void forThreadTest();
void forUcontextTest();
void forFiberTest();
int main() {
//    forLogTest();
//    forConfigTest();
//    forThreadTest();
//    forUcontextTest();
//    cout << "hello word" << endl;
    forFiberTest();
}

void forFiberTest() {
    FiberTest::test01();
}

void forUcontextTest() {
//    test01();
    test02();
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
