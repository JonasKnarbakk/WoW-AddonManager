#include <addonsources/curse.hpp>
#include <vector>
#include <Addon.hpp>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/copy.hpp>
#include <sstream>

Curse::Curse() : AddonSource() {
}

Curse::~Curse() {

}

std::vector<Addon> Curse::search(std::string searchTerm) {
	return search(searchTerm, false);
}

std::vector<Addon> Curse::search(std::string searchTerm, bool caseSensetive) {
	// Any search with less than 3 characters
	// is trouble in the current implementation
	if(searchTerm.length() < 3){
		return {};
	}

	if(!caseSensetive) {
		boost::algorithm::to_lower(searchTerm);
	}

	std::vector<Addon> addons;

	// Search the json data
	for (nlohmann::json obj : jsonData["data"]) {
		std::string currentObject = obj["Name"].get<std::string>();
		if(!caseSensetive){
			boost::algorithm::to_lower(currentObject);
		}
		if(currentObject.find(searchTerm) != std::string::npos) {
			addons.push_back(parseAddonData(obj));
		}
	}

	return addons;
}

bool Curse::updateDatabase() {
	downloadArchive();
	decompressArchive();
	// Get the json data from file
	std::ifstream database("complete.json");
	jsonData = nlohmann::json::parse(database);
	return true;
}

void Curse::downloadArchive() {
	std::string url = "https://client.forgecdn.net/feed/addons/1/v10/complete.json.bz2";
	std::string filename = "complete.json.bz2";
	// Repace all spaces with %20 so the connection request doesn't fail
	for(std::size_t pos = url.find(" "); pos != std::string::npos; pos = url.find(" ")){
		url.replace(pos, 1, "%20");
	}

	CURL* curl = curl_easy_init();

	if(curl){

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		// tell libcurl to follow redirection
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		CURLcode res;
		printf("Attemptin to create file %s\n", filename.c_str());
		FILE *fileptr = fopen(filename.c_str(), "w");
		if(fileptr){

			// Write the html body data to file
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fileptr);
			res = curl_easy_perform(curl);

			if(res != CURLE_OK){
				// Print error
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
				// Cleanup
				curl_easy_cleanup(curl);
				fclose(fileptr);

				return;
			}
		} else {
			printf("Failed to save html to file.\n");
			return;
		}
		curl_easy_cleanup(curl);
		fclose(fileptr);
	}
}

void Curse::decompressArchive() {
	// Input
	try {
		std::ifstream datfile("complete.json.bz2", std::ios::in | std::ios::binary);
		if(datfile.good()) {
			boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
			in.push(boost::iostreams::bzip2_decompressor{});
			in.push(datfile);

			std::ofstream out;
			out.open("complete.json", std::ios::out | std::ios::trunc);
			boost::iostreams::copy(in, out);
			out.close();
		}
	} catch (const boost::iostreams::bzip2_error& exception) {
		int error = exception.error();

		if (error == boost::iostreams::bzip2::data_error) {
			// Compressed data stream is corrupted
			std::cout << "Compressed data stream is corrupted!" << std::endl;
		} else if (error == boost::iostreams::bzip2::data_error_magic) {
			// Compressed data does not begin with the magic sequence'B' 'Z' 'h'
			std::cout << "Compressed data does not begin with the magic sequence \'B\' \'Z\' \'h\'" << std::endl;
		} else if (error == boost::iostreams::bzip2::config_error) {
			// libzip2 has been improperly configure for the current platform
			std::cout << "libzip2 has been improperly configure for the current platform" << std::endl;
		} else if (error == boost::iostreams::bzip2::unexpected_eof) {
			// when the compressed file finishes before the logical end of stream is detected
			std::cout << "The compressed file finished before the logical end of stream was detected" << std::endl;
		} else {
			std::cout << error << ": Caught error of unkown type: " << exception.what() << std::endl;
		}
	}
}

Addon Curse::parseAddonData(nlohmann::json jsonAddon) {
	Addon addon;
	if(!jsonAddon["Name"].is_null()) {
		addon.setName(jsonAddon["Name"]);
	}
	if(!jsonAddon["LatestFiles"].is_null()) {
		nlohmann::json latestFile = getLatestFile(jsonAddon);
		if(latestFile != nullptr) {
			addon.setVersion(latestFile["FileName"]);
			addon.setSupportedVersion(latestFile["GameVersion"].at(0));
			addon.setDownloadLink(latestFile["DownloadURL"]);
			std::vector<std::string> modules;
			for(auto& module : latestFile["Modules"]) {
				modules.push_back(module["Foldername"]);
			}
			addon.setModules(modules);
		}
	}
	if(!jsonAddon["Attachments"].is_null()) {
		for(auto img : jsonAddon["Attachments"]) { //.at(0)["ThumbnailUrl"]);
			if(img["IsDefault"]) {
				addon.setImageLink(img["ThumbnailUrl"]);
			}
		}
		//addon.setImageLink(
	}
	if(!jsonAddon["DownloadCount"].is_null()) {
		addon.setTotaltDownloads(jsonAddon["DownloadCount"]);
	}
	return addon;
}

nlohmann::json Curse::getLatestFile(nlohmann::json jsonAddon) {
	nlohmann::json latestFile;
	std::string latestFileDate;
	std::string latestGameVersion;
	for(auto& latestFiles : jsonAddon["LatestFiles"]) {
		if(latestFileDate.empty()) {
			latestFileDate = latestFiles["FileDate"];
		}
		if(latestGameVersion.empty()) {
			latestGameVersion = latestFiles["GameVersion"].at(0);
		}
		if(!latestFiles["IsAlternate"]) {
			// Try to determine the highest game version supported
			std::string fileHighestVersion = latestFiles["GameVersion"].at(0);
			for(auto& gameVersion : latestFiles["GameVersion"]) {
				if(fileHighestVersion <= gameVersion) {
					fileHighestVersion = gameVersion;
				}
			}
			if(latestGameVersion < fileHighestVersion) {
				latestFile = latestFiles;
				latestFileDate = latestFiles["FileDate"];
				latestGameVersion = fileHighestVersion;
			// If the game version is the same, let the FileDate decide
			// which game files are the newest
			} else if(latestGameVersion == fileHighestVersion
				&& latestFileDate < latestFiles["FileDate"]) {
				latestFile = latestFiles;
				latestFileDate = latestFiles["FileDate"];
				latestGameVersion = fileHighestVersion;
			}
		}
	}

	return latestFile;
}

