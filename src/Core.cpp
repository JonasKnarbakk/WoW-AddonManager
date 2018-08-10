// Description: New Class
// Created by Jonas Knarbakk (13:54:04-07/02/2017)

#include "Core.hpp"
#include <algorithm>
#include <vector>
#include <thread>
#include <dirent.h>
#include "Connection.h"
#include "HTMLParser.hpp"
#include <sstream>
#include <zip.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <boost/algorithm/string/predicate.hpp>

Curse Core::curse;
Logger Core::logger;
std::string Core::m_InstallPath;

void Core::setInstallPath(std::string path){
	Core::m_InstallPath = path;

	FILE * fileptr = fopen("settings.txt", "w");

	if(fileptr){
		std::string setting = "PATH=" + Core::m_InstallPath + "\n";
		fputs(setting.c_str(), fileptr);
		fclose(fileptr);
	} else {
		printf("Failed to write to file %s\n", "settings.txt");
	}
}

std::string Core::getInstallPath(){
	return Core::m_InstallPath;
}

void Core::checkSettings(){
	FILE *fileptr = fopen("settings.txt", "r");

	if(fileptr){
		std::string line = "";
		char c;
		while((c = fgetc(fileptr)) != EOF){
			if(c == '\n'){
				if(line.find("PATH=") != std::string::npos){
					printf("Setting path to: %s\n",
							line.substr(5).c_str());
					Core::setInstallPath(line.substr(5));
				}
				line = "";
			} else {
				line += c;
			}
		}
		fclose(fileptr);
	} else {
		printf("Faile to read file %s\n", "settings.txt");
	}
}

void Core::updateDatabase() {
	curse.updateDatabase();
}

void Core::downloadHTML(std::vector<std::string> *list,
			std::string url,
			unsigned int count){
	Connection conn;
	std::string filename = "addon" + std::to_string(count) + ".html";
	if(conn.connect(url)){
		if(conn.save_data_to_file(filename)){
			list->push_back(filename);
		}
	}
}

std::vector<Addon> Core::search(std::string search){
	return curse.search(search);
}

std::vector<Addon> Core::search(std::string search, bool caseSensitive){
	return curse.search(search, caseSensitive);
}

std::vector<Addon> Core::list(){
	std::vector<Addon> list;
	DIR *dirptr = opendir(Core::m_InstallPath.c_str());
	dirent *dptr;

	if(dirptr){
		while((dptr = readdir(dirptr)) != nullptr){
			std::stringstream output;
			output << dptr;
			logger.log(output.str(), Logger::logLevel::INFO);
		}
	} else {
		std::stringstream errorMessage;
		errorMessage << "Could not open directory: %s\n",
				Core::m_InstallPath.c_str();
		logger.log(errorMessage.str(), Logger::logLevel::INFO);
	}
	closedir(dirptr);
	return list;
}

void Core::install(std::string addon){
	std::string downloadLink = "https://www.curseforge.com/wow/addons/" + addon + "/download";
	printf("download link: %s\n", downloadLink.c_str());
	Connection c;
	std::string filename = addon + ".html";
	if(c.connect(downloadLink)){
		c.save_data_to_file(filename);
	} else {
		printf("Failed to connect to %s\n", downloadLink.c_str());
	}
	HTMLParser p(addon + ".html");
	downloadLink = p.getDownloadLink();
	printf("download link: %s\n", downloadLink.c_str());
	if(c.connect(downloadLink)){
		c.save_data_to_file(Core::m_InstallPath + addon + ".zip");
	} else {
		printf("Failed to connect to: %s\n", downloadLink.c_str());
	}
	remove(filename.c_str());
}

bool Core::sortByNames(const Addon &a1, const Addon &a2){
	return a1.getName() < a2.getName();
}

bool Core::sortByDownloads(const Addon &a1, const Addon &a2){
	return a1.getTotalDownloads() > a2.getTotalDownloads();
}

