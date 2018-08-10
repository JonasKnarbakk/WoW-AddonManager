// Description: This addon class is for storing
// all relevant information about an Addon
// Created by Jonas Knarbakk (05:17:26-30/01/2017)

#include "Addon.hpp"

Addon::Addon(std::string name,
		std::string version,
		std::string supports,
		std::string image,
		std::string downloadLink,
		unsigned int downloads,
		std::vector<std::string> modules){
	m_Name = name;
	m_Version = version;
	m_Supports = supports;
	m_Image = image;
	m_DownloadLink = downloadLink;
	m_TotalDownloads = downloads;
	m_Modules = modules;
}

std::string Addon::getName() const {
	return m_Name;
}

std::string Addon::getVersion() const {
	return m_Version;
}

std::string Addon::getSupportedVersion() const {
	return m_Supports;
}

std::string Addon::getImageLink() const {
	return m_Image;
}

std::string Addon::getDownloadLink() const {
	return m_DownloadLink;
}

unsigned int Addon::getTotalDownloads() const {
	return m_TotalDownloads;
}

std::vector<std::string> Addon::getModules() const {
	return m_Modules;
}

void Addon::setName(std::string name) {
	m_Name = name;
}

void Addon::setVersion(std::string version) {
	m_Version = version;
}

void Addon::setSupportedVersion(std::string supports) {
	m_Supports = supports;
}

void Addon::setImageLink(std::string url) {
	m_Image = url;
}

void Addon::setDownloadLink(std::string url) {
	m_DownloadLink = url;
}

void Addon::setTotaltDownloads(unsigned int downloads) {
	m_TotalDownloads = downloads;
}

void Addon::setModules(std::vector<std::string> modules) {
	m_Modules = modules;
}

std::string Addon::getTotalDownloadsFormat(std::string seperator) const {
	// Format downloads to have commas to make it more readable
	std::string downloads = std::to_string(m_TotalDownloads);
	int insertPosition = downloads.length()-3;
	while(insertPosition > 0){
		downloads.insert(insertPosition, seperator);
		insertPosition-=3;
	}
	return downloads;
}

std::ostream &operator <<(std::ostream &output, const Addon &addon) {
	output << "Name: " << addon.getName()
		<< "\nVersion: " << addon.getVersion()
		<< "\nSupports: " << addon.getSupportedVersion()
		<< "\nImage Link: " << addon.getImageLink()
		<< "\nTotal Downloads: " << addon.getTotalDownloadsFormat(",");

	return output;
}

bool operator <(const Addon &a1, const Addon &a2) {
	return a1.getTotalDownloads() < a2.getTotalDownloads();
}
