#ifndef SPREADS_H
#define SPREADS_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<limits.h>
#include"macros.h"
#include"utils.h"
#include"spreads.h"
#include "cv.h"
#include "highgui.h"

/* The function that decides the update function to choose */
void spread(int** forest_old, int** forest_new, int rows, int cols, long double pImmune, long double pLightning, long double pGrow, int spread_type, int neighbourhood_type);

/* The normal update function without any modifications. */
void spread_normal(int** forest_old, int** forest_new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type);

/* The update function where trees burn for two iterations. */
void spread_2_steps_to_burn(int** forest_old, int** forest_new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type);

/* The update function where burning prob is number of burning neighbours. */
void spread_burn_prob_neighbours(int** forest_old, int** forest_new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type);

/* The update function where the trees age slowly. */
void spread_aging_trees(int** forest_old, int** forest_new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type);


/* Trees can spontaneously grow at any moment. */
void spread_grow(int** old, int** new, int rows, int cols, long double pImmune, long double pLightning, long double pGrow, int neighbourhood_type);

void spread_wind(int **forest_old, int **forest_new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type, int wind_speed, int wind_direction); 


#endif