// TODO: Refactor this nightmare ¯\_(ツ)_/¯
void Core::extractZipArchive(std::string filepath) {
	const char* archive;
	struct zip* za;
	struct zip_file* zf;
	struct zip_stat sb;
	char buff[100];
	int err;
	int i, len;
	int fd;
	long long sum;

	archive = filepath.c_str();
	if ((za = zip_open(archive, 0, &err)) == NULL) {
		zip_error_to_str(buff, sizeof(buff), err, errno);
		fprintf(stderr, "can't open zip archive %s : %s\n", archive, buff);
		return;
	}

	for (i = 0; i < zip_get_num_entries(za, 0); i++) {
		if (zip_stat_index(za, i, 0, &sb) == 0) {
			std::stringstream fileToCreate;
			std::string filepathCopy = filepath;
			std::string tmpFile;
			std::size_t found = filepath.find_last_of(boost::filesystem::path::preferred_separator);
			const std::string ext(filepath.substr(found+1));
			logger.log("Substring: " + filepath.substr(found+1), Logger::logLevel::INFO);
			if( filepathCopy != ext
				&& filepathCopy.size() > ext.size()
				&& filepathCopy.substr(filepathCopy.size() - ext.size()) == filepath.substr(found+1)) {
				fileToCreate << filepathCopy.substr(0, filepathCopy.size() - ext.size());
			}
			fileToCreate << sb.name;
			printf("==================\n");
			len = strlen(sb.name);
			printf("Name: [%s], ", sb.name);
			printf("Size: [%llu], ", sb.size);
			printf("mtime: [%u]\n", (unsigned int)sb.mtime);
			printf("File name: ", sb.name);


			std::vector<std::string> folders;
			std::string segment;
			while(std::getline(fileToCreate, segment, '/')) {
				folders.push_back(segment);
			}

			std::stringstream path;
			// Start in the root directory
			path << "/";
			for(auto& folder : folders) {
				if(!folder.empty()) {
					if(!boost::filesystem::exists(path.str())) {
						logger.log("Attempting to create file: " + path.str(), Logger::logLevel::INFO);
						boost::filesystem::create_directory(path.str());
					}
					path << "/" << folder;
				}
			}
			if (sb.name[len - 1] == '/') {
				printf("Creating main directory");
				if (mkdir(fileToCreate.str().c_str(), 0700) < 0) {
					fprintf(stderr, "Failed to create folder %s\n", fileToCreate.str().c_str());
				}
			} else {
				zf = zip_fopen_index(za, i, 0);
				if (!zf) {
					fprintf(stderr, "Failed to open at index\n");
					return;
				}


				fd = open(fileToCreate.str().c_str(), O_RDWR | O_TRUNC | O_CREAT, 0666);
				if (fd < 0) {
					fprintf(stderr, "Failed to open %s ERROR: %d\n", fileToCreate.str().c_str(), fd);
					return;
				}

				sum = 0;

				while (sum != sb.size) {
					len = zip_fread(zf, buff, 100);
					if (len < 0) {
						fprintf(stderr, "Failed to read archive\n");
						return;
					}
					write(fd, buff, len);
					sum += len;
				}
				::close(fd);
				zip_fclose(zf);
			}
		} else {
			printf("File[%s] Line[%d]\n", __FILE__, __LINE__);
		}
	}

	if (zip_close(za) == -1) {
		fprintf(stderr, "can't close zip archive %s\n", archive);
		return;
	}

	// Clean up the tmp file
	logger.log("Filepath value: " + filepath, Logger::logLevel::INFO);
	if(boost::filesystem::exists(filepath)) {
		logger.log("Deleting file:" + filepath, Logger::logLevel::INFO);
		boost::filesystem::remove(filepath);
	}
}

std::vector<Addon> Core::indexInstalled() {
	namespace bf = boost::filesystem;

	bf::path addonDir(m_InstallPath);

	if(!bf::exists(addonDir)) {
		std::vector<Addon> empty;
		return empty;
	}

	std::vector<std::string> tocFiles;

	findTocFiles(addonDir, tocFiles);

	for(auto& tocFile : tocFiles) {
		extractVersionFromTocFile(tocFile);
	}

	std::vector<std::string> installed;
	std::vector<Addon> addons;

	findAddons(addonDir, installed);

	for(auto& addon : installed) {
		std::vector<Addon> matches = curse.search(addon);
		if(!matches.empty()) {
			setCorrectVersion(tocFiles, matches.at(0));
			addons.push_back(matches.at(0));
		}
	}

	return addons;
}

void Core::findAddons( const boost::filesystem::path &dirPath,
			std::vector<std::string> &addons) {
	namespace bf = boost::filesystem;

	bf::directory_iterator endItr;
	for(bf::directory_iterator itr(dirPath);
		itr != endItr;
		++itr) {
		if(bf::is_directory(itr->status())) {
			addons.push_back(itr->path().filename().native());
		}
	}
}

void Core::findTocFiles( const boost::filesystem::path &dirPath,
			std::vector<std::string> &tocFiles) {
	namespace bf = boost::filesystem;

	bf::recursive_directory_iterator endItr;
	for(bf::recursive_directory_iterator itr(dirPath);
		itr != endItr;
		++itr) {
		if(bf::is_regular_file(itr->status())
		&& boost::algorithm::ends_with(itr->path().filename().string(), ".toc")) {
			tocFiles.push_back(itr->path().string());
		}
	}
}

std::string Core::extractVersionFromTocFile(std::string tocFilePath) {
	std::ifstream tocFile(tocFilePath);

	std::string line;
	while (std::getline(tocFile, line)) {
		std::istringstream iss(line);
		if(!(iss)) {
			break; // Error
		}
		if(line.find("## Version:") != std::string::npos) {
			return line.substr(11, line.length());
		}
	}
	return "";
}

bool Core::checkConnection() {
	if(system("ping -c3 8.8.8.8 > /dev/null") == 0) {
		return true;
	} else if (system("ping -c3 208.67.222.222 > /dev/null") == 0) {
		return true;
	} else {
		return false;
	}
}


void Core::setCorrectVersion(std::vector<std::string> &tocFiles, Addon &addon) {
	bool versionSet = false;
	for(auto& tocFile : tocFiles) {
		std::size_t start = tocFile.find_last_of("/")+1;
		std::string addonName = tocFile.substr(start, tocFile.length());
		addonName = addonName.substr(0, addonName.length()-4);
		if(addon.getName().find(addonName) != std::string::npos) {
			std::string version = extractVersionFromTocFile(tocFile);
			if(!version.empty()) {
				addon.setVersion(version);
				versionSet = true;
			}
		}
	}
	if(!versionSet) {
		std::cout << "Warning!: Coudn't determine addon version for "
			<< addon.getName() << std::endl;
	}
}
