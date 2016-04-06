/* This defines all the macros that are used in all the other codes */
#include "cv.h"
#include "highgui.h"

#ifndef MACROS_H
#define MACROS_H

#define N 2  /* Log10 of the number of steps */


/*The directions*/
#define NORTH 1
#define SOUTH -1
#define WEST 2
#define EAST -2

/* This allows generationg of uniform random variables */
#define U ((long double)rand()/RAND_MAX)



#define VON_NEUMANN 1 /*Only edge-connected neighbours are counted.*/
/* North neighbor */
#define Nr(i) (i-1)
#define Nc(j) (j)
/* East neighbor */		    
#define Er(i) (i)
#define Ec(j) (j+1)
/* West neighbor */
#define Wr(i) (i)
#define Wc(j) (j-1)
/* South neighbor */
#define Sr(i) (i+1)
#define Sc(j) (j)



#define MOORE 2	/*Even diagonally connected cells are neighbours*/
/* North-East neighbor */
#define NEr(i) (i-1)
#define NEc(j) (j+1)
/* South-East neighbor */		    
#define SEr(i) (i+1)
#define SEc(j) (j+1)
/* North-West neighbor */
#define NWr(i) (i-1)
#define NWc(j) (j-1)
/* South-West neighbor */
#define SWr(i) (i+1)
#define SWc(j) (j-1)


/* Kinds of cells */
#define ERROR (-1)
#define EMPTY (0)
#define TREE (10)
#define BURNING (20)
#define STILL_BURNING (29)

#define AGE_IT (1)
/* Kinds of trees (for exercise 6) */
#define BABY (11)
#define YOUNG (BABY+AGE_IT)
#define MIDDLE (YOUNG+AGE_IT)
#define OLD (MIDDLE+AGE_IT)
#define EMPTY_TREE (OLD+AGE_IT)	/* To handle an edge case */

#define BURNING_INCREMENT (10)
/* NOTE: ALWAYS KEEP THE BURNING NUMBERS 10 MORE THAN TREE NUMBERS */
/* Kinds of trees (for exercise 6) */
#define BABY_BURNING (BABY+BURNING_INCREMENT)
#define YOUNG_BURNING (YOUNG+BURNING_INCREMENT)
#define MIDDLE_BURNING (MIDDLE+BURNING_INCREMENT)
#define OLD_BURNING (OLD+BURNING_INCREMENT)
#define EMPTY_BURNING (OLD_BURNING+AGE_IT) /* To handle an edge case */

/* Kinds of the spread update functions */
#define NORMAL (1)
#define TWO_STEPS_TO_BURN (2)
#define BURN_PROB_NEIGHBOURS (3)
#define AGING_TREES (4)
#define GROW (5)
#define WIND (6)
#define DAMP (7)
#define REALITY (8)

#define RED 255, 0, 0
#define GREEN 0, 255, 0
#define BLUE 0, 0, 255

#define BLACK 0, 0, 0
#define WHITE 255, 255, 255

/*STILL_BURNING*/
#define YELLOW 255, 247, 0

/* These are the TREE colors at different ages alongwith GREEN */
#define LIGHT_GREEN 187, 255, 141
#define TEAL 30, 170, 128
#define BROWN 138, 108, 37

/* These are the burning colors alongwith RED */
#define MAROON 142, 40, 40
#define LIGHT_RED 255, 105, 105
#define PINK 255, 141, 191

/* Kinds of boundary */
#define PERIODIC (1)
#define REFLECTIVE (2)


#endif
