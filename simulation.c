#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<limits.h>
#include"spreads.h"
#include"utils.h"
#include"macros.h"

#include "cv.h"
#include "highgui.h"

void on_trackbar( int i, void* bleh)
{
 
 //printf("TRACKING... "); 
 
}

/* Global Probability variables */
/* long double pTree=0.8, pBurning=0.005, pLightning=0.00001, pImmune=0.25; */
/* Values used for quick testing */

long double pTree=0.8, pBurning=0.005, pLightning=0.01, pImmune=0.25, pGrow=0.1;


int main(){

  printf("Empty:%d\nTree:%d\nBurning:%d\nStill Burning:%d\n\n",
	 EMPTY, TREE, BURNING, STILL_BURNING);


  /* Seed the random number. Always!!!! */
  srand(time(NULL));
  /* Iteration variables and n x n matrix. */
  int i, j, k, n=50;/* 100; */
  /* Currently the same but can be changed when required. */
  int rows=n, cols=n;
  /* Number of steps to run simulation for */
  int steps=100;/* pow(10,N); */

  /* The types of the neighbourhood and the fire spreading */
  int neighbourhood_type=VON_NEUMANN;
  int spread_type=TWO_STEPS_TO_BURN;
  int boundary_type=PERIODIC;

  
  
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
  //initForest(forest[0], rows, cols, pTree, pBurning);
  print_forest(forest[0], rows, cols);
  printf("\n");
  file_print_forest(fptr,forest[0], rows, cols);
  fprintf(fptr, "\n");
  
  char *window_name = "Fire_spread";
  int thresh1 = 10*pGrow;
  int thresh2 = 100*pLightning;
  int thresh3 = 10*pImmune;

  // for time_delay
  int thresh4 = 1;
  int time_delay = thresh4;

  cvNamedWindow("Fire_spread", CV_WINDOW_NORMAL);
  //cvResizeWindow(window_name, 200, 200);   
  cvCreateTrackbar("pGrow", "Fire_spread", &thresh1, 10, 0);   
  cvCreateTrackbar("pLightning", "Fire_spread", &thresh2, 100, 0);   
  cvCreateTrackbar("pImmune", "Fire_spread", &thresh3, 10, 0);  

  cvCreateTrackbar("time_delay", "Fire_spread", &thresh4, 10, 0);

  /* Simulating for other steps */

  /* generateMatrix(forest, rows, cols,  steps, 1000,  pTree, pBurning, pGrow,  pImmune, pLightning, spread_type, neighbourhood_type, boundary_type); */

  for(k=1;k<steps;k++){

    pGrow=(float)(cvGetTrackbarPos("pGrow", "Fire_spread")/10.0);
    pLightning=(float)(cvGetTrackbarPos("pLightning", "Fire_spread")/100.0);
    pImmune = (float)(cvGetTrackbarPos("pImmune", "Fire_spread")/10.0);
    time_delay = cvGetTrackbarPos("time_delay", "Fire_spread");

    /* Filling the periodic boundaries before each step. */
    //printf("%llf\n", pBurning);
    fillBoundary(forest[k],rows, cols, boundary_type);
    /* The actual spreading of the forest fire. */
    spread(forest[k-1],forest[k],rows, cols, pImmune, pLightning, pGrow, spread_type, neighbourhood_type);
    /* spread_burn_prob_neighbours(forest[k-1],forest[k],rows, cols, pImmune, pLightning, neighbourhood_type); */
    
    /* Printing the resultant forests */
    print_forest(forest[k], rows, cols);
    printf("\n");
    file_print_forest(fptr,forest[k], rows, cols);
    fprintf(fptr, "\n");


    // Adding the openCV thing
    IplImage *input = cvCreateImage(cvSize(rows, cols), IPL_DEPTH_8U, 3);
    cv_animation(forest[k], rows, cols, k, input);  
    cvShowImage("Fire_spread", input);
    cvWaitKey(time_delay*1000);    

  }
  //findingValues(forest, cols, rows, steps);
  return 0;
}
