//
// Created by xm on 2022/7/12.
//

#ifndef JUSTXM_UCONTEXTTEST_H
#define JUSTXM_UCONTEXTTEST_H
#include "ucontext.h"
using namespace std;
#include <iostream>
#include <functional>
ucontext_t parent,child;

void F2() {
    for (int i = 0; i < 5; ++i) {
        cout << "child :" << i << endl;
        swapcontext(&child, &parent);
    }
}

void F1() {
    char stack[1024];
    getcontext(&child);
    child.uc_stack.ss_sp = stack;
    child.uc_stack.ss_size = sizeof(stack);
    child.uc_link = &parent;
//    function<void(void)> f = F2;

    makecontext(&child, (void(*)(void)) F2, 0);

    for (int i = 0; i < 5; ++i) {
        cout << "parent: " << i << endl;
        swapcontext(&parent, &child);
    }

}

void test01() {
    F1();
}

int add(int a, int b) {
    return a+b;
}
void test02() {
    function<int(int, int)> f = add;
    cout << f(1, 2) << endl;
    cout << "hey" << endl;
}

#endif //JUSTXM_UCONTEXTTEST_H
