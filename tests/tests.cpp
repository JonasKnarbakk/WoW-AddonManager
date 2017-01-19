#include <iostream>
#include "../src/Connection.h"

void connection_test(std::string url){
    Connection con;
    if(con.connect(url)){
        std::cout << "\033[1;32mConnection to " << url << " PASSED\033[0m\n" << std::endl;
    } else {
        std::cout << "\033[1;31mConnection to " << url << " FAILED\033[0m\n" << std::endl;
    }
}

void download_test(std::string url){
    Connection con;
    if(con.connect(url)){
        if(con.save_data_to_file("tempfile.zip")){
            std::cout << "\033[1;32mDownloading " << url << " PASSED\033[0m\n" << std::endl;
            remove("tempfile.zip");
        } else {
            std::cout << "\033[1;31mDownloading " << url << " FAILED\033[0m\n" << std::endl;
        }
    } else {
        std::cout << "\033[1;31mConnecting to download " << url << " FAILED\033[0m\n" << std::endl;
    }
}

int main(){
    connection_test("http://www.vg.no/");
    connection_test("https://mods.curse.com/addons/wow");
    download_test("http://addons.curse.cursecdn.com/files/2366/614/DBM-Core-7.1.7.zip");
    download_test("http://addons.curse.cursecdn.com/files/2364/379/Recount-v7.1.0h release.zip");
}
