//
// Created by 肖应雄 on 2022/6/10.
//

#ifndef JUSTXM_UTIL_H
#define JUSTXM_UTIL_H

#include <iostream>
using namespace std;
#include <fstream>
#include <execinfo.h>
#include <sys/time.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
namespace xm {


    class FSUtil {
    public:
        static bool OpenForWrite(ofstream &ofs, const string filename, ios_base::open_mode mode);

        static string Dirname(const string &filename);

        static bool Mkdir(const string &dirname);
    };

}
#endif //JUSTXM_UTIL_H
