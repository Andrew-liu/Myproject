#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <string>
#include <echo/NonCopyable.h>


class Config : private NonCopyable
{
    public:
        Config();
        ~Config();

        std::string return_file()
        {   return filename_; }
        std::string return_wcache()
        {   return wcache_; }
        std::string return_out()
        {   return out_; }
    private:
        std::string filename_;
        std::string wcache_;
        std::string out_;
};

#endif
