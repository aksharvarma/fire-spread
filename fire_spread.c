#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<limits.h>


#define N 2  /* Log10 of the number of steps */

/* This allows generationg of uniform random variables */
#define U ((long double)rand()/RAND_MAX)

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

/* Empty/Tree/Burning */
#define EMPTY (0)
#define TREE (1)
#define BURNING (2)


/* Function declarations */
/* A helper function that prints the forest at a particular time */
int** print_forest(int** forest, int rows, int cols );
/* A helper function that makes a string of a forest */
char *string_forest(char*,int** forest, int rows, int cols);
/* A helper function that prints the whole grid */
void print_grid(int** forest, int rows, int cols );
/* Initialize the forest according to the correct probabilities */
void initForest(int** forest,int rows, int cols, long double pTree,long double pBurning);
/* After every iteration, renew the boundaries */
void fillBoundary(int** forest, int rows, int cols);
/* The update function at each step */
void spread(int** forest_old, int** forest_new, int rows, int cols, long double pImmune, long double pLightning);


/* Global Probability variables */
long double pTree=0.8, pBurning=0.5, pLightning=0.00001, pImmune=0.25;

int main(){
  /* Seed the random number. Always!!!! */
  srand(time(NULL));
  int i, j, k, n=100;
  int rows=n, cols=n;
  int steps=pow(10,N);
  long double u1, u2;
  int ***forest=(int***)calloc(steps,sizeof(int**));
  for(i=0;i<steps;i++){
    forest[i]=(int**)calloc(n+2,sizeof(int*));
    for(j=0;j<n+2;j++){
      forest[i][j]=(int*)calloc(n+2,sizeof(int));
    }
  }

  /* for(k=0;k<steps;k++){ */
  /*   print_grid(forest[k], rows, cols); */
  /* } */

  
  FILE* fptr;
  fptr=fopen("forest.tr","w");
  if(fptr==NULL){
    printf("Error! File not opened\n");
  }
  
  initForest(forest[0], rows, cols, pTree, pBurning);
  
  for(k=0;k<steps;k++){
    fillBoundary(forest[k],rows, cols);
    /* for(i=1;i<=rows;i++){ */
    /*   for(j=1;j<=cols;j++){ */
	if(k){
	  spread(forest[k-1],forest[k],rows, cols, pImmune, pLightning);
	  /* forest[k][i][j]=(forest[k-1][Nr(i)][Nc(j)]+ */
	  /* 		   forest[k-1][Er(i)][Ec(j)]+ */
	  /* 		   forest[k-1][Wr(i)][Wc(j)]+ */
	  /* 		   forest[k-1][Sr(i)][Sc(j)])%3; */
	}
    /*   } */
    /* } */
    print_forest(forest[k], rows, cols);
  }


  /* sprintf(s,"%s", print_forest(forest[0], rows, cols)); */
  /* char *s=(char*)malloc((rows*cols*3)*sizeof(char)); */
  /* sprintf(s, "%d", 10000); */
  /* fprintf(fptr, "%s", string_forest(s, forest[0], rows, cols)); */
  
  /* for(k=0;k<steps;k++){ */
  /*   printf("%d\n",k); */
  /*   print_forest(forest[k], rows, cols); */
  /* } */

  return 0;
}


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

char *string_forest(char *s, int** forest, int rows, int cols){
  int i, j; 
 
  for(i=1;i<=rows;i++){
    for(j=1;j<=cols;j++){
      sprintf(s, "%d ", forest[i][j]);
    }
    sprintf(s,":");
  }
  sprintf(s,";");
  sprintf(s,"?");
  printf("%s", s);
  return s;
}
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


void initForest(int** forest,int rows, int cols, long double pTree, long double pBurning){
  int i,j;

  for(i=1;i<=rows;i++){
    for(j=1;j<=cols;j++){
      if(U<pTree){
	if(U<pBurning)
	  forest[i][j]=BURNING;
	else
	  forest[i][j]=TREE;
      }
      else
	forest[i][j]=EMPTY;
    }
  }
  return;
}

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


void spread(int** old, int** new, int rows, int cols, long double pImmune, long double pLightning){
  int i,j;

  /* printf(":::::::SPREADER:::::::\n"); */
  /* printf("Start\n"); */
  /* print_grid(old, rows, cols); */
  /* print_grid(new, rows, cols); */
  
  for(i=1;i<=rows;i++){
    for(j=1;j<=cols;j++){
      /* printf("i:%d j:%d val:%d\n", i, j, old[i][j]); */
      if(old[i][j]==0){
	/* printf("From nothing to nothing\n"); */
	new[i][j]=EMPTY;
	continue;
	/* printf("EMPTY: %d %d\n", i, j); */
      }else if(old[i][j]==2){
	/* printf("Burnt to the ground\n"); */
	new[i][j]=EMPTY;
	continue;
	/* printf("BURNING: %d %d\n", i, j); */
      }else if(old[i][j]==1){
	if(old[Nr(i)][Nc(j)]==BURNING ||
	   old[Er(i)][Ec(j)]==BURNING ||
	   old[Wr(i)][Wc(j)]==BURNING ||
	   old[Sr(i)][Sc(j)]==BURNING 
	   ){
	/* printf("TREE: %d %d\n", i, j); */
	  if(U<pImmune){
	    new[i][j]=TREE;
	    continue;
	  }
	  else{
	    new[i][j]=BURNING;
	    continue;
	  }
	}else{
	  new[i][j]=TREE;
	}
	continue;
      }else{
	new[i][j]=12;
      }
    }
  }
  /* printf("End\n"); */
  /* print_grid(old, rows, cols); */
  /* print_grid(new, rows, cols); */
  return;
}
