#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<limits.h>

// This allows generationg of uniform random variables
#define U ((long double)rand()/RAND_MAX)


#define VON_NEUMANN 1 //Only edge-connected neighbours are counted.
//North neighbor 
#define Nr(i) (i-1)
#define Nc(j) (j)
//East neighbor 		    
#define Er(i) (i)
#define Ec(j) (j+1)
//West neighbor 
#define Wr(i) (i)
#define Wc(j) (j-1)
// South neighbor
#define Sr(i) (i+1)
#define Sc(j) (j)

//Kind of cells
#define TREE (1)
#define BURNING (2)

int NROWS = 17;
int NCOLS = 17;

long double burnProbability = 0.9;
int num_steps = 5;
int num_experiments = 10;

void initForest(int** forest);
void spread(int** forest_old, int** forest_new);
int do_neighbours_burn(int** forest,int row_index,int col_index);
void fillBoundary(int **forest);
double per_of_forest_burned(int **forest);
void print_forest(int **forest);

int main()
{
	printf("\nTree:%d\nBurning:%d\n\n",TREE, BURNING);
	
	// Seed the random number. Always!!!! 
  	srand(time(NULL));

  	int i,j;
  	int **forest=(int**)calloc(NROWS+2,sizeof(int*));
  	for(i=0;i<NROWS+2;i++)
  		forest[i] = (int*)calloc(NCOLS+2,sizeof(int));
  	double per_forest_burned = 0;
	for(i=0;i<num_experiments;i++)
	{	
		initForest(forest);
		//print_forest(forest);

		for(j=0;j<num_steps;j++)
		{
			fillBoundary(forest);
			spread(forest,forest);
			//print_forest(forest);
		}
		per_forest_burned += per_of_forest_burned(forest); 
	}
	printf("\n%lf\n",(per_forest_burned/num_experiments));
}
//Initialize intial forest with just the middle tree burning
void initForest(int** forest)
{
	int i,j;
	for(i=1;i<=NROWS;i++)
	{
		for(j=1;j<=NCOLS;j++)
		{
			if(i==NROWS/2 && j==NCOLS/2)
				forest[i][j] = BURNING;
			else
				forest[i][j] = TREE;
		}
	}
}

void spread(int** forest_old, int** forest_new)
{
	int i,j;
	for(i=1;i<=NROWS;i++)
	{
		for(j=1;j<=NCOLS;j++)
		{
			if(forest_old[i][j]==BURNING)
				forest_new[i][j]=BURNING;
			else
			{	if(do_neighbours_burn(forest_old,i,j))
				{
					if(U<burnProbability)
						forest_new[i][j]=BURNING;
					else
						forest_new[i][j]=TREE;
				}
			}
		}
	}
}

int do_neighbours_burn(int** forest,int row_index,int col_index)
{
	return (forest[Nr(row_index)][Nc(col_index)]==BURNING ||
	    forest[Er(row_index)][Ec(col_index)]==BURNING ||
	    forest[Wr(row_index)][Wc(col_index)]==BURNING ||
	    forest[Sr(row_index)][Sc(col_index)]==BURNING
	    );
}
void fillBoundary(int** forest){
  int i,j;

  //Fill the row above; 
  i=0;
  for(j=1;j<=NCOLS;j++)
    forest[i][j]=forest[NROWS][j];

  // Fill the row below
  i=NROWS+1;
  for(j=1;j<=NCOLS;j++)
    forest[i][j]=forest[1][j];

  //Now fill the whole left column
  j=0;
  for(i=0;i<=NROWS+1;i++)
    forest[i][j]=forest[i][NCOLS];

  //Now fill the whole right column
  j=NCOLS+1;
  for(i=0;i<=NROWS+1;i++)
    forest[i][j]=forest[i][1];
  
  return;
}

void print_forest(int **forest)
{
	int i,j;
	for(i=1;i<=NROWS;i++)
	{
		for(j=1;j<=NCOLS;j++)
			printf("%d ",forest[i][j]);
		printf("\n");
	}
	printf("\n");
	
}

double per_of_forest_burned(int **forest)
{
	int i,j;
	int count_BURN = 0;
	for(i=1;i<=NROWS;i++)
	{
		for(j=1;j<=NCOLS;j++)
		{
			if(forest[i][j]==BURNING)
				count_BURN++;
		}
	}
	printf("%d\n",count_BURN);
	return (count_BURN*100/(NROWS*NCOLS));
	return 0;
}
