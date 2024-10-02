// SPDX-License-Identifier: Apache-2.0
#ifndef DRAFTER_H
#define DRAFTER_H


void drafter_init();
void drafter_cleanup();

void drafter_add_point(pair *);
void drafter_add_func(vector *);
void drafter_add_pw_func(vector *, vector *);
void drafter_add_str_func(char *);

void drafter_get_graph();
void drafter_show_graph();
void drafter_clear_graph();

#endif
