// Description: This addon class is for storing
// all relevant information about an Addon
// Created by Jonas Knarbakk (05:17:26-30/01/2017)

#include "Addon.hpp"

Addon::Addon(){
    m_Name = "";
    m_Version = "";
    m_Supports = "";
    m_Image = "";
    m_TotalDownloads = 0;
}

Addon::Addon(std::string name, std::string version, std::string supports, std::string image, unsigned int downloads){
    m_Name = name;
    m_Version = version;
    m_Supports = supports;
    m_Image = image;
    m_TotalDownloads = downloads;
}

Addon::~Addon(){
}

std::string Addon::getName() const{
    return m_Name;
}

std::string Addon::getVersion() const{
    return m_Version;
}

std::string Addon::getSupportedVersion() const{
    return m_Supports;
}

std::string Addon::getImageLink() const{
    return m_Image;
}

unsigned int Addon::getTotalDownloads() const{
    return m_TotalDownloads;
}

void Addon::setName(std::string name){
    m_Name = name;
}

void Addon::setVersion(std::string version){
    m_Version = version;
}

void Addon::setSupportedVersion(std::string supports){
    m_Supports = supports;
}

void Addon::setImageLink(std::string url){
    m_Image = url;
}

void Addon::setTotaltDownloads(unsigned int downloads){
    m_TotalDownloads = downloads;
}

std::ostream &operator <<(std::ostream &output, const Addon &addon){
    // Format downloads to have commas to make it more readable
    std::string downloads = std::to_string(addon.getTotalDownloads());
    int insertPosition = downloads.length()-3;
    while(insertPosition > 0){
        downloads.insert(insertPosition, ",");
        insertPosition-=3;
    }
    
    output << "Name: " << addon.getName()
        << "\nVersion: " << addon.getVersion()
        << "\nSupports: " << addon.getSupportedVersion()
        << "\nImage Link: " << addon.getImageLink()
        << "\nTotal Downloads: " << downloads;

    return output;
}

bool operator <(const Addon &a1, const Addon &a2){
    return a1.getTotalDownloads() < a2.getTotalDownloads();
}
