//
// Created by 肖应雄 on 2022/6/14.
//

#ifndef JUSTXM_SPECIALIZATION_H
#define JUSTXM_SPECIALIZATION_H

#include <boost/lexical_cast.hpp>

/*
 * Config 中的所有特化模板函数放置于此
 */
namespace xm {

    template<class F,class T>
    class LexicalCast {
    public:
        T operator() (const F &v){
            return boost::lexical_cast<T>(v);
        }
    };
}



#endif //JUSTXM_SPECIALIZATION_H
