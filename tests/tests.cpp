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

int main(){
    connection_test("http://www.vg.no/");
    connection_test("https://mods.curse.com/addons/wow");
}
