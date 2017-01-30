// Description: This application is an Addon Manager for World of Warcraft
// made primarily for GNU/Linux it utilizes scraping of the curse website
// to automate the process of downloading addons and keeping them up to date.
//
// Author: Jonas Knarbakk

#include <iostream>
#include "Connection.h"
#include "Addon.hpp"
#include "HTMLParser.hpp"

int main(){

    std::string website = "https://mods.curse.com/search?game-slug=wow&search=";
    std::string search = "recount";
    std::string finalSearch = website + search;
    Connection con;
    if(con.connect(finalSearch)){
        con.save_data_to_file("tempfile.html");
    }
    
    HTMLParser parse("tempfile.html");

    std::vector<std::string> result = parse.getAddonLinks();
    std::vector<std::string>::iterator it;

    for(it = begin(result); it != end(result); ++it){
        std::cout << *it << std::endl;
    }
    
    return 0;
}
