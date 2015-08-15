#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 70
#define HEIGHT 40
#define BUFSIZE 1000
#define COM_OTHER 0
#define COM_QUIT 1
#define HISTORY_SIZE 100

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

void line_command()
{
  int x0, y0, x1, y1;
  x0 = atoi(strtok(NULL, " "));
  y0 = atoi(strtok(NULL, " "));
  x1 = atoi(strtok(NULL, " "));
  y1 = atoi(strtok(NULL, " "));

  draw_line(x0, y0, x1, y1);
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
