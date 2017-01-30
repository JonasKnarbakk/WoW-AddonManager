// Description: New project
// Author: Jonas Knarbakk ()

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
