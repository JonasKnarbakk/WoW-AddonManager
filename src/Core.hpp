#ifndef CORE_HPP
#define CORE_HPP

#include <string>
#include <vector>
#include "GUI.hpp"
#include "Addon.hpp"

class Core{
    private:
    public:
        Core();
        ~Core();

        static std::vector<Addon> search(std::string search);
        static void searchGUI(std::string search);
        static bool sortByNames(const Addon &a1, const Addon &a2);
        static bool sortByDownloads(const Addon &a1, const Addon &a2);
};

#endif
