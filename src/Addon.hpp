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
        std::string m_TotalDownloads;
    public:
        // Default constructor
        Addon();
        // Constructor with defaults
        Addon(std::string name = "Not set",
                std::string version = "Not set",
                std::string supports = "Not set",
                std::string image = "Not set",
                std::string downloads = "Not set");
        ~Addon();

        // Returns the name of the addon
        std::string getName() const;
        // Returns the build version of the addon
        std::string getVersion() const;
        // Returns wich version of World of Warcraft the addon supports
        std::string getSupportedVersion() const;
        // Returns the link to the addons display image
        std::string getImageLink() const;
        // Returns the total number of downloads for the addon
        std::string getTotalDownloads() const;
        
        // Set the addon name
        void setName(std::string name);
        // Set the addon version
        void setVersion(std::string version);
        // Set the version of World of Warcraft the addons supports
        void setSupportedVersion(std::string supported);
        // Set the link for the image that showcase the addon
        void setImageLink(std::string url);
        // Set the number the addon has for total downloads
        void setTotaltDownloads(std::string downloads);
        
        friend std::ostream &operator <<(std::ostream &output, const Addon &addon);
};

#endif
