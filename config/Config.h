//
// Created by 肖应雄 on 2022/6/14.
//

#ifndef JUSTXM_CONFIG_H
#define JUSTXM_CONFIG_H

#include <iostream>
using namespace std;
#include <boost/lexical_cast.hpp>
#include "Specialization.h"
#include "../log/Log.h"
namespace xm {

    class ConfigVArBase {
    public:
        typedef shared_ptr<ConfigVArBase> ptr;

        ConfigVArBase(const string &mName, const string &mDescription);
        ~ConfigVArBase(){};

        const string &getMName() const;
        const string &getMDescription() const;

        virtual string toString() = 0;
        virtual bool fromString(const string val) = 0;

    private:
        string m_name;
        string m_description;
    };

    template<class T, class FromStr = LexicalCast<string,T>,
            class ToStr = LexicalCast<T,string>>
    class ConfigVar: public ConfigVArBase {
    public:
        typedef shared_ptr<ConfigVar> ptr;

        ConfigVar(const string &mName, const string &mDescription, T mVal) : ConfigVArBase(mName, mDescription),
                                                                             m_val(mVal) {}

        T getMVal() const {return m_val;}
        void setMVal(T mVal) {m_val = mVal;}

        string toString() override {
            try {
                return ToStr()(m_val);
            }catch (exception e){
                xm::Logger::ptr logger = XM_LOG_ROOT();
                XM_LOG_DEBUG(logger) << "ConfigVar::ToString exception: " << e.what() << endl;
            }
            return "";
        }

        bool fromString(const string val) override {
            try {
                setMVal(FromStr()(val));
                return true;
            }catch (exception e) {
                xm::Logger::ptr logger = XM_LOG_ROOT();
                XM_LOG_DEBUG(logger) << "ConfigVar::FromStr exeception: " << e.what() << endl;
            }
            return false;
        }

    private:
        T m_val;
    };

}
#endif //JUSTXM_CONFIG_H
