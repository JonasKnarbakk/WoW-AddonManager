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

	boost::algorithm::to_lower(searchTerm);
	std::vector<Addon> addons;

	// Search the json data
	for (auto obj : jsonData["data"]) {
		std::string currentObject = obj["Name"].get<std::string>();
		boost::algorithm::to_lower(currentObject);
		if(currentObject.find(searchTerm) != std::string::npos) {
			Addon addon{};
			if(!obj["Name"].is_null()) {
				addon.setName(obj["Name"]);
			}
			if(!obj["LatestFiles"].is_null()) {
				addon.setVersion(obj["LatestFiles"].at(0)["FileName"]);
				addon.setSupportedVersion(obj["LatestFiles"].at(0)["GameVersion"].at(0));
				addon.setDownloadLink(obj["LatestFiles"].at(0)["DownloadURL"]);
			}
			if(!obj["Attachments"].is_null()) {
				for(auto img : obj["Attachments"]) { //.at(0)["ThumbnailUrl"]);
					if(img["IsDefault"]) {
						addon.setImageLink(img["ThumbnailUrl"]);
					}
				}
				//addon.setImageLink(
			}
			if(!obj["DownloadCount"].is_null()) {
				addon.setTotaltDownloads(obj["DownloadCount"]);
			}
			std::cout << "Found: " << currentObject << std::endl;
			addons.push_back(addon);
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
