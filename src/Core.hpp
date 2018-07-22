#ifndef CORE_HPP
#define CORE_HPP

#include <string>
#include <vector>
#include "Addon.hpp"
#include <boost/filesystem.hpp>
#include <addonsources/curse.hpp>
#include <Logger.hpp>

class Core{
	public:
		static void setInstallPath(std::string path);
		static std::string getInstallPath();

		static bool checkConnection();
		static void checkSettings();
		static void updateDatabase();
		static void downloadHTML(std::vector<std::string> *list, std::string url, unsigned int count);
		static std::vector<Addon> search(std::string search);
		static std::vector<Addon> search(std::string search, bool caseSensitive);
		static std::vector<Addon> list();
		static void install(std::string addon);
		static bool sortByNames(const Addon &a1, const Addon &a2);
		static bool sortByDownloads(const Addon &a1, const Addon &a2);
		static void extractZipArchive(std::string filepath);
		static std::vector<Addon> indexInstalled();
	private:
		static Curse curse;
		static Logger logger;
		static std::string m_InstallPath;
		static std::vector<Addon> m_Installed;
		static void findAddons( const boost::filesystem::path& dirPath,
					std::vector<std::string>& addons);
};

#endif
