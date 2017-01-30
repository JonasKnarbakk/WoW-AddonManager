// Description: This class will parse a html file
// for relevant information about addons
// Created by Jonas Knarbakk (06:02:31-30/01/2017)

#include <sstream>
#include <iostream>
#include "HTMLParser.hpp"

HTMLParser::HTMLParser(std::string filename){
    m_File.open(filename);
}

HTMLParser::~HTMLParser(){
}

void HTMLParser::init(){
    std::string currentLine;
    bool parse = false;
    bool done = false;

    while(std::getline(m_File, currentLine)){
        if(!parse && !done){
            if(currentLine.find("<tbody>") != std::string::npos) parse = true;
        } else if(parse && !done){
            if(currentLine.find("</tbody>") != std::string::npos) done = true;
            if(checkForContent(currentLine, "/addons/wow/")){
                m_AddonLinks.push_back(getAddonLink(currentLine));
            }
        }
    }
}

bool HTMLParser::checkForContent(std::string line, std::string content){
    if(line.find(content) != std::string::npos){
        return true;
    }
    return false;
}

std::string HTMLParser::getAddonLink(std::string line){
    std::string link;
    std::size_t start = line.find("wow/") + 4;
    std::size_t end = line.find("\">");
    std::size_t wordSize = end - start;
    link = "https://mods.curse.com/addons/wow/" + line.substr(start, wordSize);

    return link;
}

std::string HTMLParser::getAddonName(std::string line){
    std::string name;
    std::size_t start = line.find("<H2 >") + 5;
    std::size_t end = line.find("</H2>");
    std::size_t wordSize = end - start;
    name = line.substr(start, wordSize);

    return name;
}

std::string HTMLParser::getAddonSupports(std::string line){
    std::string supports;
    std::size_t start = line.find("Supports: ") + 10;
    std::size_t end = line.find("</li>");
    std::size_t wordSize = end - start;
    supports = line.substr(start, wordSize);

    return supports;
}

std::string HTMLParser::getAddonVersion(std::string line){
    std::string version;
    std::size_t start = line.find("File: ") + 6;
    std::size_t end = line.find("</li>");
    std::size_t wordSize = end - start;
    version = line.substr(start, wordSize);

    return version;
}

std::string HTMLParser::getAddonImage(std::string line){
    std::string image;
    std::size_t start = line.find("src=\"") + 5;
    std::size_t end = line.find_last_of(".jpeg") + 1;
    std::size_t wordSize = end - start;
    image = line.substr(start, wordSize);

    return image;
}

std::string HTMLParser::getAddonTotalDownloads(std::string line){
    std::string downloads;
    std::size_t start = line.find("downloads\">") + 11;
    std::size_t end = line.find(" Total");
    std::size_t wordSize = end - start;
    downloads = line.substr(start, wordSize);

    return downloads;
}

std::vector<std::string> HTMLParser::getAddonLinks() const{
    return m_AddonLinks;
}

Addon HTMLParser::getAddon(){
    Addon addon("placeholder");
    std::string currentLine;
    bool parse = false;
    bool done = false;

    while(std::getline(m_File, currentLine)){
        if(!parse && !done){
            if(currentLine.find("project-overview") != std::string::npos) parse = true;
        } else if(parse && !done){
            if(currentLine.find("primary-content") != std::string::npos) done = true;
            if(checkForContent(currentLine, "<H2 >")){
                addon.setName(getAddonName(currentLine));
            } else if(checkForContent(currentLine, "Supports:")){
                addon.setSupportedVersion(getAddonSupports(currentLine));
            } else if(checkForContent(currentLine, "File:")){
                addon.setVersion(getAddonVersion(currentLine));
            } else if(checkForContent(currentLine, "primary-project-attachment")){
                addon.setImageLink(getAddonImage(currentLine));
            } else if(checkForContent(currentLine, "Total Downloads")){
                addon.setTotaltDownloads(getAddonTotalDownloads(currentLine));
            }
        }
    }   

    return addon;
}

void HTMLParser::setFile(std::string filename){
    m_File.open(filename);
}
