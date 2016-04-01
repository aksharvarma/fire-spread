#ifndef SPREADS_H
#define SPREADS_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<limits.h>
#include"macros.h"
#include"utils.h"

/* The function that decides the update function to choose */
void spread(int** forest_old, int** forest_new, int rows, int cols, long double pImmune, long double pLightning, int spread_type, int neighbourhood_type);

/* The normal update function without any modifications. */
void spread_normal(int** forest_old, int** forest_new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type);

/* The update function where trees burn for two iterations. */
void spread_2_steps_to_burn(int** forest_old, int** forest_new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type);

/* The update function where burning prob is number of burning neighbours. */
void spread_burn_prob_neighbours(int** forest_old, int** forest_new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type);

#endif
