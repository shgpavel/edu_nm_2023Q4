#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

#include <jemalloc/jemalloc.h>

#include "common.h"
#include "types/vector.h"
#include "types/pair.h"
#include "methods/polynoms.h"
#include "methods/lagrange.h"
#include "methods/newton.h"
#include "methods/linear_spline.h"
#include "methods/quad_spline.h"
#include "methods/qube_spline.h"
#include "methods/penalty.h"
#include "draw/draw.h"
#include "funcs/funcs.h"

#define CONTROL_POINTS 150

void help();

size_t get_x_index(vector *points, double x) {
  for (size_t i = 0; i < points->size - 2; ++i) {
    if (pair_get(points, i).a <= x && x <= pair_get(points, i + 1).a) {
      return i;
    }
  }
}

/* OLD INTERFACE */
void penalty() {
  pair segm = {1.0, 2.542};
  FILE *matrix_csv = fopen("matrix.csv", "w");
  FILE *newtonp_csv = fopen("newtonp.csv", "w");
  FILE *s20_csv = fopen("s20.csv", "w");

  fprintf(matrix_csv, "n, m, RM, RMopt\n");
  fprintf(newtonp_csv, "n, m, RND, RNDopt\n");
  fprintf(s20_csv, "n, m, RS20, RS20opt\n");

  str_func("cot(x) - x");
  str_func("x = 1");
  str_func("x = 2.542");
  
  for (size_t s = 5; s <= 50; ++s) {
    vector points;
    vector_init(&points, s, sizeof(pair));
    for (size_t i = 0; i < s; ++i) {
      double point = segm.a 
        + (double)i * ((segm.b - segm.a) / (double)(s - 1));
      pair point_p = {point, func(point)};
      vector_push(&points, (void *)&point_p);
    }

    vector *res_m = lagr_slae(&points);
    vector *res_n = newton_poly_dd(&points);

    vector *res_s20 = (vector *)malloc(sizeof(vector));
    vector_init(res_s20, points.size - 1, sizeof(vector));
    for (size_t i = 0; i < points.size - 2; i += 2) {
      res_s20 = linear_spline(&points, i, res_s20);
    }
            
    vector opt_points;
    vector_init(&opt_points, s, sizeof(pair));
    for (size_t i = 0; i < s; ++i) {
      double point = opt_point(segm, i, s);
      pair point_p = {point, func(point)};
      vector_push(&opt_points, (void *)&point_p);
    }
    vector *res_mo = lagr_slae(&opt_points);
    vector *res_no = newton_poly_dd(&opt_points);

    vector *res_s20_o = (vector *)malloc(sizeof(vector));
    vector_init(res_s20_o, opt_points.size - 1, sizeof(vector));
    for (size_t i = 0; i < opt_points.size - 2; i += 2) {
      res_s20_o = linear_spline(&opt_points, i, res_s20_o);
    }
    
    double x = segm.a;
    double add = (segm.b - segm.a) / CONTROL_POINTS;
    double RM = 0, RN = 0, RS2 = 0, RMO = 0, RNO = 0, RS2O = 0;
    for (size_t i = 0; i < CONTROL_POINTS; ++i, x += add) {
      double diff_m = fabs(func(x) - poly_val(res_m, x));
      double diff_n = fabs(func(x) - poly_val(res_n, x));
      double diff_mo = fabs(func(x) - poly_val(res_mo, x));
      double diff_no = fabs(func(x) - poly_val(res_no, x));
      
      size_t index = get_x_index(&points, x);
      size_t index_o = get_x_index(&opt_points, x);
         
      double diff_s20 = 0, diff_s20_o = 0;
      if (index < res_s20->size) diff_s20 = fabs(func(x) - poly_val(vector_get(res_s20, index), x));
      if (index_o < res_s20_o->size) diff_s20_o = fabs(func(x) - poly_val(vector_get(res_s20_o, index_o), x));

      RM = diff_m > RM ? diff_m : RM;
      RN = diff_n > RN ? diff_n : RN;
      RMO = diff_mo > RMO ? diff_mo : RMO;
      RNO = diff_no > RNO ? diff_no : RNO;

      RS2 = diff_s20 > RS2 ? diff_s20 : RS2;
      RS2O = diff_s20_o > RS2O ? diff_s20_o : RS2O;
    }

    fprintf(matrix_csv, "%zu, %d, %lg, %lg\n", s, CONTROL_POINTS, RM, RMO);
    fprintf(newtonp_csv, "%zu, %d, %lg, %lg\n", s, CONTROL_POINTS, RN, RNO);

    fprintf(s20_csv, "%zu, %d, %lg, %lg\n", s, CONTROL_POINTS, RS2, RS2O);

    
    //add_spline_func(res_s20, &points);
    
    vector_free(res_m);
    vector_free(res_n);
    vector_free(res_mo);
    vector_free(res_no);
    
    vector_free(res_s20);
    vector_free(res_s20_o);

    vector_free(&points);
    vector_free(&opt_points);
    
    free(res_m);
    free(res_n);
    free(res_mo);
    free(res_no);

    free(res_s20);
    free(res_s20_o);
  }
  //plot(1);
  //clear_plot();
}

