// Description: This application is an Addon Manager for World of Warcraft
// made primarily for GNU/Linux it utilizes scraping of the curse website
// to automate the process of downloading addons and keeping them up to date.
//
// Author: Jonas Knarbakk

#include <iostream>
#include "Connection.h"
#include "Addon.hpp"

int main(){

    // Connection con;
    // if(con.connect("https://mods.curse.com/addons/wow")){
        // con.save_html_to_file("tempfile.html");
    // }
    
    Addon a("Recout");
    
    std::cout << a << std::endl;
    
    return 0;

}
