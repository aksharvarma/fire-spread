#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<limits.h>
#include"spreads.h"
#include"utils.h"
#include"macros.h"


/* Global Probability variables */
/* long double pTree=0.8, pBurning=0.005, pLightning=0.00001, pImmune=0.25; */
/* Values used for quick testing */
long double pTree=0.8, pBurning=0.5, pLightning=0.01, pImmune=0.25;

int main(){
  printf("Empty:%d\nTree:%d\nBurning:%d\nStill Burning:%d\n\n",
	 EMPTY, TREE, BURNING, STILL_BURNING);

  /* Seed the random number. Always!!!! */
  srand(time(NULL));
  /* Iteration variables and n x n matrix. */
  int i, j, k, n=3;/* 100; */
  /* Currently the same but can be changed when required. */
  int rows=n, cols=n;
  /* Number of steps to run simulation for */
  int steps=3;/* pow(10,N); */

  /* The types of the neighbourhood and the fire spreading */
  int neighbourhood_type=MOORE;
  int spread_type=BURN_PROB_NEIGHBOURS;
  
  
  /* The 3d matrix which stores all states of the forest */

  int ***forest=(int***)calloc(steps,sizeof(int**));
  for(i=0;i<steps;i++){
    /* It is initialized with more space to accomodate the boundaries */
    forest[i]=(int**)calloc(n+2,sizeof(int*));
    for(j=0;j<n+2;j++){
      forest[i][j]=(int*)calloc(n+2,sizeof(int)); /* Boundary again */
    }
  }

  
  /* Will be used to print to file */
  FILE* fptr;
  fptr=fopen("forest.tr","w");
  if(fptr==NULL){
    printf("Error! File not opened\n");
  }
  
  /* Initializing the forest */
  initForest(forest[0], rows, cols, pTree, pBurning);
  print_forest(forest[0], rows, cols);
  printf("\n");
  file_print_forest(fptr,forest[0], rows, cols);
  fprintf(fptr, "\n");
  
  /* Simulating for other steps */
  for(k=1;k<steps;k++){
    /* Filling the periodic boundaries before each step. */
    fillBoundary(forest[k],rows, cols);
    /* The actual spreading of the forest fire. */
    spread(forest[k-1],forest[k],rows, cols, pImmune, pLightning, spread_type, neighbourhood_type);
    /* spread_burn_prob_neighbours(forest[k-1],forest[k],rows, cols, pImmune, pLightning, neighbourhood_type); */
    
    /* Printing the resultant forests */
    print_forest(forest[k], rows, cols);
    printf("\n");
    file_print_forest(fptr,forest[k], rows, cols);
    fprintf(fptr, "\n");
  }
  return 0;
}
