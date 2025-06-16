#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"

// This callback function is called by cURL whenever it receives data from the server
static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    cJSON **json = (cJSON**)userdata;

    size_t bytes = size * nmemb;
    char *response = malloc(bytes + 1);
    if (response == NULL) {
        return 0;
    }

    memcpy(response, ptr, bytes);
    response[bytes] = '\0';

    *json = cJSON_Parse(response);
    free(response);

    return bytes;
}

int main() {
    CURL *curl;
    CURLcode res;
    char *url = "http://localhost:8086/ss";
    cJSON *json_array = NULL;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &json_array);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Access each JSON object in the array here
            cJSON *json_item = NULL;
            cJSON_ArrayForEach(json_item, json_array) {
                cJSON *name = cJSON_GetObjectItem(json_item, "nome_sistema_sensor");
                // cJSON *age = cJSON_GetObjectItem(json_item, "age");
                // cJSON *is_student = cJSON_GetObjectItem(json_item, "is_student");

                printf("Received JSON data:\n%s\n", cJSON_Print(json_item));
                printf("Name: %s\n", name->valuestring);
                // printf("Age: %d\n", age->valueint);
                // printf("Is student: %s\n", is_student->valueint ? "true" : "false");
            }
        }

        curl_easy_cleanup(curl);
    }

    if (json_array != NULL) {
        cJSON_Delete(json_array);
    }

    return 0;
}
