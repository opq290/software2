//氏名　セキ　キンヨウ
//学籍番号　141168G

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define HEIGHT 50
#define WIDTH 70

int cell[HEIGHT][WIDTH];
int cell_next[HEIGHT][WIDTH];

void print_cells(FILE *fp)
{
  int i, j;

  fprintf(fp, "----------\n");

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      char c = (cell[i][j] == 1) ? '#' : ' ';
      fputc(c, fp);
    }
    fputc('\n', fp);
  }
  fflush(fp);

  sleep(1);
}


int init_cells()
{
  char buf[WIDTH];
  FILE *fp;
  int i, j, k;

 const char *filename = "tmp.txt";
  if((fp = fopen(filename,"r")) == NULL){
    printf ("error: can't open %s\n", filename);
    return 1;
  }
 
 for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      cell[i][j] = 0;
    }
 }
   i = 0;
  while (fgets (buf,WIDTH,fp) != NULL && (i < HEIGHT) ){
     for (j= 0; j < WIDTH; j++) {
       if (buf[j] == '#' ){
	 cell[i][j] = 1;
       }
       else if(buf[j] == '\0')
	 break;
        
     }
  
     i += 1;
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


int main()
{
  int gen = 1;

  FILE *fp;

  if ((fp = fopen("cells.txt", "w")) == NULL) {
    fprintf(stderr, "error: cannot open a file.\n");
    return 1;
  }

  init_cells();
  print_cells(fp);

  while (1) {
    printf("gen = %d\n", gen++);
    update_cells();
    print_cells(fp);
  }

  fclose(fp);
}
