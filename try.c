#include <stdio.h>
#include <unistd.h>
#include<string.h>

#define HEIGHT 50
#define WIDTH 70

int cell[HEIGHT][WIDTH];
int cell_next[HEIGHT][WIDTH];
int full[150][HEIGHT][WIDTH];

void print_cells(FILE *fp)
{
  int i, j;

  fprintf(fp, "----------\n");

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      char c = (full[0][i][j] == 1) ? '#' : ' ';
      fputc(c, fp);
    }
    fputc('\n', fp);
  }
  fflush(fp);

  sleep(1);
}
  

int init_cells()
{
  int i, j,w;
  int k[10];
 
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      cell[i][j] = 0;
    }
  }
  for (i = 24; i < 27; i++){
    for (j = 34; j < 37; j++){
      cell[i][j] = rand() % 2;
    }
  }
}

int compare(int i, int k){
  int l,m,state;
  state =1;
  for (l =0 ; l < HEIGHT; l++){
    for (m = 0; m < WIDTH; m++){
      if(full[i][l][m] != full[k][l][m]){
	state =0;
	break;
      }
    }
  }
  if (state == 1){
    return 0;
  }
}

int cell_number()
{
  int i, j,sum;
  sum = 0;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      if (cell[i][j] == 1) {
	sum += 1;
      }
    }
  }
  return sum;
}
void update_cells()
{
  int i, j, k, l;

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      int n = 0;
      for (k = i - 1; k <= i + 1; k++) {
	if (k < 0 || k >= HEIGHT) continue;
	for (l = j - 1; l <= j + 1; l++) {
	  if (k == i && l == j) continue;
	  if (l < 0 || l >= WIDTH) continue;
	  n += cell[k][l];
	}
      }
      if (cell[i][j] == 0) {
	if (n == 3) 
	  cell_next[i][j] = 1;
	else        
	  cell_next[i][j] = 0;
      } else {
	if (n == 2 || n == 3) 
	  cell_next[i][j] = 1;
	else
	  cell_next[i][j] = 0;
      }
    }
  }

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      cell[i][j] = cell_next[i][j];
    }
  }
}




int main()
{
  int gen = 1;
  int life_length = 0;
  int full[150][HEIGHT][WIDTH];
  FILE *fp;
  int i,j,k,w,l,m;
  int count,state;
 

  if ((fp = fopen("cells.txt", "w")) == NULL) {
    fprintf(stderr, "error: cannot open a file.\n");
    return 1;
  }
 for (w = 0;w <100; w++){
    init_cells();
   for(i =0; i <150; i ++){
    for (j =0; j < HEIGHT; j ++){
      for (k = 0; k <WIDTH; k++){
	full[i][j][k] =0;
      }
    }
  }
   for (j =0; j < HEIGHT; j ++){
     for (k = 0; k <WIDTH; k++){
    full[0][j][k]= cell[j][k];
      }
    }
   for (j =1; j < 150; j ++){
    update_cells();
    
     for(l = 0; l < HEIGHT; l ++){
       for(m = 0; m <WIDTH; m ++){
	 full[j][l][m] = cell[l][m];
       }
     }
   }
   count =0;
   state =0;
   for(i =0; i < 150; i ++){
     for (k = i+1; k <150; k ++){
       if (compare(i,k) == 0){
	 state = 1;
       }
       }
     if (state==0){
       count ++;
     }
   }
   if(count >= 1){
     print_cells(fp);
     printf("%d\n",count);
   }
  
 }
    
      
  fclose(fp);
}

