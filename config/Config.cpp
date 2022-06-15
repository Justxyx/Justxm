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



}