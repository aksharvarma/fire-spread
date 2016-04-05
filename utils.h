#ifndef UTILS_H
#define UTILS_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<limits.h>
#include"utils.h"
#include"macros.h"
#include "cv.h"
#include "highgui.h"
/* Function declarations */

void findingValues(int ***forest, int ncols, int nrows, int time_instants);

/* A helper function that prints the forest at a particular time */
int** print_forest(int** forest, int rows, int cols );

/* A helper function that prints the matrix to a file. */
void file_print_forest(FILE *fptr, int** forest, int rows, int cols);

/* A helper function that prints the whole grid */
void print_grid(int** forest, int rows, int cols );

/* Initialize the forest according to the correct probabilities */
void initForest(int** forest,int rows, int cols, long double pTree,long double pBurning);

/* Find a random age for a tree. For initialization purposes. */
int random_age();

/* Initialize the forest with aging tree */
void initForest_aging(int** forest,int rows, int cols, long double pTree,long double pBurning);

/* After every iteration, renew the boundaries */
void fillBoundary(int** forest, int rows, int cols);

/* This returns 1 if any neighbours burn, 0 otherwise */
int do_neighbours_burn(int** forest, int row_index, int col_index, int neighbourhood_type);

/* This counts the number of burning neighbours */
int count_burning_neighbours(int** forest, int row_index, int col_index, int neighbourhood_type);

int check_burning_wind(int** forest, int row_index, int col_index, int neighbourhood_type, int wind_speed, int wind_direction, long double pImmune, int rows, int cols);

#endif
