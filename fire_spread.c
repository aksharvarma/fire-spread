/* Don't use this humongous source. It has everything in the same place. */
/* This has been split into four different files to ease things. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<limits.h>


#define N 2  /* Log10 of the number of steps */

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
#define TREE (1)
#define BURNING (2)
#define STILL_BURNING (3)

/* Kinds of trees (for exercise 6) */
#define BABY (10)
#define YOUNG (11)
#define MIDDLE (12)
#define OLD (13)


/* Kinds of the spread update functions */
#define NORMAL 1
#define TWO_STEPS_TO_BURN 2
#define BURN_PROB_NEIGHBOURS 3



/* Function declarations */
/* A helper function that prints the forest at a particular time */
int** print_forest(int** forest, int rows, int cols );
/* A helper function that prints the matrix to a file. */
void file_print_forest(FILE *fptr, int** forest, int rows, int cols);
/* A helper function that prints the whole grid */
void print_grid(int** forest, int rows, int cols );
/* Initialize the forest according to the correct probabilities */
void initForest(int** forest,int rows, int cols, long double pTree,long double pBurning);
/* After every iteration, renew the boundaries */
void fillBoundary(int** forest, int rows, int cols);



/* The function that decides the update function to choose */
void spread(int** forest_old, int** forest_new, int rows, int cols, long double pImmune, long double pLightning, int spread_type, int neighbourhood_type);

/* This returns 1 if any neighbours burn, 0 otherwise */
int do_neighbours_burn(int** forest, int row_index, int col_index, int neighbourhood_type);

/* This counts the number of burning neighbours */
int count_burning_neighbours(int** forest, int row_index, int col_index, int neighbourhood_type);

/* The normal update function without any modifications. */
void spread_normal(int** forest_old, int** forest_new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type);

/* The update function where trees burn for two iterations. */
void spread_2_steps_to_burn(int** forest_old, int** forest_new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type);

/* The update function where burning prob is number of burning neighbours. */
void spread_burn_prob_neighbours(int** forest_old, int** forest_new, int rows, int cols, long double pImmune, long double pLightning, int neighbourhood_type);


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


/* This merely prints the forest matrix */
int** print_forest(int** forest, int rows, int cols ){
  int i, j;

  /* printf("__________________________\n"); */
  for(i=1;i<=rows;i++){
    /* printf("| "); */
    for(j=1;j<=cols;j++){
      printf("%d ", forest[i][j]);
    }
    printf("\n");
  }
  /* printf("__________________________\n"); */
  return forest;
}

/* This prints to a file */
void file_print_forest(FILE *fptr, int** forest, int rows, int cols){
  int i, j; 
 
  for(i=1;i<=rows;i++){
    for(j=1;j<=cols;j++){
      fprintf(fptr, "%d ", forest[i][j]);
    }
    fprintf(fptr,"\n");
  }
  return ;
}

/* This prints the whole grid. Made for testing the periodic boundary. */
void print_grid(int** forest, int rows, int cols ){
  int i, j;

  printf("__________________________\n");
  for(i=0;i<=rows+1;i++){
    printf("| ");
    for(j=0;j<=cols+1;j++){
      printf("%d ", forest[i][j]);
    }
    printf("\n");
  }
  /* printf("__________________________\n"); */
  return;
}


/* Initialize the forest according to the requisite probabilities. */
void initForest(int** forest,int rows, int cols, long double pTree, long double pBurning){
  int i,j;

  for(i=1;i<=rows;i++){
    for(j=1;j<=cols;j++){
      if(U<pTree){ /* Is there a tree? */
	if(U<pBurning) /* Is it burning? */
	  forest[i][j]=BURNING;
	else
	  forest[i][j]=TREE;
      }
      else
	forest[i][j]=EMPTY; /* Nothing there. */
    }
  }
  return;
}

