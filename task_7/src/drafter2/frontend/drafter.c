// SPDX-License-Identifier: Apache-2.0
// Frontend
#include <curl/curl.h>
#include <jemalloc/jemalloc.h>
#include <stdio.h>
#include <string.h>

#include "fmt.h"
#include "../legacy/pair.h"
#include "../legacy/vector.h"

// New funcs
void drafter_init() {
	curl_global_init(CURL_GLOBAL_ALL);
}

void drafter_cleanup() {
	curl_global_cleanup();
}

// TODO points with e
void drafter_add_point(pair *a) {
  char buf[64];
  sprintf(buf, "(%lg, %lg)", a->a, a->b);
  drafter_add_str_func(buf);
}

void drafter_add_func(vector *in) {
  char *staged = fmt_vec2str(in);
  drafter_add_str_func(staged);
  free(staged);
}

void drafter_add_pw_func(vector *funcs, vector *points) {
  for (size_t i = 0; i < points->size - 1 && i < funcs->size; ++i) {
		// ? need something better
		char *request_str = malloc(str_limit);
    char *staged = fmt_vec2str(vector_get(target, i));
    char limits[64];
		// looks dangerous now
    sprintf(limits, "{%lg <= x <= %lg:", pair_get(points, i).a,
            pair_get(points, i + 1).a);
    strcpy(request_str, limits);
    strcat(request_str, staged);
    strcat(request_str, "}");
		
    drafter_add_str_func(request_str);

		free(staged);
    free(request_str);
  }
}

void drafter_add_str_func() {
  CURL *curl;
  CURLcode res;
  char postfields[str_limit];

  sprintf(postfields, "{\"latex\": \"%s\"}", p);

	curl = curl_easy_init();
  if (!curl) {
		fprintf(stderr,
						"Error: Failed to init curl request [drafter asf]\n");
		exit(1);
	}

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");

	curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/add");
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

void drafter_get_graph() {
	CURL *curl;
  CURLcode res;
  struct memory_struct chunk;

  chunk.memory = malloc(1);
  chunk.size = 0;

  curl = curl_easy_init();
  if (!curl) {
		fprintf(stderr,
						"Error: Failed to init curl request [drafter gg]\n");
		exit(1);
	}
	
	if (spline_flag == 1) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/plot-splines");
	} else {
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/plot");
	}
	
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
			// Get rid of that sh..
			snprintf(command, sizeof(command), "xdg-open %s", filename);
			system(command);
		} else {
			fprintf(stderr, "Failed to open file for writing\n");
		}
	}
	curl_easy_cleanup(curl);
	free(chunk.memory);
}

void drafter_show_graph() {

}

void drafter_clear_graph() {
  CURL *curl;
  CURLcode res;
  char postfields[0];

  curl = curl_easy_init();
  if (!curl) {
		fprintf(stderr,
						"Error: Failed to init curl request [drafter cg]\n");
		exit(1);
	}

	struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(curl, CURLOPT_URL,
									 "http://localhost:3000/clear");
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_POST, 1L);

	res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		fprintf(stderr, "Error: curl_easy_perform() failed: %s\n",
						curl_easy_strerror(res));
	}
	curl_easy_cleanup(curl);
	curl_slist_free_all(headers);
}
