#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#define WIDTH 75
#define HEIGHT 50

const double G = 1.0;

struct star
{
  double m;
  double x;
  double y;
  double vx;
  double vy;
  double tmpx;
  double tmpy;
};

struct star stars[3] = {
  { 1.0, 10.0, 10.0, 0.0, 0.0,0.0,0.0 },
  { 0.5, 10.1, 10.0, 0.2, 0.0,0.0,0.0 },{1.5, -15.0,5.0,0.3,0.0,0.0,0.0 } };

 int nstars = sizeof(stars) / sizeof(struct star);

void plot_stars(FILE *fp, const double t)
{
  int i;
  char space[WIDTH][HEIGHT];

  memset(space, ' ', sizeof(space));
  for (i = 0; i < nstars; i++) {
    const int x = WIDTH  / 2 + stars[i].x;
    const int y = HEIGHT / 2 + stars[i].y;
    if (x < 0 || x >= WIDTH)  continue;
    if (y < 0 || y >= HEIGHT) continue;
    char c = 'o';
    if (stars[i].m >= 1.0) c = 'O';
    space[x][y] = c;
  }

  int x, y;
  fprintf(fp, "----------\n");
  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++)
      fputc(space[x][y], fp);
    fputc('\n', fp);
  }
  fflush(fp);

  printf("t = %5.1f", t);
  for (i = 0; i < nstars; i++){ 
    printf(", stars[%d].x = %7.2f", i, stars[i].x);
    printf(",stars[%d].y = %7.2f", i, stars[i].y);
  }
  printf("\n");

  usleep(100 * 1000);
}

void update_velocities(double dt)
{
  int i, j;
  for (i = 0; i < nstars; i++) {
    double ax = 0;
    double ay = 0;
    double ahx = 0;
    double ahy = 0;
    for (j = 0; j < nstars; j++) {
      if (i == j) continue;
      const double rix = stars[i].x + stars[i].vx *(dt/2);
      const double riy = stars[i].y + stars[i].vy *(dt/2);
      const double rjx = stars[j].x + stars[j].vx *(dt/2);
      const double rjy = stars[j].y + stars[j].vy *(dt/2);
      const double dx  = rjx -rix;
      const double dy = rjy -riy;
      const double r = sqrt(dx * dx + dy * dy);
      if( r > 0.5){
	ax += stars[j].m *dx / (r * r *r );
        ay += stars[j].m *dy / (r * r *r );
      }
      else if(r != 0){
	 const double px = stars[i].m *stars[i].vx + stars[j].m * stars[j].vx;
	 const double py = stars[i].m *stars[i].vy + stars[j].m * stars[j].vy;
	 const double ms = stars[i].m +stars[j].m;
	 const double nx = (stars[i].x +stars[j].x)/2;
	 const double ny = (stars[i].y +stars[j].y)/2;
	 stars[i].m = ms;
	 stars[i].vx = px/ms;
	 stars[i].vy = py/ms;
	 stars[i].x = nx;
	 stars[i].y = ny;
	 stars[i].tmpx = 0;
	 stars[i].tmpy =0;
	 stars[j] = stars[i];
      }
      else 
	continue;
      
    }
    ax *= G;
    ay *= G;
    stars[i].vx += ax * dt;
    stars[i].vy += ay * dt;
    stars[i].tmpx = (stars[i].vx +stars[i].vx -ax*dt)/2;
    stars[i].tmpy = (stars[i].vy + stars[i].vy - ay*dt)/2;
  }
}

void update_positions(double dt){
  int i;
  for(i=0; i < nstars; i++){
    stars[i].x += stars[i].tmpx*dt;
    stars[i].y += stars[i].tmpy*dt;
}
}

int main(int argc, char *argv[])
{
  const char *space_file = "space.txt";
  FILE *fp;
  if ((fp = fopen(space_file, "w")) == NULL) {
    fprintf(stderr, "error: cannot open %s.\n", space_file);
    return 1;
  }

   double dt;
  const double stop_time = 400;
  int c;
  while((c = getopt(argc,argv,"t:h")) != -1){
    switch(c){
    case 't':
      dt = atof(optarg);
      break;
    case 'h':
      printf ("enter dt\n");
      break;
    }
  }
  int i;
  double t = 0;
  for (i = 0; t <= stop_time; i++, t += dt) {
    update_velocities(dt);
    update_positions(dt);
    if (i % 10 == 0) {
      plot_stars(fp, t);
    }
  }

  fclose(fp);
}
