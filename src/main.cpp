// Description: This application is an Addon Manager for World of Warcraft
// made primarily for GNU/Linux it utilizes scraping of the curse website
// to automate the process of downloading addons and keeping them up to date.
//
// Author: Jonas Knarbakk

#include "GUI.hpp"
#include <string>
#include <vector>
#include "Addon.hpp"
#include "Core.hpp"
#include <algorithm>

bool sortByNames(const Addon &a1, const Addon &a2){
    return a1.getName() < a2.getName();
}

bool sortByDownloads(const Addon &a1, const Addon &a2){
    return a1.getTotalDownloads() > a2.getTotalDownloads();
}


int main(int argc, char * argv[]){
    
    if(argc > 0){
        std::string arg = argv[1];
        if(arg.compare("gui") == 0){
            GUI app(&argc, argv);    
        } else {
            std::string arg1 = argv[1];
            if(arg1.compare("search") == 0){
                std::vector<Addon>searchResults;
                std::string searchQuery;
                for(int i = 2; i < argc; i++){
                    std::string arg = argv[i];
                    searchQuery += arg + " ";
                }
                Core c;
                searchResults = c.search(searchQuery);
                std::vector<Addon>::iterator it;
                std::sort(searchResults.begin(), searchResults.end(), sortByDownloads);
                for(it = searchResults.begin(); it != searchResults.end(); ++it){
                    std::cout << *it << "\n" << std::endl;
                }
            }
        }
    }

    return 0;
}
