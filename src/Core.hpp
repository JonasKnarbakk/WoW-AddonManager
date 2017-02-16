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
};

#endif