int old_main(void) {
  //penalty();
  
  pair segm = {1.0, 2.542};

  FILE *lagr_csv = fopen("lagr.csv", "w");
  FILE *newt_csv = fopen("newton.csv", "w");
  FILE *lin_csv = fopen("lin.csv", "w");
  FILE *quad_csv = fopen("quad.csv", "w");
  FILE *qube_csv = fopen("qube.csv", "w");

  if (!lagr_csv) return -1;
  if (!newt_csv) return -1;
  if (!lin_csv) return -1;
  if (!quad_csv) return -1;
  if (!qube_csv) return -1;
  
  fprintf(lagr_csv, "n, m, RL, RLopt\n");
  fprintf(newt_csv, "n, m, RN, RNopt\n");
  fprintf(lin_csv, "n, m, RS10, RS10opt\n");
  fprintf(quad_csv, "n, m, RS21, RS21opt\n");
  fprintf(qube_csv, "n, m, RS32, RS32opt\n");

  //str_func("cot(x) - x");
  //str_func("x = 1");
  //str_func("x = 2.542");

  for (size_t s = 5; s <= 50; ++s) {
    vector points;
    vector_init(&points, s, sizeof(pair));
    for (size_t i = 0; i < s; ++i) {
      double point = segm.a 
        + (double)i * ((segm.b - segm.a) / (double)(s - 1));
      pair point_p = {point, func(point)};
      vector_push(&points, (void *)&point_p);
    }

    vector *res_l = lagrange_poly(&points);
    vector *res_n = newton_poly(&points);

    vector *res_lin = (vector *)malloc(sizeof(vector));
    vector_init(res_lin, points.size - 1, sizeof(vector));
    for (size_t i = 0; i < points.size - 1; ++i) {
      res_lin = linear_spline(&points, i, res_lin);
    }

    
    vector *res_quad = (vector *)malloc(sizeof(vector));
    vector_init(res_quad, s - 1, sizeof(vector));
    for (size_t i = 0; i < points.size - 2; i += 2) {
      res_quad = quad_spline(&points, i, res_quad);
    }
    
    vector *res_qube = (vector *)malloc(sizeof(vector));
    vector_init(res_qube, s - 1, sizeof(vector));
    for (size_t i = 0; i < points.size - 3; i += 3) {
      res_qube = qube_spline(&points, i, res_qube);
    }    
        
    vector opt_points;
    vector_init(&opt_points, s, sizeof(pair));
    for (size_t i = 0; i < s; ++i) {
      double point = opt_point(segm, i, s);
      pair point_p = {point, func(point)};
      vector_push(&opt_points, (void *)&point_p);
    }
    vector *res_lo = lagrange_poly(&opt_points);
    vector *res_no = newton_poly(&opt_points);

    vector *res_lin_o = (vector *)malloc(sizeof(vector));
    vector_init(res_lin_o, s - 1, sizeof(vector));
    for (size_t i = 0; i < opt_points.size - 1; ++i) {
      res_lin_o = linear_spline(&opt_points, i, res_lin_o);
    }
    
    vector *res_quad_o = (vector *)malloc(sizeof(vector));
    vector_init(res_quad_o, s - 1, sizeof(vector));
    for (size_t i = 0; i < opt_points.size - 2; i += 2) {
      res_quad_o = quad_spline(&opt_points, i, res_quad_o);
    }
    
    vector *res_qube_o = (vector *)malloc(sizeof(vector));
    vector_init(res_qube_o, s - 1, sizeof(vector));
    for (size_t i = 0; i < opt_points.size - 3; i += 3) {
      res_qube_o = qube_spline(&opt_points, i, res_qube_o);
    }
    
    double x = segm.a;
    double add = (segm.b - segm.a) / CONTROL_POINTS;
    double RL = 0, RN = 0, RLO = 0,
      RNO = 0, RS1 = 0, RS2 = 0, RS3 = 0,
      RS1O = 0, RS2O = 0, RS3O = 0;
    for (size_t i = 0; i < CONTROL_POINTS; ++i, x += add) {
      double diff_l = fabs(func(x) - poly_val(res_l, x));
      double diff_n = fabs(func(x) - poly_val(res_n, x));
      double diff_lo = fabs(func(x) - poly_val(res_lo, x));
      double diff_no = fabs(func(x) - poly_val(res_no, x));

      
      size_t index = get_x_index(&points, x);
      size_t index_o = get_x_index(&opt_points, x);
         
      double diff_lin = 0, diff_quad = 0, diff_qube = 0, diff_lin_o = 0, diff_quad_o = 0, diff_qube_o = 0;
      if (index < res_lin->size) diff_lin = fabs(func(x) - poly_val(vector_get(res_lin, index), x));
      if (index < res_quad->size) diff_quad = fabs(func(x) - poly_val(vector_get(res_quad, index), x));
      if (index < res_qube->size) diff_qube = fabs(func(x) - poly_val(vector_get(res_qube, index), x));

      if (index_o < res_lin_o->size) diff_lin_o = fabs(func(x) - poly_val(vector_get(res_lin_o, index_o), x));
      if (index_o < res_quad_o->size)  diff_quad_o = fabs(func(x) - poly_val(vector_get(res_quad_o, index_o), x));
      if (index_o < res_qube_o->size)  diff_qube_o = fabs(func(x) - poly_val(vector_get(res_qube_o, index_o), x));
      
      RL = diff_l > RL ? diff_l : RL;
      RN = diff_n > RN ? diff_n : RN;
      RLO = diff_lo > RLO ? diff_lo : RLO;
      RNO = diff_no > RNO ? diff_no : RNO;

      RS1 = diff_lin > RS1 ? diff_lin : RS1;
      RS2 = diff_quad > RS2 ? diff_quad : RS2;
      RS3 = diff_qube > RS3 ? diff_qube : RS3;

      RS1O = diff_lin_o > RS1O ? diff_lin_o : RS1O;
      RS2O = diff_quad_o > RS2O ? diff_quad_o : RS2O;
      RS3O = diff_qube_o > RS3O ? diff_qube_o : RS3O;
    }

    fprintf(lagr_csv, "%zu, %d, %lg, %lg\n", s, CONTROL_POINTS, RL, RLO);
    fprintf(newt_csv, "%zu, %d, %lg, %lg\n", s, CONTROL_POINTS, RN, RNO);

    fprintf(lin_csv, "%zu, %d, %lg, %lg\n", s, CONTROL_POINTS, RS1, RS1O);
    fprintf(quad_csv, "%zu, %d, %lg, %lg\n", s, CONTROL_POINTS, RS2, RS2O);
    fprintf(qube_csv, "%zu, %d, %lg, %lg\n", s, CONTROL_POINTS, RS3, RS3O);

    
    //if (s % 3 == 0) add_func(res_l);
    //if (s % 3 == 0) add_func(res_lo);
    //if (s % 3 == 0) add_func(res_n);
    //if (s % 3 == 0) add_func(res_no);

    vector_free(res_l);
    vector_free(res_n);
    vector_free(res_lo);
    vector_free(res_no);
    
    vector_free(res_lin);
    vector_free(res_lin_o);
    vector_free(res_quad);
    vector_free(res_quad_o);
    vector_free(res_qube);
    vector_free(res_qube_o);

    vector_free(&points);
    vector_free(&opt_points);
    
    free(res_l);
    free(res_n);
    free(res_lo);
    free(res_no);

    free(res_lin);
    free(res_lin_o);
    free(res_quad);
    free(res_quad_o);
    free(res_qube);
    free(res_qube_o);
  }

  //plot(0);
  clear_plot();    
  
  return 0;
}

