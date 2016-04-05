/* This includes only the fire spread updating functions.
   New variations on that should be added here.
*/

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



	/* Actual definitions start now. */

	/* This function selects which kind of update function to call. */
void spread(int** old, int** new, int rows, int cols, long double pImmune, long double pLightning, long double pGrow, int spread_type, int neighbourhood_type){

  /* We swtich over "type" to select the spread function. */
  switch(spread_type){
  case NORMAL:			/* The normal variation */
    printf("Normal fire\n");
    spread_normal(old,new,rows, cols, pImmune, pLightning, neighbourhood_type);
    break;
  case TWO_STEPS_TO_BURN:	/* Tree takes two steps to burn */
    printf("Two steps to burn\n");
    spread_2_steps_to_burn(old,new,rows, cols, pImmune, pLightning, neighbourhood_type);
    break;
  case BURN_PROB_NEIGHBOURS:	/*Burning prob ~ on neighbour count*/
    printf("Burning prob ~ on neighbour count\n");
    spread_burn_prob_neighbours(old,new,rows, cols, pImmune, pLightning, neighbourhood_type);
    break;
  case AGING_TREES:	/* Trees age */
    printf("Trees age\n");
    spread_aging_trees(old,new,rows, cols, pImmune, pLightning, neighbourhood_type);
    break;
  case GROW:	/* Trees spontaneously grow */
    printf("Trees spontaneously grow\n");
    spread_grow(old,new,rows, cols, pImmune, pLightning, pGrow, neighbourhood_type);
    break;
  case WIND:
    printf("AAg\n");
    int wind_speed = 2;
    int wind_direction = EAST;
    spread_wind(old, new, rows, cols, pImmune, pLightning, neighbourhood_type, wind_speed, wind_direction); 
	      break;
  default:
    printf("Defaulting\n");
    spread_normal(old,new,rows, cols, pImmune, pLightning, neighbourhood_type);
    break;
  }
  return;
}


/* This is where the updating of the forest happens. */
void spread_normal(int** old, int** new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type){
  int i,j;

  /* Looping over all the cells */
  for(i=1;i<=rows;i++){
    for(j=1;j<=cols;j++){
      if(old[i][j]==EMPTY){ /* If empty, remain empty */
	new[i][j]=EMPTY;
      }else if(old[i][j]==BURNING){ /* If burning, burn down. */
	new[i][j]=EMPTY;
      }else if(old[i][j]==TREE){ /* if tree, */
	if(do_neighbours_burn(old,i,j,neighbourhood_type)){
	  /* and neigbours are burning */
	  if(U<pImmune){
	    new[i][j]=TREE;	/* keep tree if immune */
	  }
	  else{
	    new[i][j]=BURNING;	/* else burn it. */
	  }
	}else if(U<pLightning*(1-pImmune)){
	  new[i][j]=BURNING;
	}else {
	  new[i][j]=TREE;/* if neighbors aren't burning */
	}
      }else{			/* If it's none of these, */
	new[i][j]=ERROR;	/* there's something wrong. */
      }
    }
  }
  return;
}


/* A tree takes two steps to burn. */
void spread_2_steps_to_burn(int** old, int** new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type){
  int i,j;

  /* Looping over all the cells */
  for(i=1;i<=rows;i++){
    for(j=1;j<=cols;j++){
      if(old[i][j]==EMPTY){ /* If empty, remain empty */
	new[i][j]=EMPTY;
      }else if(old[i][j]==BURNING){ /* if burning, keep burning */
	new[i][j]=STILL_BURNING;
      }else if(old[i][j]==STILL_BURNING){ /* has been burning, burn down */
	new[i][j]=EMPTY;
      }else if(old[i][j]==TREE){ /* if tree, */
	if(do_neighbours_burn(old,i,j,neighbourhood_type)){ 			/* and neigbours are burning */
	  if(U<pImmune){
	    new[i][j]=TREE;	/* keep tree if immune */
	  }
	  else{
	    new[i][j]=BURNING;	/* else burn it. */
	  }
	}else if(U<pLightning*(1-pImmune)){
	  new[i][j]=BURNING;
	}else {
	  new[i][j]=TREE;/* if neighbors aren't burning */
	}
      }else{			/* If it's none of these, */
	new[i][j]=ERROR;	/* there's something wrong. */
      }
    }
  }
  return;
}

/* This is where the updating of the forest happens. */
void spread_burn_prob_neighbours(int** old, int** new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type){
  int i,j;
  double neighbors_on_fire;
  int n_neighbours;
  double prob_of_burning;
  
  switch(neighbourhood_type){
  case VON_NEUMANN:
    n_neighbours=4;
    break;
  case MOORE:
    n_neighbours=8;
    break;
  default:
    n_neighbours=4;
    break;
  }
  
  /* Looping over all the cells */
  for(i=1;i<=rows;i++){
    for(j=1;j<=cols;j++){
      if(old[i][j]==EMPTY){ /* If empty, remain empty */
	new[i][j]=EMPTY;
      }else if(old[i][j]==BURNING){ /* If burning, burn down. */
	new[i][j]=EMPTY;
      }else if(old[i][j]==TREE){ /* If tree, */
	if(do_neighbours_burn(old,i,j,neighbourhood_type)){			/* And neighbours are burning */

	  /* Count number of burning neighbours */
	  neighbors_on_fire=count_burning_neighbours(old,i,j,neighbourhood_type);

	  prob_of_burning=neighbors_on_fire/n_neighbours;
	  /* Prob according to number of burning neighbours  */
	  if(U<prob_of_burning){
	    new[i][j]=BURNING;	/* Burn it down */
	  }
	  else{
	    new[i][j]=TREE;	/* else keep it a tree */
	  }
	}else if(U<pLightning*(1-pImmune)){
	  new[i][j]=BURNING;
	}else {
	  new[i][j]=TREE;/* if neighbors aren't burning */
	}
      }else{			/* If it's none of these, */
	new[i][j]=ERROR;	/* there's something wrong. */
      }
    }
  }
  return;
}


