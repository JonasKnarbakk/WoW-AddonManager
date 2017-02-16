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

        std::vector<Addon> search(std::string search);
};

#endif
