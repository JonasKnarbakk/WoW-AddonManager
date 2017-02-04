#include <iostream>
#include "../src/Connection.h"
#include "../src/HTMLParser.hpp"
#include "ReturnValue.hpp"

void printRed(std::string message);
void printGreen(std::string message);

void connection_test(std::string url){
    Connection con;
    if(con.connect(url)){
        printGreen("Connection to " + url + " Passed!");
    } else {
        printRed("Connection to " + url + " FAILED!");
    }
}

void download_test(std::string url){
    Connection con;
    if(con.connect(url)){
        if(con.save_data_to_file("tempfile.zip")){
            if(remove("tempfile.zip") == 0){
                printGreen("Downloaded " + url + " Successfully!");
            }
        } else {
            printRed("Downloading of " + url + " FAILED!");
        }
    } else {
        printRed("Connecting too " + url + " when trying to download FAILED!");
    }
}

void parsing_test(){
    Connection con;
    if(con.connect("https://mods.curse.com/addons/wow/recount")){
        con.save_data_to_file("parsingTest.html");
        HTMLParser p("parsingTest.html");
        Addon recount = p.getAddon();
        if(recount.getName() == "Recount"){
            printGreen("Parsed addon name successfully!");
        } else {
            printRed("FAILED to parse addon name!");
        }
        if(recount.getVersion() == "v7.1.5a release" ){
            printGreen("Parsed addon version successfully");
        } else {
            printRed("FAILED to parse addon version!\nCheck if it has changed before assuming the parser to be broken.");
        }
        if(recount.getSupportedVersion() == "7.1.5"){
            printGreen("Parsed addon supported version successfully!");
        } else {
            printRed("FAILED to parse addon supported version!\nCheck if it has changed before assuming the parser to be broken.");
        }
        if(recount.getImageLink().find("https") == 0){
            printGreen("Parsed addon image link successfully!");
        } else {
            printRed("FAILED to parse addon image link!");
        }
        if(recount.getTotalDownloads().find("94,") == 0){
            printGreen("Parsed addon total downalods successfully!");
        } else {
            printRed("FAILED to parse addon total downloads!");
        }
        remove("parsingTest.html");
    } else {
        printRed("Parsing Test: Failed to connect to https://mods.curse.com/addons/wow/recount");
    }
}

int main(){
    connection_test("http://www.vg.no/");
    connection_test("https://mods.curse.com/addons/wow");
    download_test("http://addons.curse.cursecdn.com/files/2366/614/DBM-Core-7.1.7.zip");
    download_test("http://addons.curse.cursecdn.com/files/2364/379/Recount-v7.1.0h release.zip");
    parsing_test();
}

void printRed(std::string message){
    std::cout << "\033[1;31m" << message << "\033[0m\n" << std::endl;
}

void printGreen(std::string message){
    std::cout << "\033[1;32m" << message << "\033[0m\n" << std::endl;
}
