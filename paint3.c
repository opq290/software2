#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH 70
#define HEIGHT 40
#define BUFSIZE 1000
#define COM_OTHER 0
#define COM_QUIT 1
#define HISTORY_SIZE 100
#define COM_UNDO 2
char canvas[WIDTH][HEIGHT];
char *history[HISTORY_SIZE];

void print_canvas(FILE *fp)
{
  int x, y;

  fprintf(fp, "----------\n");

  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++) {
      char c = canvas[x][y];
      fputc(c, fp);
    }
    fputc('\n', fp);
  }
  fflush(fp);
}

int init_canvas()
{
  memset(canvas, ' ', sizeof(canvas));
}

void draw_line(int x0, int y0, int x1, int y1)
{
double dx, dy;
double k;
 int x,y,xs,xf,ys,yf;
 dx = x1-x0;
 dy = y1-y0;
  if(x0 <x1){
     xs= x0;
     xf =x1;
   }
  else {
    xs = x1;
    xf = x0;
  }
  if(y0 <y1){
     ys= y0;
     yf =y1;
   }
  else {
    ys = y1;
    yf = y0;
  }

 if (dx != 0){
   k = dy/dx;
   y = ys;
     for(x = xs; x <=xf; x++){
       canvas[x][(int)(y+0.5)] = '#';
       y += k;
     }
 }
 else{
   for(y = ys; y <= yf; y ++){
     canvas[x0][y] = '#';
   }
 }
 
}
void draw_circle(int x0, int y0, int r){
  int x,y;
  double x1,y1;
  double t;
  for(t = 0; t <2*M_PI;t += M_PI/160){
  x1 = x0+r*sin(t);
  y1 = y0+r*cos(t);
  if ((x1-(int)x1) < 0.5){
    x = (int)x1;
  }
    else{
      x = (int)(x1) +1;
    }
  if ((y1-(int)y1) < 0.5){
    y = (int)y1;
  }
    else{
      y = (int)(y1) +1;
    }

  
  canvas[x][y] = '#';
}

  
}
    

void line_command()
{
  int x0, y0, x1, y1;
  x0 = atoi(strtok(NULL, " "));
  y0 = atoi(strtok(NULL, " "));
  x1 = atoi(strtok(NULL, " "));
  y1 = atoi(strtok(NULL, " "));

  draw_line(x0, y0, x1, y1);
}
void rec_command(){
  int x0,y0,x1,y1;
  x0 = atoi(strtok(NULL," "));
  y0 = atoi(strtok(NULL," "));
  x1 = atoi(strtok(NULL," "));
  y1 = atoi(strtok(NULL," "));
  draw_line(x0,y0,x1,y0);
  draw_line(x1,y0,x1,y1);
  draw_line(x1,y1,x0,y1);
  draw_line(x0,y1,x0,y0);
}
void circle_command(){
  int x0,y0,r;
  x0 = atoi(strtok(NULL," "));
  y0 = atoi(strtok(NULL," "));
  r  = atoi(strtok(NULL," "));
  draw_circle(x0,y0,r);
}
void undo_command(){
  int n;
  n =0;
  while (strcmp(history[n],"undo\n") != 0){
    n++;
  }
  free(history[n]);
 int com,m;
  const char *canvas_file = "canvas.txt";
  FILE *fp;
 

 if ((fp = fopen(canvas_file, "w")) == NULL) {
    fprintf(stderr, "error: cannot open %s.\n", canvas_file);
 
  }
 init_canvas();
print_canvas(fp);
  
  for(m = 0; m < n-1; m ++){
    com = interpret_command(history[m]);
    if(strcmp(history[m],"undo\n") == 0)
      break;
 }
  }


  
int interpret_command(const char *command)
{
  char buf[BUFSIZE];
  strcpy(buf, command);

  char *s = strtok(buf, " ");
  char c = tolower(s[0]);

  switch (c) {
  case 'l':
    line_command();
    break;
  case 'q':
    return COM_QUIT;
  case 'r':
    rec_command();
    break;
  case'c':
    circle_command();
    break;
  case 'u':

    undo_command();
  default:
    break;
  }
  return COM_OTHER;
}

int main()
{
  int com, n, i;
  const char *canvas_file = "canvas.txt";
  FILE *fp;
  char buf[BUFSIZE];

  if ((fp = fopen(canvas_file, "w")) == NULL) {
    fprintf(stderr, "error: cannot open %s.\n", canvas_file);
    return 1;
  }

  init_canvas();
  print_canvas(fp);

  n = 0;
  while (1) {
    printf("%d > ", n);
    fgets(buf, BUFSIZE, stdin);

    history[n] = (char*)malloc(sizeof(char) * strlen(buf));
    strcpy(history[n], buf);
    if (++n >= HISTORY_SIZE) break;
			       
    com = interpret_command(buf);
    if (com == COM_QUIT) break;
    print_canvas(fp);
  }

  fclose(fp);

  if ((fp = fopen("history.txt", "w")) != NULL) {
    for (i = 0; i < n; i++) 
      fprintf(fp, "%s", history[i]);
    fclose(fp);
  }
}
