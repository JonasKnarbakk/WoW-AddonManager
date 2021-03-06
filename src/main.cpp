// Description: This application is an Addon Manager for World of Warcraft
// made primarily for GNU/Linux it utilizes scraping of the curse website
// to automate the process of downloading addons and keeping them up to date.
//
// Author: Jonas Knarbakk

#include "GUI.hpp"
#include <string>
#include <sstream>
#include <vector>
#include "Addon.hpp"
#include "Core.hpp"
#include <algorithm>
#include "WoW-Addon-Manager.hpp"

int main(int argc, char * argv[]){

	// Print out project name and version
	std::stringstream project_string;
	project_string << PROJECT_NAME << " Version: " << PROJECT_VERSION_MAJOR << "."
		<< PROJECT_VERSION_MINOR << "." << PROJECT_VERSION_HOTFIX << std::endl;
	std::cout << project_string.str();

	if(argc > 1){
		Core::checkSettings();
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
				searchResults = Core::search(searchQuery);
				std::vector<Addon>::iterator it;
				std::sort(searchResults.begin(), searchResults.end(), Core::sortByDownloads);
				for(it = searchResults.begin(); it != searchResults.end(); ++it){
					std::cout << *it << "\n" << std::endl;
				}
			} else if (arg.compare("list") == 0){
				Core::list();
			} else if (arg.compare("set-path") == 0){
				if(argc >= 2){
					std::string path = argv[2];
					Core::setInstallPath(path);
					std::cout << Core::getInstallPath() << std::endl;
				}
			} else if (arg.compare("install") == 0){
				std::cout << Core::getInstallPath() << std::endl;
				std::string addon;
				for(int i = 2; i < argc; i++){
					std::string arg = argv[i];
					addon += arg;
					if(i < argc - 1){
						addon += "-";
					}
				}
				Core::install(addon);
			}
		}
	}

	return 0;
}
