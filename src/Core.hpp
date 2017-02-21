#ifndef CORE_HPP
#define CORE_HPP

#include <string>
#include <vector>
#include "GUI.hpp"
#include "Addon.hpp"

class Core{
    private:
        static std::string m_InstallPath;
    public:
        Core();
        ~Core();


        static void setInstallPath(std::string path);
        static std::string getInstallPath();

        static void checkSettings();
        static void downloadHTML(std::vector<std::string> *list, std::string url, unsigned int count);
        static std::vector<Addon> search(std::string search);
        static std::vector<Addon> list();
        static bool sortByNames(const Addon &a1, const Addon &a2);
        static bool sortByDownloads(const Addon &a1, const Addon &a2);

        static void searchGUI(std::string search);
};

#endif