/* This is the function that fills the periodic boundaries */
void fillBoundary(int** forest, int rows, int cols){
  int i,j;

  /* Fill the row above; */
  i=0;
  for(j=1;j<=cols;j++)
    forest[i][j]=forest[rows][j];

  /* Fill the row below */
  i=rows+1;
  for(j=1;j<=cols;j++)
    forest[i][j]=forest[1][j];

  /* Now fill the whole left column */
  j=0;
  for(i=0;i<=rows+1;i++)
    forest[i][j]=forest[i][cols];

  /* Now fill the whole right column */
  j=cols+1;
  for(i=0;i<=rows+1;i++)
    forest[i][j]=forest[i][1];
  
  return;
}


/* This function selects which kind of update function to call. */
void spread(int** old, int** new, int rows, int cols, long double pImmune, long double pLightning, int spread_type, int neighbourhood_type){

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
  default:
    printf("Defaulting\n");
    spread_normal(old,new,rows, cols, pImmune, pLightning, neighbourhood_type);
    break;
  }
}



/* This returns 1 if any neighbours burn, 0 otherwise */
int do_neighbours_burn(int** forest, int row_index, int col_index, int neighbourhood_type){

  int i=row_index;
  int j=col_index;
  
  switch(neighbourhood_type){
  case VON_NEUMANN:
    return (forest[Nr(i)][Nc(j)]==BURNING ||
	    forest[Er(i)][Ec(j)]==BURNING ||
	    forest[Wr(i)][Wc(j)]==BURNING ||
	    forest[Sr(i)][Sc(j)]==BURNING
	    );
  case MOORE:
    return (forest[Nr(i)][Nc(j)]==BURNING ||
	    forest[Er(i)][Ec(j)]==BURNING ||
	    forest[Wr(i)][Wc(j)]==BURNING ||
	    forest[Sr(i)][Sc(j)]==BURNING ||
	    //Diagonals
	    forest[NEr(i)][NEc(j)]==BURNING ||
	    forest[SEr(i)][SEc(j)]==BURNING ||
	    forest[NWr(i)][NWc(j)]==BURNING ||
	    forest[SWr(i)][SWc(j)]==BURNING 
	    );
  default:
    return 0;
  }
}

/* This counts the number of burning neighbours */
int count_burning_neighbours(int** forest, int row_index, int col_index, int neighbourhood_type){
  int neighbors_on_fire=0;
  int i=row_index;
  int j=col_index;

  switch(neighbourhood_type){
  case VON_NEUMANN:
    if(forest[Nr(i)][Nc(j)]==BURNING)
      neighbors_on_fire++;
    if(forest[Er(i)][Ec(j)]==BURNING)
      neighbors_on_fire++;
    if(forest[Wr(i)][Wc(j)]==BURNING)
      neighbors_on_fire++;
    if(forest[Sr(i)][Sc(j)]==BURNING)
      neighbors_on_fire++;
    break;
  case MOORE:
    if(forest[Nr(i)][Nc(j)]==BURNING)
      neighbors_on_fire++;
    if(forest[Er(i)][Ec(j)]==BURNING)
      neighbors_on_fire++;
    if(forest[Wr(i)][Wc(j)]==BURNING)
      neighbors_on_fire++;
    if(forest[Sr(i)][Sc(j)]==BURNING)
      neighbors_on_fire++;
    /* Diagonals */
    if(forest[NEr(i)][NEc(j)]==BURNING)
      neighbors_on_fire++;
    if(forest[SEr(i)][SEc(j)]==BURNING)
      neighbors_on_fire++;
    if(forest[NWr(i)][NWc(j)]==BURNING)
      neighbors_on_fire++;
    if(forest[SWr(i)][SWc(j)]==BURNING)
      neighbors_on_fire++;
    break;
  default:
    break;
  }
  return neighbors_on_fire;
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
	}else{
	  new[i][j]=TREE;	/* if neighbors aren't burning */
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
	}else{
	  new[i][j]=TREE;	/* if neighbors aren't burning */
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
	}else{
	  new[i][j]=TREE;	/* if neighbors aren't burning */
	}
      }else{			/* If it's none of these, */
	new[i][j]=ERROR;	/* there's something wrong. */
      }
    }
  }
  return;
}

