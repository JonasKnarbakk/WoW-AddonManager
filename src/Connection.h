#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <curl/curl.h>

class Connection{
    private:
        CURL *curl;
    public:
        Connection();
        ~Connection();
        bool connect(std::string url);
        bool save_data_to_file(std::string filename);
};

#endif