/* NEW INTERFACE */
int main(void) {
  char *input = NULL;
  size_t len = 0;
  FILE *points_file;
  FILE *funcs_file;
  vector points;
  vector funcs;
  
  while (1) {
    printf(">> ");
    getline(&input, &len, stdin);

    if (strstr(input, "load")) {

      size_t flag = 0;
      if (strstr(input, "points")) {
        flag = 1;
      } else if (strstr(input, "funcs")) {
        flag = 2;
      } else {
        printf("Error: load what?\n");
      }
      
      char *token = strtok(input, " ");

      for (size_t i = 0; i < 3; ++i) {
        if (i == 2) break;
        token = strtok(NULL, " ");
      }
      
      if (token[strlen(token) - 1] == '\n') {
        token[strlen(token) - 1] = '\0';
      }

      if (flag == 1) {
        points_file = fopen(token, "r");
        if (points_file == NULL) {
          printf("Error: Could not open such file\n");
          return -1;
        }
        size_t sz;
        fscanf(points_file, "%zu", &sz);
        vector_init(&points, sz, sizeof(pair));
        
        for (size_t i = 0; i < sz; ++i) {
          pair pushed;
          fscanf(points_file, "%lf", &pushed.a);
          vector_push(&points, &pushed);
        }

        for (size_t i = 0; i < sz; ++i) {
          fscanf(points_file, "%lf", &(pair_get(&points, i)).b);
        }
        vector_print_pairs(&points);
      } else if (flag == 2) {
        funcs_file = fopen(token, "r");
        if (funcs_file == NULL) {
          printf("Error: Could not open such file\n");
          return -1;
        }
        size_t sz;
        fscanf(funcs_file, "%zu", &sz);
        vector_init(&funcs, sz, sizeof(vector));
        
      }
    }

    if (strstr(input, "exit")) break;
    else help();
  }

  vector_free(&points);
  vector_free(&funcs);
  free(input);
  return 0;
}

