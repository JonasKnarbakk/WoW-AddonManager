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
#include <boost/filesystem.hpp>
#include <zip.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

Core::Core(){
}

Core::~Core(){
}

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
					printf("Setting path to: %s\n", line.substr(5).c_str());
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

void Core::downloadHTML(std::vector<std::string> *list, std::string url, unsigned int count){
	Connection conn;
	std::string filename = "addon" + std::to_string(count) + ".html";
	if(conn.connect(url)){
		if(conn.save_data_to_file(filename)){
			list->push_back(filename);
		}
	}
}

std::vector<Addon> Core::search(std::string search){

	std::vector<Addon> addons;
	std::vector<std::string> filesCreated;

	std::string website = "https://www.curseforge.com/wow/addons/search?search=";
	std::transform(search.begin(), search.end(), search.begin(), [](char ch) {
		return ch == ' ' ? '+' : ch;
	});
	std::string finalSearch = website + search;
	Connection con;
	if(con.connect(finalSearch)){
		con.save_data_to_file("searchResults.html");
	}
	
	HTMLParser parse("searchResults.html");
	parse.init();

	remove("searchResults.html");

	// Do a multithreaded download for the HTML for all addons found
	std::vector<std::string> result = parse.getAddonLinks();
	std::vector<std::string>::iterator it;

	std::vector<std::thread> threads;
	std::vector<std::thread>::iterator tit;

	int count = 1;

	for(it = begin(result); it != end(result); ++it){
		threads.push_back(std::thread(downloadHTML, &filesCreated, *it, count));
		count++;
	}

	// Wait for all started threads to finish
	for(tit = begin(threads); tit != end(threads); ++tit){
		tit->join();
	}
	// Cleanup, delete all files created
	for(it = begin(filesCreated); it != end(filesCreated); ++it){
		std::string filename = *it;
		HTMLParser p(filename);
		addons.push_back(p.getAddon());
		remove(filename.c_str());
	}

	return addons;
}

std::vector<Addon> Core::list(){
	std::vector<Addon> list;
	DIR *dirptr = opendir(Core::m_InstallPath.c_str());
	dirent *dptr;

	if(dirptr){
		while((dptr = readdir(dirptr)) != nullptr){
			std::cout << dptr << std::endl; 
		}
	} else {
		printf("Could not open directory: %s\n", Core::m_InstallPath.c_str());
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
			const std::string ext("tmp.zip");
			if( filepathCopy != ext
				&& filepathCopy.size() > ext.size()
				&& filepathCopy.substr(filepathCopy.size() - ext.size()) == "tmp.zip") {
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

	std::cout << "Filepath value: " << filepath << std::endl;
	if(boost::filesystem::exists(filepath)) {
		std::cout << "Deleting file: " << filepath << std::endl;
		boost::filesystem::remove(filepath);
	}
}
