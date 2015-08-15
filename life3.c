
//  氏名　セキ　キンヨウ
//　学籍番号　141168G
#include <stdio.h>
#include <unistd.h>
#include<string.h>

#define HEIGHT 50
#define WIDTH 70

int cell_next[HEIGHT][WIDTH];
int full[150][HEIGHT][WIDTH];
int cell[HEIGHT][WIDTH];
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
int init_cells(h,w)
{
  int i, j;

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      cell[i][j] = 0;
    }
  }
  for(i = 23; i < 23+h;i++){
    for(j = 33; j <33+w; j++){
      cell[i][j] = rand() % 2;
    }
  }
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
int compare(int s, int t){
  int i,j,state;
  state = 0;
  for (i = 0;i < HEIGHT; i++){
    for(j= 0;j < WIDTH; j++){
      if (full[s][i][j] != full[t][i][j]){
	state = 1;
      }
    }
  }
  return state;
}

main(){
  int w,i,j,k,l,m;
  int state,count;

  FILE *fp;

  if ((fp = fopen("cells.txt", "w")) == NULL) {
    fprintf(stderr, "error: cannot open a file.\n");
    return 1;
  }
 for (w = 0;w <100; w++){
   init_cells(2,5);
  
   for(i =0; i <150; i++){
    for (j =0; j < HEIGHT; j++){
      for (k = 0; k <WIDTH; k++){
	full[i][j][k] =0;
       }
    }
  }
   for (j =0; j < HEIGHT; j++){
     for (k = 0; k <WIDTH; k++){
      full[0][j][k] = cell[j][k];
      }
    }
   
  for (j = 1; j < 150; j++){
      update_cells();
    
     for(l = 0; l < HEIGHT; l++){
       for(m = 0; m <WIDTH; m++){
	 full[j][l][m] = cell[l][m];
       }
     }
   }
  state=0;
  count=0;
  for (i=0; i <150;i++){
    for (k =i+1; k <150;k++){
      if(compare(i,k)==0){
	state =1;
      }
    }
    if(state==0){
      count ++;
	}
  }
  printf("%d\n",count);
  if (count >=100){
    print_cells(fp);
  }
 }
}



