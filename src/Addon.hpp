#ifndef ADDON_HPP
#define ADDON_HPP

#include <string>
#include <iostream>

class Addon{
    private:
        std::string m_Name;
        std::string m_Version;
        std::string m_Supports;
        std::string m_Image;
        std::string m_DownloadLink;
        unsigned int m_TotalDownloads;
    public:
        // Default constructor
        // Addon();
        // Constructor with defaults
        Addon(std::string name = "Not set",
                std::string version = "Not set",
                std::string supports = "Not set",
                std::string image = "Not set",
                std::string downloadLink = "Not set",
                unsigned int downloads = 0);
        ~Addon();

        // Returns the name of the addon
        std::string getName() const;
        // Returns the build version of the addon
        std::string getVersion() const;
        // Returns wich version of World of Warcraft the addon supports
        std::string getSupportedVersion() const;
        // Returns the link to the addons display image
        std::string getImageLink() const;
        // Return the link to download the addons zip file
        std::string getDownloadLink() const;
        // Returns the total number of downloads for the addon
        unsigned int getTotalDownloads() const;
        std::string getTotalDownloadsFormat(std::string seperator) const;
        
        // Set the addon name
        void setName(std::string name);
        // Set the addon version
        void setVersion(std::string version);
        // Set the version of World of Warcraft the addons supports
        void setSupportedVersion(std::string supported);
        // Set the link for the image that showcase the addon
        void setImageLink(std::string url);
        // Set the link for the zip file download
        void setDownloadLink(std::string url);
        // Set the number the addon has for total downloads
        void setTotaltDownloads(unsigned int downloads);

        bool sortByNames(const Addon &a1, const Addon &a2);
        bool sortByDownloads(const Addon &a1, const Addon &a2);

        friend std::ostream &operator <<(std::ostream &output, const Addon &addon);
        friend bool operator <(const Addon &a1, const Addon &a2);

        
};

#endif
