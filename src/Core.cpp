// Description: New Class
// Created by Jonas Knarbakk (13:54:04-07/02/2017)

#include "Core.hpp"
#include <algorithm>
#include <vector>
#include <thread>
#include <dirent.h>
#include "Connection.h"
#include "HTMLParser.hpp"

Core::Core(){
}

Core::~Core(){
}

std::string Core::m_InstallPath;

void Core::setInstallPath(std::string path){
    Core::m_InstallPath = path;

    FILE * fileptr = fopen("settings.txt", "w");
    
    if(fileptr){
        std::string setting = "PATH=" + Core::m_InstallPath + "\n";
        fputs(setting.c_str(), fileptr);
        fclose(fileptr);
    } else {
        printf("Failed to write to file %s\n", "settings.txt");
    }
}

std::string Core::getInstallPath(){
    return Core::m_InstallPath;
}

void Core::checkSettings(){
    FILE *fileptr = fopen("settings.txt", "r");

    if(fileptr){
        std::string line = "";
        char c;
        while((c = fgetc(fileptr)) != EOF){
            if(c == '\n'){
                if(line.find("PATH=") != std::string::npos){
                    printf("Setting path to: %s\n", line.substr(5).c_str());
                    Core::setInstallPath(line.substr(5));
                }
                line = "";
            } else {
                line += c;
            }
        }
        fclose(fileptr);
    } else {
        printf("Faile to read file %s\n", "settings.txt");
    }
}

void Core::downloadHTML(std::vector<std::string> *list, std::string url, unsigned int count){
    Connection conn;
    std::string filename = "addon" + std::to_string(count) + ".html";
    if(conn.connect(url)){
        if(conn.save_data_to_file(filename)){
            list->push_back(filename);
        }
    }
}

std::vector<Addon> Core::search(std::string search){

    std::vector<Addon> addons;
    std::vector<std::string> filesCreated;

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
        threads.push_back(std::thread(downloadHTML, &filesCreated, *it, count));
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

    return addons;
}

std::vector<Addon> Core::list(){
    std::vector<Addon> list;
    DIR *dirptr = opendir(Core::m_InstallPath.c_str());
    dirent *dptr;

    if(dirptr){
        while((dptr = readdir(dirptr)) != nullptr){
            std::cout << dptr << std::endl; 
        }
    } else {
        printf("Could not open directory: %s\n", Core::m_InstallPath.c_str());
    }
    closedir(dirptr);
    return list;
}

void Core::install(std::string addon){
    std::string downloadLink = "https://mods.curse.com/addons/wow/" + addon + "/download";
    printf("download link: %s\n", downloadLink.c_str());
    Connection c;
    std::string filename = addon + ".html";
    if(c.connect(downloadLink)){
        c.save_data_to_file(filename);
    } else {
        printf("Failed to connect to %s\n", downloadLink.c_str());
    }
    HTMLParser p(addon + ".html");
    downloadLink = p.getDownloadLink();
    printf("download link: %s\n", downloadLink.c_str());
    if(c.connect(downloadLink)){
        c.save_data_to_file(Core::m_InstallPath + addon + ".zip");
    } else {
        printf("Failed to connect to: %s\n", downloadLink.c_str());
    }
    remove(filename.c_str());
}

void Core::searchGUI(std::string searchQuery){
    GUI::working(true);
    std::vector<Addon> addons = Core::search(searchQuery);
    std::sort(addons.begin(), addons.end(), Core::sortByDownloads);
    std::vector<Addon>::iterator it;
    // Print search result with details
    int imageCount = 1;
    for(it = begin(addons); it != end(addons); ++it){
        Connection conn;
        if(conn.connect(it->getImageLink())){
            conn.save_data_to_file("thumbnail" + std::to_string(imageCount));
        }
        std::string imagePath = "thumbnail" + std::to_string(imageCount);
        GUI::addAddon(it->getName(), it->getVersion(), it->getSupportedVersion(), imagePath, it->getTotalDownloadsFormat(","));
        imageCount++;
    }

    for(int i = 1; i <= 10; i++){
        std::string filename = "thumbnail" + std::to_string(i);
        remove(filename.c_str());
    }

    GUI::working(false);
}

bool Core::sortByNames(const Addon &a1, const Addon &a2){
    return a1.getName() < a2.getName();
}

bool Core::sortByDownloads(const Addon &a1, const Addon &a2){
    return a1.getTotalDownloads() > a2.getTotalDownloads();
}
