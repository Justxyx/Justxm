//
// Created by 肖应雄 on 2022/6/14.
//

#ifndef JUSTXM_SINGLETON_H
#define JUSTXM_SINGLETON_H

#include <memory>
namespace xm {

    template<class T, class X = void, int N = 0>
    class Singleton {
    public:
        static T* GetInstance(){
            static T v;
            return &v;
        }
    };

}


#endif //JUSTXM_SINGLETON_H
