// Description: This application is an Addon Manager for World of Warcraft
// made primarily for GNU/Linux it utilizes scraping of the curse website
// to automate the process of downloading addons and keeping them up to date.
//
// Author: Jonas Knarbakk

#include <iostream>
#include <thread>
#include "Connection.h"
#include "Addon.hpp"
#include "HTMLParser.hpp"

std::vector<std::string> filesCreated;

void downloadHTML(std::string url, unsigned int count){
    Connection conn;
    std::string filename = "addon" + std::to_string(count) + ".html";
    if(conn.connect(url)){
        if(conn.save_data_to_file(filename)){
            filesCreated.push_back(filename);
        }
    }
}

int main(){

    std::string website = "https://mods.curse.com/search?game-slug=wow&search=";
    std::string search = "recount";
    std::string finalSearch = website + search;
    Connection con;
    if(con.connect(finalSearch)){
        if(con.save_data_to_file("searchResults.html")){
            filesCreated.push_back("searchResults.html");
        }
    }
    
    HTMLParser parse("searchResults.html");

    // Do a multithreaded download for the HTML for all addons found
    std::vector<std::string> result = parse.getAddonLinks();
    std::vector<std::string>::iterator it;

    std::vector<std::thread> threads;
    std::vector<std::thread>::iterator tit;

    int count = 1;

    for(it = begin(result); it != end(result); ++it){
        threads.push_back(std::thread(downloadHTML, *it, count));
        count++;
    }

    // Wait for all started threads to finish
    for(tit = begin(threads); tit != end(threads); ++tit){
        tit->join();
    }

    // Cleanup, delete all files created
    for(it = begin(filesCreated); it != end(filesCreated); ++it){
        std::string filename = *it;
        remove(filename.c_str());
    }
    
    return 0;
}
