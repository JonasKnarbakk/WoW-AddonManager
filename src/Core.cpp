// Description: New Class
// Created by Jonas Knarbakk (13:54:04-07/02/2017)

#include "Core.hpp"
#include <algorithm>
#include <vector>
#include <thread>
#include "Connection.h"
#include "HTMLParser.hpp"

Core::Core(){
}

Core::~Core(){
}

std::vector<std::string> filesCreated;
std::vector<Addon> addons;

void downloadHTML(std::string url, unsigned int count){
    Connection conn;
    std::string filename = "addon" + std::to_string(count) + ".html";
    if(conn.connect(url)){
        if(conn.save_data_to_file(filename)){
            filesCreated.push_back(filename);
        }
    }
}

void search(){
    std::string search = "Recount";
    std::string website = "https://mods.curse.com/search?game-slug=wow&search=";
    std::transform(search.begin(), search.end(), search.begin(), [](char ch) {
        return ch == ' ' ? '+' : ch;
    });
    std::string finalSearch = website + search;
    Connection con;
    if(con.connect(finalSearch)){
        con.save_data_to_file("searchResults.html");
    }
    
    HTMLParser parse("searchResults.html");
    parse.init();

    remove("searchResults.html");

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
        HTMLParser p(filename);
        addons.push_back(p.getAddon());
        remove(filename.c_str());
    }

    std::vector<Addon>::iterator ait;
    // Print search result with details
    for(ait = begin(addons); ait != end(addons); ++ait){
        GUI::addAddon(ait->getName(), ait->getVersion(), ait->getSupportedVersion(), ait->getImageLink(), ait->getTotalDownloads());
    }
}