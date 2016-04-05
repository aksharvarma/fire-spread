/* This includes utility files that are used to make the other things
   easier. */

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


void generateMatrix(int ***forest, int rows, int cols, int iterations, int simulations, float pTree, float pBurning, float pGrow, float pImmune, float pLightning, int spread_type, int neighbourhood_type);

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

int random_age(){
  int age;

  if(U<0.25)
    age=BABY;
  else if(U<0.5)
    age=YOUNG;
  else if(U<0.75)
    age=MIDDLE;
  else
    age=OLD;
  
  return age;
}

/* Initialize the forest according to the requisite probabilities. */
void initForest_aging(int** forest,int rows, int cols, long double pTree, long double pBurning){
  int i,j, age;

  for(i=1;i<=rows;i++){
    for(j=1;j<=cols;j++){
      if(U<pTree){ /* Is there a tree? */
	/* Find the age */
	age=random_age();
	
	if(U<pBurning) /* Is it burning? */
	  forest[i][j]=age+BURNING_INCREMENT; /* Ref. macros.h */
	else
	  forest[i][j]=age;
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


/* This returns 1 if any neighbours burn, 0 otherwise */
int do_neighbours_burn(int** forest, int row_index, int col_index, int neighbourhood_type){

  int i=row_index;
  int j=col_index;
  
  switch(neighbourhood_type){
  case VON_NEUMANN:
    return ((forest[Nr(i)][Nc(j)]>=BURNING &&
	     forest[Nr(i)][Nc(j)]<=OLD_BURNING) ||
	    (forest[Er(i)][Ec(j)]>=BURNING &&
	     forest[Er(i)][Ec(j)]<=OLD_BURNING) ||
	    (forest[Wr(i)][Wc(j)]>=BURNING &&
	     forest[Wr(i)][Wc(j)]<=OLD_BURNING) ||
	    (forest[Sr(i)][Sc(j)]>=BURNING &&
	     forest[Sr(i)][Sc(j)]<=OLD_BURNING)
	    );
  case MOORE:
    return ((forest[Nr(i)][Nc(j)]>=BURNING &&
	     forest[Nr(i)][Nc(j)]<=OLD_BURNING) ||
	    (forest[Er(i)][Ec(j)]>=BURNING &&
	     forest[Er(i)][Ec(j)]<=OLD_BURNING) ||
	    (forest[Wr(i)][Wc(j)]>=BURNING &&
	     forest[Wr(i)][Wc(j)]<=OLD_BURNING) ||
	    (forest[Sr(i)][Sc(j)]>=BURNING &&
	     forest[Sr(i)][Sc(j)]<=OLD_BURNING)
	    ||
	    //Diagonals
	    (forest[NEr(i)][NEc(j)]>=BURNING &&
	     forest[NEr(i)][NEc(j)]<=OLD_BURNING) ||
	    (forest[SEr(i)][SEc(j)]>=BURNING &&
	     forest[SEr(i)][SEc(j)]<=OLD_BURNING) ||
	    (forest[NWr(i)][NWc(j)]>=BURNING &&
	     forest[NWr(i)][NWc(j)]<=OLD_BURNING) ||
	    (forest[SWr(i)][SWc(j)]>=BURNING &&
	     forest[SWr(i)][SWc(j)]<=OLD_BURNING)
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
    if(forest[Nr(i)][Nc(j)]>=BURNING && forest[Nr(i)][Nc(j)]<=OLD_BURNING)
      neighbors_on_fire++;
    if(forest[Er(i)][Ec(j)]>=BURNING && forest[Er(i)][Ec(j)]<=OLD_BURNING)
      neighbors_on_fire++;
    if(forest[Wr(i)][Wc(j)]>=BURNING && forest[Wr(i)][Wc(j)]<=OLD_BURNING)
      neighbors_on_fire++;
    if(forest[Sr(i)][Sc(j)]>=BURNING && forest[Sr(i)][Sc(j)]<=OLD_BURNING)
      neighbors_on_fire++;
    break;
  case MOORE:
    if(forest[Nr(i)][Nc(j)]>=BURNING && forest[Nr(i)][Nc(j)]<=OLD_BURNING)
      neighbors_on_fire++;
    if(forest[Er(i)][Ec(j)]>=BURNING && forest[Er(i)][Ec(j)]<=OLD_BURNING)
      neighbors_on_fire++;
    if(forest[Wr(i)][Wc(j)]>=BURNING && forest[Wr(i)][Wc(j)]<=OLD_BURNING)
      neighbors_on_fire++;
    if(forest[Sr(i)][Sc(j)]>=BURNING && forest[Sr(i)][Sc(j)]<=OLD_BURNING)
      neighbors_on_fire++;
    /* Diagonals */
    if(forest[NEr(i)][NEc(j)]>=BURNING && forest[NEr(i)][NEc(j)]<=OLD_BURNING)
      neighbors_on_fire++;
    if(forest[SEr(i)][SEc(j)]>=BURNING && forest[SEr(i)][SEc(j)]<=OLD_BURNING)
      neighbors_on_fire++;
    if(forest[NWr(i)][NWc(j)]>=BURNING && forest[NWr(i)][NWc(j)]<=OLD_BURNING)
      neighbors_on_fire++;
    if(forest[SWr(i)][SWc(j)]>=BURNING && forest[SWr(i)][SWc(j)]<=OLD_BURNING)
      neighbors_on_fire++;
    break;
  default:
    break;
  }
  return neighbors_on_fire;
}
int check_burning_wind(int** forest, int row_index, int col_index, int neighbourhood_type, int wind_speed, int wind_direction, long double pImmune,int rows, int cols)
{
	int i=row_index;
  int j=col_index;
  int neighbour_status = -5;
	switch(wind_speed)
	{
		case 0:
			return TREE;
			break;
		case 2:
			
			switch(wind_direction)
			{
				case SOUTH:
					neighbour_status = forest[Nr(Nr(i))%rows][Nc(Nc(j))%cols];
					break;
				case NORTH:
					neighbour_status = forest[Sr(Sr(i))%rows][Sc(Sc(j))%cols];
					break;
				case EAST:
					neighbour_status = forest[Wr(Wr(i))%rows][Wc(Wc(j))%cols];
					break;
				case WEST:
					neighbour_status = forest[Er(Er(i))%rows][Ec(Ec(j))%cols];
					break;
			}
			if (pImmune<U && neighbour_status == BURNING)
				return BURNING;
		case 1:
			neighbour_status = -5;
			switch(wind_direction)
			{
				case SOUTH:
					neighbour_status = forest[Nr(i)][Nc(j)];
					break;
				case NORTH:
					neighbour_status = forest[Sr(i)][Sc(j)];
					break;
				case EAST:
					neighbour_status = forest[Wr(i)][Wc(j)];
					break;
				case WEST:
					neighbour_status = forest[Er(i)][Ec(j)];
					break;
			}
			if (pImmune<U && neighbour_status == BURNING)
				return BURNING;
			else
				return TREE;	
	}
	
}


void findingValues(int ***forest, int ncols, int nrows, int time_instants) {
	FILE* fptr;
  fptr=fopen("forest_data.tr","w");
  if(fptr==NULL){
    printf("Error! File not opened\n");
  }
  int *burning = (int *)calloc(time_instants,sizeof(int));
	int *empty = (int *)calloc(time_instants,sizeof(int));	
	int *trees = (int *)calloc(time_instants,sizeof(int));
	int i,j,k;
	for(i = 0; i<time_instants;i++)
	{
		for(j=1; j<=nrows; j++)
		{
			for(k=1;k<=ncols; k++)
			{
				switch(forest[i][j][k]/10)
				{
					case 0: empty[i]++;
									break;
					case 1: trees[i]++;
									break;
					case 2: 
							burning[i]++;
									break;
				}
			}
		}
	}	
	for(i = 1; i<=time_instants; i++)
	{
		printf("%d %d %d %d\n", i, empty[i-1], burning[i-1], trees[i-1]);
		fprintf(fptr,	"%d %.2f %.2f %.2f\n", i, empty[i-1]/(ncols*nrows), burning[i-1]/(ncols*nrows), trees[i-1]/(ncols*nrows));
	}
	fclose(fptr);
}

void generateMatrix(int ***forest, int rows, int cols, int iterations, int simulations, float pTree, float pBurning, float pGrow, float pImmune, float pLightning, int spread_type, int neighbourhood_type){
		
	int i,j,k,l,count;
	FILE* fptr;
  	fptr=fopen("bell_please.tr","w");
  	if(fptr==NULL){
    		printf("Error! File not opened\n");
  	}	
	for(i = 0; i<simulations; i++)
	{
		initForest(forest[0], rows, cols, pTree, pBurning);
		
		count = 0;
/*		for(k = 1; k<=rows; k++)
		{
			for(l = 1; l<=cols; l++)
			{
				if((forest[0][k][l]/10<=2) && (forest[0][k][l]!=EMPTY_BURNING))
				{
					count++;
				}							
			}
		}
		fprintf(fptr,"%f ",count/(rows*cols));		
*/
		for(j = 1; j<=iterations; j++)
		{
			spread(forest[j-1],forest[j],rows, cols, pImmune, pLightning, pGrow, spread_type, neighbourhood_type);
			count = 0;
			for(k = 1; k<=rows; k++)
			{
				for(l = 1; l<=cols; l++)
				{
					if((forest[j][k][l]/10<=2) && (forest[0][k][l]!=EMPTY_BURNING))
					{
						count++;
					}							
				}
			}
			fprintf(fptr,"%f ",count/(rows*cols));
		}
		
		fprintf(fptr, "\n");
	}
		
	fclose(fptr);
}

