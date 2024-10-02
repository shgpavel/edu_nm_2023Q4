#define _GNU_SOURCE
#include <jemalloc/jemalloc.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "draw/draw.h"
#include "funcs/funcs.h"
#include "methods/lagrange.h"
#include "methods/linear_spline.h"
#include "methods/newton.h"
#include "methods/penalty.h"
#include "methods/polynoms.h"
#include "methods/quad_spline.h"
#include "methods/qube_spline.h"
#include "types/pair.h"
#include "types/vector.h"

#define CONTROL_POINTS 150
#define BLUE_COLOR "\033[34m"
#define YELLOW_COLOR "\033[33m"
#define RESET_COLOR "\033[0m"

enum errors {
  LOADING,
  FILE_NOT_OPEN,
  FILE_TYPE_INVALID,
  PARSE,
  INVALID_INPUT,
  POINTS_EMPTY,
  FILE_IO_ERROR,
  POINT_NOT_FOUND,
  SET_INTERVAL,
  FUNCS_EMPTY
};


struct pair_char_pointers {
  char *a;
  char *b;
};

struct pair_char_pointers parse_args(char *);
void all_errors(enum errors);
void help();

int main(void) {
  char *input = NULL;
  size_t len = 0, p_allocated = 0, f_allocated = 0, interval_set = 0;
  pair segm;
  FILE *points_file;
  FILE *funcs_file;
  vector points;
  vector funcs;

  while (1) {
    printf(">> ");
    getline(&input, &len, stdin);

    if (strstr(input, "load")) {
      size_t flag = 0;
      if (strstr(input, "point")) {
        flag = 1;
      } else if (strstr(input, "func")) {
        flag = 2;
      } else {
        all_errors(LOADING);
        continue;
      }

      struct pair_char_pointers token = parse_args(input);
      if (flag == 1) {
        points_file = fopen(token.a, "r");
        if (points_file == NULL) {
          all_errors(FILE_NOT_OPEN);
          continue;
        }
        if (p_allocated) {
          vector_free(&points);
          p_allocated = 0;
        }

        size_t sz;
        int comp_file;
        comp_file = fscanf(points_file, "%zu", &sz);
        if (comp_file != 1) {
          all_errors(FILE_TYPE_INVALID);
          continue;
        }
        vector_init(&points, sz, sizeof(pair));
        p_allocated = 1;

        for (size_t i = 0; i < sz; ++i) {
          pair pushed;
          comp_file = fscanf(points_file, "%lf", &pushed.a);
          if (comp_file != 1) {
            all_errors(FILE_TYPE_INVALID);
            break;
          }
          vector_push(&points, &pushed);
        }

        for (size_t i = 0; i < sz; ++i) {
          comp_file = fscanf(points_file, "%lf", &(pair_get(&points, i)).b);
          if (comp_file != 1) {
            all_errors(FILE_TYPE_INVALID);
            continue;
          }
        }
        vector_print_pairs(&points);
        fclose(points_file);
      } else if (flag == 2) {
        funcs_file = fopen(token.a, "r");
        if (funcs_file == NULL) {
          all_errors(FILE_NOT_OPEN);
          continue;
        }
        if (f_allocated) {
          for (size_t i = 0; i < funcs.size; ++i) {
            vector_free(vector_get(&funcs, i));
            free(vector_get(&funcs, i));
          }
          free(funcs.data);
          f_allocated = 0;
        }

        vector_init(&funcs, 3, sizeof(vector));
        f_allocated = 1;

        char *resp_cache = NULL;
        size_t fun_len;
        ssize_t err = 0;

        while ((err = getline(&resp_cache, &fun_len, funcs_file)) != -1) {
          char *curr_pos = resp_cache;
          vector *to_push_v = (vector *)malloc(sizeof(vector));
          double to_push_d_swp;
          vector_init(to_push_v, 3, sizeof(double));

          for (; (sscanf(curr_pos, "%lf", &to_push_d_swp)) == 1;) {
            double to_push_d = to_push_d_swp;
            vector_push(to_push_v, &to_push_d);
            while (*curr_pos != '\0' && *curr_pos != ' ' && *curr_pos != '\t' &&
                   *curr_pos != '\n') {
              ++curr_pos;
            }
            while (*curr_pos == ' ' || *curr_pos == '\t' || *curr_pos == '\n') {
              ++curr_pos;
            }
          }
          vector_push(&funcs, to_push_v);
          free(to_push_v);
        }
        free(resp_cache);
        fclose(funcs_file);
      }
    }

    else if (strstr(input, "save")) {
      size_t flag = 0;
      if (strstr(input, "point")) {
        flag = 1;
      } else if (strstr(input, "func")) {
        flag = 2;
      } else {
        all_errors(LOADING);
        continue;
      }

      struct pair_char_pointers token = parse_args(input);
      if (flag == 1) {
        points_file = fopen(token.a, "w");
        if (!points_file) {
          all_errors(FILE_NOT_OPEN);
          continue;
        }
        if (!p_allocated) {
          all_errors(POINTS_EMPTY);
          continue;
        }

        int comp_file = fprintf(points_file, "%zu\n", points.size);
        if (comp_file < 0) {
          all_errors(FILE_IO_ERROR);
          continue;
        }

        size_t xory = 0;
        for (ssize_t i = 0; (size_t)i < points.size; ++i) {
          double to_push;
          if (xory == 0) {
            to_push = pair_get(&points, (size_t)i).a;
          } else {
            to_push = pair_get(&points, (size_t)i).b;
          }

          if ((size_t)i != points.size - 1) {
            comp_file = fprintf(points_file, "%lg ", to_push);
          } else {
            comp_file = fprintf(points_file, "%lg\n", to_push);
          }

          if (comp_file < 1) {
            all_errors(FILE_IO_ERROR);
            break;
          }

          if ((size_t)i == points.size - 1 && xory == 0) {
            xory = 1;
            i = -1;
          }
        }
        fclose(points_file);
      } else if (flag == 2) {
        funcs_file = fopen(token.a, "w");
        if (!funcs_file) {
          all_errors(FILE_NOT_OPEN);
          continue;
        }
        if (!f_allocated) {
          all_errors(FUNCS_EMPTY);
          continue;
        }

        for (size_t i = 0; i < funcs.size; ++i) {
          for (size_t j = 0; j < ((vector *)vector_get(&funcs, i))->size; ++j) {
            double to_push = *((double *)vector_get(vector_get(&funcs, i), j));
            int comp_file = 0;

            if (j == ((vector *)vector_get(&funcs, i))->size - 1) {
              comp_file = fprintf(funcs_file, "%lg\n", to_push);
            } else {
              comp_file = fprintf(funcs_file, "%lg ", to_push);
            }

            if (comp_file < 1) {
              all_errors(FILE_IO_ERROR);
              goto IO_ERR;
            }
          }
        }
      IO_ERR:
        fclose(funcs_file);
      }
    }

    else if (strstr(input, "set interval") || strstr(input, "add point") ||
             strstr(input, "remove point")) {
      size_t flag = 0;
      if (strstr(input, "set interval")) {
        flag = 1;
      } else if (strstr(input, "add point")) {
        flag = 2;
      } else if (strstr(input, "remove point")) {
        flag = 3;
      }

      double *to_push_x, *to_push_y;
      pair some;
      struct pair_char_pointers token = parse_args(input);

      if (flag == 1) {
        to_push_x = &segm.a;
        to_push_y = &segm.b;
      } else {
        to_push_x = &some.a;
        to_push_y = &some.b;
      }

      int comp_input = sscanf(token.a, "%lf", to_push_x);
      if (comp_input != 1) {
        all_errors(PARSE);
        continue;
      }
      if (token.b != NULL && *token.b != '\0') {
        token.a = strtok_r(NULL, " ", &token.b);
      } else {
        all_errors(PARSE);
        continue;
      }
      comp_input = sscanf(token.a, "%lf", to_push_y);
      if (comp_input != 1) {
        all_errors(PARSE);
        continue;
      }

      if (flag == 1) {
        printf("] Interval %lg %lg\n", segm.a, segm.b);
        interval_set = 1;
      } else if (flag == 2) {
        if (!p_allocated) {
          vector_init(&points, 1, sizeof(pair));
          p_allocated = 1;
        }
        vector_push(&points, &some);
        printf("] Added x = %lg y = %lg\n", some.a, some.b);
      } else if (flag == 3) {
        const double tol = 1e-3;
        size_t is_found = 0;
        for (size_t i = 0; i < points.size; ++i) {
          if (fabs(pair_get(&points, i).a - some.a) < tol &&
              fabs(pair_get(&points, i).b - some.b) < tol) {
            is_found = 1;
            vector_delete(&points, i);
            printf("] Deleted x = %lg y = %lg\n", some.a, some.b);
            if (points.size <= 0) {
              vector_free(&points);
              p_allocated = 0;
            }
          }
        }
        if (!is_found) {
          all_errors(POINT_NOT_FOUND);
          continue;
        }
      }
    }

    else if (strstr(input, "linspace point") ||
             strstr(input, "optimal point")) {
      size_t flag = 0;
      if (strstr(input, "optimal point")) flag = 1;

      size_t num_of_points;
      struct pair_char_pointers token = parse_args(input);

      int comp_input = sscanf(token.a, "%zu", &num_of_points);
      if (comp_input != 1 || num_of_points <= 1) {
        all_errors(PARSE);
        continue;
      }

      if (!interval_set) {
        all_errors(SET_INTERVAL);
        continue;
      }

      if (!p_allocated) {
        vector_init(&points, num_of_points, sizeof(pair));
        p_allocated = 1;
      }

      if (flag) {
        for (size_t i = 0; i < num_of_points; ++i) {
          double point = opt_point(segm, i, num_of_points);
          pair to_push = {point, func(point)};
          vector_push(&points, &to_push);
        }
      } else {
        for (size_t i = 0; i < num_of_points; ++i) {
          double point = segm.a + (double)i * ((segm.b - segm.a) /
                                               (double)(num_of_points - 1));
          pair to_push = {point, func(point)};
          vector_push(&points, &to_push);
        }
      }

      printf("] Added %zu points between %lg %lg\n", num_of_points, segm.a,
             segm.b);
    }

    else if (strstr(input, "print")) {
      if (strstr(input, "point")) {
        vector_print_pairs(&points);
      } else if (strstr(input, "func")) {
        if (f_allocated) {
          for (size_t i = 0; i < funcs.size; ++i) {
            vector_print(vector_get(&funcs, i));
          }
        }
      } else {
        all_errors(LOADING);
        continue;
      }
    }

    else if (strstr(input, "plot")) {
      // Maybe it should not be like that
      if (strstr(input, "clear")) {
        goto plot_clear;
      }
      if (!f_allocated) {
        all_errors(FUNCS_EMPTY);
        continue;
      }

      if (!strstr(input, "spline")) {
        for (size_t i = 0; i < funcs.size; ++i) {
          add_func(vector_get(&funcs, i));
        }
        plot(0);
      } else {
        if (!p_allocated) {
          all_errors(POINTS_EMPTY);
          continue;
        }
        add_spline_func(&funcs, &points);
        plot(1);
      }
    }

    else if (strstr(input, "clear")) {
      if (strstr(input, "point")) {
        if (!p_allocated) {
          all_errors(POINTS_EMPTY);
          continue;
        }
        vector_free(&points);
        p_allocated = 0;
      } else if (strstr(input, "func")) {
        if (!f_allocated) {
          all_errors(FUNCS_EMPTY);
          continue;
        }
        for (size_t i = 0; i < funcs.size; ++i) {
          vector_free(vector_get(&funcs, i));
          free(vector_get(&funcs, i));
        }
        free(funcs.data);
        f_allocated = 0;
      } else if (strstr(input, "plot")) {
     plot_clear:
        clear_plot();
      }
    }

    else if (strstr(input, "lagr") || strstr(input, "newton") ||
             strstr(input, "slae")) {
      size_t flag = 0;
      if (strstr(input, "newton")) flag = 1;
      if (strstr(input, "newton2")) flag = 2;
      if (strstr(input, "slae")) flag = 3;

      if (!p_allocated) {
        all_errors(POINTS_EMPTY);
        continue;
      }
      vector *res;
      switch (flag) {
        case 0:
          res = lagrange_poly(&points);
          break;
        case 1:
          res = newton_poly(&points);
          break;
        case 2:
          res = newton_poly_dd(&points);
          break;
        case 3:
          res = lagr_slae(&points);
          break;
      }
      if (!f_allocated) {
        vector_init(&funcs, 1, sizeof(vector));
        f_allocated = 1;
      }
      vector_push(&funcs, res);
    }

    else if (strstr(input, "linear") || strstr(input, "quad") ||
             strstr(input, "qube") || strstr(input, "spline")) {
      size_t flag = 0;
      if (strstr(input, "quad"))
        flag = 1;
      else if (strstr(input, "qube"))
        flag = 2;
      else if (strstr(input, "spline"))
        flag = 3;

      if (!p_allocated) {
        all_errors(POINTS_EMPTY);
        continue;
      }

      if (!interval_set) {
        all_errors(SET_INTERVAL);
        continue;
      }

      if (!f_allocated) {
        vector_init(&funcs, 1, sizeof(vector));
        f_allocated = 1;
      }

      vector *res = (vector *)malloc(sizeof(vector));
      vector_init(res, points.size - 1, sizeof(vector));

      switch (flag) {
        case 0:
          for (size_t i = 0; i < points.size - 1; ++i) {
            res = linear_spline(&points, i, res);
          }
          break;
        case 1:
          for (size_t i = 0; i < points.size - 2; i += 2) {
            res = quad_spline(&points, i, res);
          }
          break;
        case 2:
          for (size_t i = 0; i < points.size - 3; i += 3) {
            res = qube_spline(&points, i, res);
          }
          break;
        case 3:
          for (size_t i = 0; i < points.size - 2; i += 2) {
            res = spline_2_0(&points, i, res);
          }
          break;
      }

      for (size_t i = 0; i < res->size; ++i) {
        vector_push(&funcs, vector_get(res, i));
      }
    }

    else if (strstr(input, "exit"))
      break;
    else if (strstr(input, "help"))
      help();
    else
      all_errors(INVALID_INPUT);
  }
  if (p_allocated) vector_free(&points);
  if (f_allocated) {
    for (size_t i = 0; i < funcs.size; ++i) {
      vector_free(vector_get(&funcs, i));
      free(vector_get(&funcs, i));
    }
    free(funcs.data);
  }
  free(input);
  return 0;
}

