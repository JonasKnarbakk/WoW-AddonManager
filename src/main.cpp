// Description: New project
// Author: Jonas Knarbakk ()

#include <cstdio>
#include "Connection.h"

int main( int argc, char ** argv ){

    puts("Hello World!");
    Connection con;
    // if(con.connect("https://mods.curse.com/addons/wow")){
        // con.save_html_to_file("tempfile.html");
    // }
    if(con.connect("https://addons-origin.cursecdn.com/files/2364/379/Recount-v7.1.0h%20release.zip")){
        con.save_data_to_file("recount.zip");
    }
    return 0;

}
