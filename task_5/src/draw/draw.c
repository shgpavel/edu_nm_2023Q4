#include <stdio.h>
#include <string.h>
#include <jemalloc/jemalloc.h>
//#include <stdlib.h>
#include <curl/curl.h>

#include "../common.h"
#include "../types/vector.h"

#define str_limit 20000

char *go_str(vector *target) {
  char *equation = malloc(str_limit);
  char *ptr = equation;

  for (size_t i = 0; i < target->size; ++i) {
    sprintf(ptr, "%lgx^%zu", 
                    unwrap_double(vector_get(target, i)), i);
    ptr += strlen(ptr);
    if ((i < target->size - 1) &&
        unwrap_double(vector_get(target, i + 1)) > 0) {
      sprintf(ptr, "%s", "+");
      ptr += strlen(ptr);
    }
  }
  return equation; 
}

struct memory_struct {
  size_t size;
  char *memory;
};

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct memory_struct *mem = (struct memory_struct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if (!ptr) {
    fprintf(stderr, "Error: Not enough memory\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

void add_func(vector *target) {
  char *p = go_str(target);
  
  CURL *curl;
  CURLcode res;
  char postfields[1024];

  sprintf(postfields, "{\"latex\": \"%s\"}", p);

  curl = curl_easy_init();
  
  if (curl) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/add-function");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
      fprintf(stderr, "Error: curl_easy_perform() failed: %s\n", 
          curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
  }
  free(p);
}

void plot() {
  CURL *curl;
  CURLcode res;
  struct memory_struct chunk;

  chunk.memory = malloc(1);
  chunk.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/plot");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      fprintf(stderr, "Error: curl_easy_perform() failed: %s\n",
          curl_easy_strerror(res));
    } else {
      const char *filename = "/tmp/plot.html";
      FILE *file = fopen(filename, "w");
      if (file != NULL) {
        fwrite(chunk.memory, sizeof(char), chunk.size, file);
        fclose(file);

        char command[256];
        snprintf(command, sizeof(command), "xdg-open %s", filename);
        system(command);
      } else {
        fprintf(stderr, "Failed to open file for writing\n");
      }
    }

    curl_easy_cleanup(curl);
    free(chunk.memory);
  }
  
  curl_global_cleanup();
}

