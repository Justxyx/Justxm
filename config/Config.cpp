//
// Created by 肖应雄 on 2022/6/14.
//

#include "Config.h"
namespace xm {
    xm::ConfigVArBase::ConfigVArBase(const string &mName,
                                     const string &mDescription) : m_name(mName),
                                                                   m_description(mDescription) {}

    const string &xm::ConfigVArBase::getMName() const {
        return m_name;
    }

    const string &xm::ConfigVArBase::getMDescription() const {
        return m_description;
    }

    ConfigVArBase::ptr Config::LookUpBase(const string &name) {
        auto it = GetDatas().find(name);
        return it == GetDatas().end() ? nullptr : it->second;
    }

    static void ListAllMember(const string &prefix,
                              const YAML::Node &node,
                              list<pair<string,const YAML::Node>> &output){
        output.push_back(make_pair(prefix,node));
        if (node.IsMap()){
            for (auto i = node.begin(); i != node.end(); ++i){
                ListAllMember(prefix.empty() ? i->first.Scalar() : prefix + "." + i->first.Scalar(),
                                i->second,
                                output);
            }
        }
    }

    void Config::LoadFromYaml(const YAML::Node &root) {
        list<pair<string,const YAML::Node>> all_nodes;
        ListAllMember("",root,all_nodes);
        for (const auto &item : all_nodes) {
            string key = item.first;
            if (key.empty())
                continue;
            ConfigVArBase::ptr var = Config::LookUpBase(key);
            if (var){
                if (item.second.IsScalar()) {
                    var->fromString(item.second.Scalar());
                } else {
                    stringstream  ss;
                    ss << item.second;
                    var->fromString(ss.str());
                }
            }
        }
    }
}