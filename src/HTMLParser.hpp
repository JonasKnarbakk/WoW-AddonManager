#ifndef HTMLPARSER_HPP
#define HTMLPARSER_HPP

#include <fstream>
#include <string>
#include <vector>
#include "Addon.hpp"

class HTMLParser{
    private:
        std::ifstream m_File;
        std::string m_Addons;
        std::vector<std::string> m_AddonLinks;

        bool checkForContent(std::string line, std::string content);
        std::string getAddonLink(std::string line);
        std::string getAddonName(std::string line);
        std::string getAddonSupports(std::string line);
        std::string getAddonVersion(std::string line);
        std::string getAddonImage(std::string line);
        std::string getAddonTotalDownloads(std::string line);
    public:
        HTMLParser(std::string filename);
        ~HTMLParser();

        void init();
        void setFile(std::string file);
        std::vector<std::string> getAddonLinks() const;
        Addon getAddon();
};

#endif