void help() {
  printf("\t\tInterpolation tool\t0.1\n"
         "  Usage:\n"
         "\thelp                    -- prints this message\n\n"
         "\tload points <filename>  -- load points from file\n"
         "\tsave points <filename>  -- save points to a file\n"
         "\tload funcs  <filename>  -- load functions from file\n"
         "\tsave funcs <filename>   -- save functions to a file\n\n"
         "\tset interval            -- set [a, b] segment of interpolation\n"
         "\tlinspace points         -- fill points with equidistant points from interval\n"
         "\toptimal points          -- fill points with optimal points\n"
         "\tadd point               -- add some point\n"
         "\tremove point            -- delete some point\n"
         "\tprint points            -- print current points\n\n"
         "\tprint funcs             -- print current functions\n"
         "\tplot                    -- get graphs\n"
         "\tplot splines            -- get graphs for piecewise-defined functions\n"
         "\texit                    -- terminate\n\n"
         "\tlagr                    -- lagrange interpolation\n"
         "\tnewton                  -- newton interpolation\n"
         "\tlinear                  -- linear spline interpolation\n"
         "\tquad                    -- quad splines 2 1\n"
         "\tqube                    -- qube splines 3 2\n\n"
         "\tslae                    -- slae classic vander matrix interpolation\n"
         "\tnewton2                 -- another version for newton interpolation\n"
         "\tspline20                -- quad spline С^0 functions\n"
         );
}
