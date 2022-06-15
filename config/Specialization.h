//
// Created by 肖应雄 on 2022/6/14.
//

#ifndef JUSTXM_SPECIALIZATION_H
#define JUSTXM_SPECIALIZATION_H

#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>
/*
 * Config 中的所有特化模板函数放置于此
 */
namespace xm {

    // F from_type   T to_type
    template<class F,class T>
    class LexicalCast {
    public:
        T operator() (const F &v){
            return boost::lexical_cast<T>(v);
        }
    };

    template<class T>
    class LexicalCast<string,vector<T>>{
    public:
        vector<T> operator() (const string &v) {
            YAML::Node node = YAML::Load(v);
            vector<T> vec;
            stringstream ss;
            for (const auto &item : node){
                ss.str("");
                ss << item;
                vec.push_back(LexicalCast<string,T>()(ss.str()));
            }
            return vec;
        }
    };

    template<class T>
    class LexicalCast<vector<T>,string> {
    public:
        string operator()(vector<T> &v){
            YAML::Node node;
            for (const auto &item : v){
                node.push_back(YAML::Load(LexicalCast<T,string>()(item)));
            }
            stringstream ss;
            ss << node;
            return ss.str();
        }
    };

}



#endif //JUSTXM_SPECIALIZATION_H
