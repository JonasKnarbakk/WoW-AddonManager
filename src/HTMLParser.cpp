// Description: This class will parse a html file
// for relevant information about addons
// Created by Jonas Knarbakk (06:02:31-30/01/2017)

#include <sstream>
#include <iostream>
#include <algorithm>
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
			if(currentLine.find("class=\"tabbed-container\"") != std::string::npos) parse = true;
		} else if(parse && !done){
			if(currentLine.find("class=\"listing-footer\"") != std::string::npos) done = true;
			if(checkForContent(currentLine, "list-item__details")){
				std::getline(m_File, currentLine);
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
	std::size_t start = line.find("href") + 6;
	std::size_t end = line.find("\">");
	std::size_t wordSize = end - start;
	link = "https://www.curseforge.com" + line.substr(start, wordSize);

	std::cout << "Addon link: " << link << std::endl;

	return link;
}

std::string HTMLParser::getAttribute(std::string line, std::string attribute){
	std::string data;
	std::size_t start = line.find(attribute) + attribute.size() + 2;
	data = line.substr(start);
	std::size_t end = data.find("\"");
	// std::size_t wordSize = end - start;
	return data.substr(0, end);
}

std::string HTMLParser::getAddonName(std::string line){
	std::string name;
	std::size_t start = line.find(">") + 1;
	std::size_t end = line.find("</");
	std::size_t wordSize = end - start;
	name = line.substr(start, wordSize);

	return name;
}

std::string HTMLParser::getAddonSupports(std::string line){
	std::string supports;
	std::size_t start = line.find("Game Version: ") + 14;
	std::size_t end = line.find("</span>");
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

unsigned int HTMLParser::getAddonTotalDownloads(std::string line){
	std::string downloads;
	std::size_t start = line.find(">") + 1;
	std::size_t end = line.find("</span>");
	std::size_t wordSize = end - start;
	downloads = line.substr(start, wordSize);
	downloads.erase(std::remove(downloads.begin(), downloads.end(), ','), downloads.end());
	unsigned int count = std::stoi(downloads);

	return count;
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
			if(currentLine.find("project-details__main") != std::string::npos) parse = true;
		} else if(parse && !done){
			if(currentLine.find("projcet-details__sidebar") != std::string::npos) done = true;
			if(checkForContent(currentLine, "class=\"name\"")){
				addon.setName(getAddonName(currentLine));
				addon.setDownloadLink(getAddonName(currentLine) + "/download");
			} else if(checkForContent(currentLine, "Game Version:")){
				addon.setSupportedVersion(getAddonSupports(currentLine));
			} else if(checkForContent(currentLine, "Updated:")){
				// TODO: They no long show version number on addon page
				// without doing another GET request, find a way
				// to get this without having to load another page
				addon.setVersion("1"/*getAddonVersion(currentLine)*/);
			} else if(checkForContent(currentLine, "img class=\"aspect_fill\"")){
				addon.setImageLink(getAttribute(currentLine, "src"));
			} else if(checkForContent(currentLine, "Total Downloads:")){
				// Jump one line forward as the actual download count is on the next line
				std::getline(m_File, currentLine);
				addon.setTotaltDownloads(getAddonTotalDownloads(currentLine));
			}
		}
	}

	return addon;
}

void HTMLParser::setFile(std::string filename){
	m_File.open(filename);
}

std::string HTMLParser::getDownloadLink(){
	std::string currentLine;
	bool parse = false;
	bool done = false;

	while(std::getline(m_File, currentLine)){
		if(!parse && !done){
			if(currentLine.find("countdown") != std::string::npos) parse = true;
		} else if(parse && !done){
			if(checkForContent(currentLine, "download-link")){
				return getAttribute(currentLine, "data-href");
			}
		}
	}
	return "";
}