struct pair_char_pointers parse_args(char *input) {
  char *token, *next_token;
  token = strtok_r(input, " ", &next_token);
  for (size_t i = 0; i < 3; ++i) {
    if (i == 2) break;

    if (next_token != NULL && *next_token != '\0') {
      token = strtok_r(NULL, " ", &next_token);
    } else
      break;
  }

  if (token[strlen(token) - 1] == '\n') {
    token[strlen(token) - 1] = '\0';
  }
  struct pair_char_pointers result = {token, next_token};
  return result;
}

void all_errors(enum errors error) {
  printf(YELLOW_COLOR "Error:" RESET_COLOR);
  switch (error) {
    case LOADING:
      printf(" Loading type not specified\n");
      break;
    case FILE_NOT_OPEN:
      printf(" Could not open such file\n");
      break;
    case FILE_TYPE_INVALID:
      printf(" Incompatible file type\n");
      break;
    case PARSE:
      printf(" Could not parse your input. Please try again\n");
      break;
    case INVALID_INPUT:
      printf(" Invalid input. Please type" BLUE_COLOR " help " RESET_COLOR
             "to see available commands\n");
      break;
    case POINTS_EMPTY:
      printf(" Points vector is empty\n");
      break;
    case FILE_IO_ERROR:
      printf(
          " Something went wrong during IO operation."
          " Please check if everything ok with FS\n");
      break;
    case POINT_NOT_FOUND:
      printf(" Could not find such point\n");
      break;
    case SET_INTERVAL:
      printf(" Please set interval first\n");
      break;
    case FUNCS_EMPTY:
      printf(" Funcs set are empty\n");
      break;
  }
}