/* Here the trees age in each step. */
void spread_aging_trees(int** old, int** new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type){
  int i,j;
  /* This is to tackle issues where the forest hasn't been 
     initialized with ages but the aging_trees spread model
     has been asked for.
  */
  static int init_flag=1;
  /* This is to ensure that the next block only runs once. */
    
  if(init_flag){
    for(i=1;i<=rows;i++){
      for(j=1;j<=cols;j++){
	if(old[i][j]==TREE){
	  old[i][j]=random_age();
	}else if(old[i][j]==BURNING){
	  old[i][j]=random_age()+BURNING_INCREMENT;
	}else if(old[i][j]==EMPTY){
	  old[i][j]=EMPTY;
	}else{
	  old[i][j]=ERROR;
	}
      }
    }
  }
  
  init_flag*=0;
  
  /* Looping over all the cells */
  for(i=1;i<=rows;i++){
    for(j=1;j<=cols;j++){
      if(old[i][j]==EMPTY ||
	 old[i][j]==EMPTY_TREE ||
	 old[i][j]==EMPTY_BURNING){
	/* If empty/tree died/tree burned down --> empty */
	new[i][j]=EMPTY;
      }else if(old[i][j]>=BABY_BURNING &&
	       old[i][j]<=OLD_BURNING){ /* If burning, age tree. */
	switch(old[i][j]){
	case BABY_BURNING:
	case OLD_BURNING:
	  new[i][j]=EMPTY;
	case YOUNG_BURNING:
	case MIDDLE_BURNING:
	  new[i][j]=old[i][j]+AGE_IT;
	}
	
      }else if(old[i][j]>=BABY &&
	       old[i][j]<=OLD){ /* if tree, */
	new[i][j]=old[i][j]+AGE_IT; /* age the tree */
	if(do_neighbours_burn(old,i,j,neighbourhood_type)){
	  /* and neigbours are burning */
	  if(U<pImmune){
	    new[i][j]=new[i][j]; /* Keep as tree */
	  }
	  else{
	    new[i][j]=new[i][j]+BURNING_INCREMENT;
	    /* else burn it. */
	  }
	}else if(U<pLightning*(1-pImmune)){
	  new[i][j]=new[i][j]+BURNING_INCREMENT;
	}else {
	  new[i][j]=new[i][j];/* if neighbors aren't burning */
	}
      }else{			/* If it's none of these, */
	new[i][j]=ERROR;	/* there's something wrong. */
      }
    }
  }
  return;

}



/* Trees can spontaneously grow at any moment. */
void spread_grow(int** old, int** new, int rows, int cols, long double pImmune, long double pLightning, long double pGrow, int neighbourhood_type){
  int i,j;

  /* Looping over all the cells */
  for(i=1;i<=rows;i++){
    for(j=1;j<=cols;j++){
      if(old[i][j]==EMPTY){ /* If empty, remain empty */
	if(U<pGrow)
	  new[i][j]=TREE;
	else
	  new[i][j]=EMPTY;
      }else if(old[i][j]==BURNING){ /* If burning, burn down. */
	new[i][j]=EMPTY;
      }else if(old[i][j]==TREE){ /* if tree, */
	if(do_neighbours_burn(old,i,j,neighbourhood_type)){
	  /* and neigbours are burning */
	  if(U<pImmune){
	    new[i][j]=TREE;	/* keep tree if immune */
	  }
	  else{
	    new[i][j]=BURNING;	/* else burn it. */
	  }
	}else if(U<pLightning*(1-pImmune)){
	  new[i][j]=BURNING;
	}else {
	  new[i][j]=TREE;/* if neighbors aren't burning */
	}
      }else{			/* If it's none of these, */
	new[i][j]=ERROR;	/* there's something wrong. */
      }
    }
  }
  return;
}
void spread_wind(int **old, int **new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type, int wind_speed, int wind_direction){
  int i,j;

  
  pLightning = 0;
  /* Looping over all the cells */
  for(i=1;i<=rows;i++){
    for(j=1;j<=cols;j++){
      if(old[i][j]==EMPTY){ /* If empty, remain empty */
	new[i][j]=EMPTY;
      }
      else if(old[i][j]==BURNING){ /* If burning, burn down. */
	new[i][j]=EMPTY;
      }
      else if(old[i][j]==TREE){ /* if tree, */
	if(do_neighbours_burn(old,i,j,neighbourhood_type)){
	  /* and neigbours are burning */
	  if(U<pImmune){
	    new[i][j]=check_burning_wind(old,i,j,neighbourhood_type, wind_speed, wind_direction, pImmune, rows,cols);	/* Check if Tree is not burning */
	    if(new[i][j]==BURNING)
	      printf("Burnnnnnnnn!!!\n");
	  }
	  else{
	    new[i][j]=BURNING;	/* else burn it. */
	  }
	}
	else{
	  new[i][j]=check_burning_wind(old,i,j,neighbourhood_type, wind_speed, wind_direction, pImmune, rows,cols);	/* if neighbors aren't burning */
	  if(new[i][j]==BURNING)
	    printf("Burnnnnnnnn!!!\n");
	}
	
	/* The lightning clause */
	if(U<pLightning*(1-pImmune)){
	  new[i][j]=BURNING;
	}else {
	  new[i][j]=TREE;
	}
      }
      else{			/* If it's none of these, */
	new[i][j]=ERROR;	/* there's something wrong. */
      }
    }
  }
  return;
}
