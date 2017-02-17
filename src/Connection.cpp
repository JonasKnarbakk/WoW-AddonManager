// Description: New Class
// Created by Jonas Knarbakk ()

#include "Connection.h"
#include <iostream>

Connection::Connection(){

}

Connection::~Connection(){
}

bool Connection::connect(std::string url){

    curl = curl_easy_init();

    if(curl){

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        // tell libcurl to follow redirection
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        return true;
    }
    return false;
}

bool Connection::save_data_to_file(std::string filename){
    CURLcode res;
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

            return false;
        }

        curl_easy_cleanup(curl);

        fclose(fileptr);
    } else {
        printf("Failed to save html to file.\n");
        return false;
    }

    return true;
}
