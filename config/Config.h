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
#include <yaml-cpp/yaml.h>
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
                ROOT_LOG(LogLevel::DEBUG)<< "ConfigVar::ToString exception: " << e.what() << endl;
            }
            return "";
        }

        bool fromString(const string val) override {
            try {
                setMVal(FromStr()(val));
                return true;
            }catch (exception e) {
                ROOT_LOG(LogLevel::DEBUG) << "ConfigVar::FromStr exeception: " << e.what() << endl;
            }
            return false;
        }

    private:
        T m_val;
    };

    class Config{
    public:
        typedef map<string,ConfigVArBase::ptr> ConfigVarMap;

        template<class T>
        static typename ConfigVar<T>::ptr Lookup(const string &name,
                                                 const T &default_value,
                                                 const string &description = ""){
            // 名字存在 但是类型不同 会报错返空值
            auto it = GetDatas().find(name);
            if (it != GetDatas().end()) {
                auto temp = dynamic_pointer_cast<ConfigVar<T>>(it->second);
                if (!temp) {
                    ROOT_LOG(INFO()) << "trans error " << endl;
                    return nullptr;
                }
                if (temp) {
                    ROOT_LOG(INFO()) << "Lookup name = " << name << "exists" << endl;
                    return temp;
                }
            }
            if (name.find_first_not_of("abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._123456789")
                                        != string::npos) {
                ROOT_LOG(INFO()) << "Lookup name is invalid" << name << endl;
                throw invalid_argument(name);
            }
            typename ConfigVar<T>::ptr v(new ConfigVar<T>(name,description,default_value));
            GetDatas()[name] = v;
            return v;
        }

        static void LoadFromYaml(const YAML::Node &root);
        static ConfigVArBase::ptr LookUpBase(const string &name);
    private:
        static ConfigVarMap& GetDatas(){
            static ConfigVarMap s_datas;
            return s_datas;
        }
    };

}
#endif //JUSTXM_CONFIG_H