void help() {
  printf(
      "\t\tInterpolation tool\t0.1\n"
      "  Usage:\n"
      "\thelp                       -- prints this message\n\n"
      "\tload points <filename>     -- load points from file\n"
      "\tsave points <filename>     -- save points to a file\n"
      "\tload funcs  <filename>     -- load functions from file\n"
      "\tsave funcs <filename>      -- save functions to a file\n\n"
      "\tset interval <a> <b>       -- set [a, b] segment of interpolation\n"
      "\tlinspace points <quantity> -- fill points with equidistant points\n"
      "\toptimal points <quantity>  -- fill points with optimal points\n"
      "\tadd point <x> <y>          -- add some point\n"
      "\tremove point <x> <y>       -- delete some point\n"
      "\tprint points               -- print current points\n\n"
      "\tprint funcs                -- print current functions\n"
      "\tplot                       -- get graphs\n"
      "\tplot splines               -- get graphs for piecewise-defined "
      "functions\n"
      "\tclear points               -- clears all points\n"
      "\tclear funcs                -- clears all functions\n"
      "\tclear plot                 -- clears everything on desmos plot\n"
      "\texit                       -- terminate\n\n"
      "\tlagr                       -- lagrange interpolation\n"
      "\tnewton                     -- newton interpolation\n"
      "\tlinear                     -- linear spline interpolation\n"
      "\tquad                       -- quad splines 2 1\n"
      "\tqube                       -- qube splines 3 2\n\n"
      "\tslae                       -- slae classic vander matrix "
      "interpolation\n"
      "\tnewton2                    -- another version for newton "
      "interpolation\n"
      "\tspline20                   -- quad spline С^0 functions\n");
}
