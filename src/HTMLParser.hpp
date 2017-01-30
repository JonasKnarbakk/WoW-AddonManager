#ifndef HTMLPARSER_HPP
#define HTMLPARSER_HPP

#include <fstream>
#include <string>
#include <vector>

class HTMLParser{
    private:
        std::ifstream m_File;
        std::string m_Addons;
        std::vector<std::string> m_AddonLinks;

        std::string Init();
        bool checkForContent(std::string line, std::string content);
        std::string getAddonLink(std::string line);
    public:
        HTMLParser(std::string filename);
        ~HTMLParser();

        std::vector<std::string> getAddonLinks() const;
};

#endif
