#include <stdio.h>

#include "../legacy/vector.h"

void fmt_exp(char *buf, double value) {
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

char* fmt_vec2str(vector *target) {
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
