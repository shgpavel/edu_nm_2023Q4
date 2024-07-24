#include <curl/curl.h>
#include <jemalloc/jemalloc.h>
#include <stdio.h>
#include <string.h>

#include "../common.h"
#include "../types/pair.h"
#include "../types/vector.h"

#define str_limit 4000

struct memory_struct {
  size_t size;
  char *memory;
};

static size_t write_callback(void *contents, size_t size, size_t nmemb,
                             void *userp) {
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

void format_exp(char *buf, double value) {
  sprintf(buf, "%.6e", value);
  char *e_pos = strchr(buf, 'e');

  short exponent = 0;
  sscanf(e_pos + 1, "%hd", &exponent);

  size_t shift_length = strlen(e_pos);
  memmove(e_pos + 5 + (exponent < 0 ? 2 : 1), e_pos + 1, shift_length);
  memcpy(e_pos, "*10^{", 5);
  e_pos += 5;
  sprintf(e_pos, "%hd}", exponent);
}

char *go_str(vector *target) {
  char *equation = malloc(str_limit);
  char *ptr = equation;

  char buffer[50];
  for (size_t i = 0; i < target->size; ++i) {
    if (vector_val(target, i) > 1e+6 || vector_val(target, i) < 1e-4) {
      format_exp(buffer, vector_val(target, i));
      sprintf(ptr, "%sx^{%zu}", buffer, i);
      memset(buffer, '\0', 50);
    } else {
      sprintf(ptr, "%lgx^{%zu}", vector_val(target, i), i);
    }
    ptr += strlen(ptr);
    if ((i < target->size - 1) && vector_val(target, i + 1) >= 0) {
      sprintf(ptr, "%s", "+");
      ++ptr;
    }
  }
  return equation;
}

void str_func(char *p) {
  CURL *curl;
  CURLcode res;
  char postfields[str_limit];

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
}

void add_func(vector *target) {
  char *p = go_str(target);
  str_func(p);
  free(p);
}

void add_point(pair *a) {
  char buf[50];
  sprintf(buf, "(%lg, %lg)", a->a, a->b);
  str_func(buf);
}

void add_spline_func(vector *target, vector *points) {
  for (size_t i = 0; i < points->size - 1 && i < target->size; ++i) {
    char *res_str = malloc(str_limit);
    char *p = go_str(vector_get(target, i));
    char limits[50];
    sprintf(limits, "{%lg <= x <= %lg:", pair_get(points, i).a,
            pair_get(points, i + 1).a);
    strcpy(res_str, limits);
    strcat(res_str, p);
    strcat(res_str, "}");
    str_func(res_str);
    free(p);
    free(res_str);
  }
}

void plot(size_t spline_flag) {
  CURL *curl;
  CURLcode res;
  struct memory_struct chunk;

  chunk.memory = malloc(1);
  chunk.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if (curl) {
    if (spline_flag == 1) {
      curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/plot-splines");
    } else
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

void clear_plot() {
  CURL *curl;
  CURLcode res;
  char postfields[0];

  curl = curl_easy_init();
  if (curl) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_URL,
                     "http://localhost:3000/clear-functions");
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
}
