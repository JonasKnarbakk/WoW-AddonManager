// Description: This class will parse a html file
// for relevant information about addons
// Created by Jonas Knarbakk (06:02:31-30/01/2017)

#include <sstream>
#include <iostream>
#include "HTMLParser.hpp"

HTMLParser::HTMLParser(std::string filename){
    m_File.open(filename);
    Init();
}

HTMLParser::~HTMLParser(){
}

std::string HTMLParser::Init(){
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

    return "";
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

std::vector<std::string> HTMLParser::getAddonLinks() const{
    return m_AddonLinks;
}
