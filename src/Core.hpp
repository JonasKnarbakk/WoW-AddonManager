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
		// Sets the install path for addons
		static void setInstallPath(std::string path);

		// Returns the full installation path for addons
		static std::string getInstallPath();

		// Checks if we have a valid connection to the internet
		static bool checkConnection();

		// Loads settings, if they exists
		static void checkSettings();

		// Donwloads curses datebase file
		static void updateDatabase();

		// Downloads the html for a url and saves it to a file
		[[deprecated("Parsing html shouldn't be needed as long as"
				" we have access to curses database file")]]
		static void downloadHTML(std::vector<std::string> *list,
					std::string url,
					unsigned int count);

		// Search curses database for an addon, the search is case
		// insensetive.
		static std::vector<Addon> search(std::string search);

		// Search curses database for an addon
		static std::vector<Addon> search(std::string search,
						bool caseSensitive);

		// Returns a vector if installed addons
		static std::vector<Addon> list();

		// Downloads and unzips an addon maching the passed name
		// to the set addon folder.
		static void install(std::string addon);

		// Recursively removes the folders in the vector
		static void removeAddon(std::vector<std::string> modules);

		// Comparison operator to get a vector to sort by name
		// in DESC order
		static bool sortByNames(const Addon &a1, const Addon &a2);

		// Comparison operator to get a vector to sort by downloads
		// in DESC order
		static bool sortByDownloads(const Addon &a1, const Addon &a2);

		// Extract a zip file to the same directory then deletes the
		// zip file.
		static void extractZipArchive(std::string filepath);

		// Attempts to index installed addons
		// Retuns: a vector containing Addon objects that are installed
		static std::vector<Addon> indexInstalled();
	private:
		static Curse curse;
		static Logger logger;
		static std::string m_InstallPath;
		static std::vector<Addon> m_Installed;

		// Goes through a directory path and adds all folder names
		// to the passed vector.
		static void findAddons(const boost::filesystem::path &dirPath,
					std::vector<std::string> &addons);

		// Recursively searches a directory for files that end in .toc
		// the full file path string is added to the passed vector
		static void findTocFiles(const boost::filesystem::path &dirPath,
			std::vector<std::string> &tocFiles);

		// Gets the version string from a .toc file
		// Returns: version string
		static std::string extractVersionFromTocFile(
						std::string tocFilePath);

		// Attempts to match a .toc file with a addon name,
		// then parses the version string
		static void setCorrectVersion(
					std::vector<std::string> &tocFiles,
					Addon &addon);
};

#endif
