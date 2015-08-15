#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "optimize.h"

double calc_norm(const int dim, double v[])
{
  double tmp = 0;
  int i = 0;
  for (i = 0; i < dim; i++) {
    tmp += v[i] * v[i];
  }
  tmp = sqrt(tmp);
  return tmp;
}

int optimize( const int dim, double x[], 
	     void (*calc_grad)(const double [], double []))
{
  int i;
  
  double *g = malloc(dim * sizeof(double));
  double *g_tmp = malloc(dim * sizeof(double));
  double *p = malloc(dim * sizeof(double));
  double *p_tmp = malloc(dim * sizeof(double));
  int iter = 0;
  double tmp = 0,alpha = 0, beta = 0;
  double norm2 = 1;
  (*calc_grad)(x, g);
  (*calc_grad)(x, p);
  while (++iter < 10) {
    if(iter == 1) {
      norm2 =1;
    }
    else{
      norm2 = calc_norm(dim, g_tmp);
    }
    for(i = 0; i < dim; i++){
      g_tmp[i] = g[i];
      p_tmp[i] = p[i];
    }
    (*calc_grad)(x, g);
    const double norm1 = calc_norm(dim, g);
    tmp = alpha;
    printf("norm2 = %f\n",norm2);
    alpha = (norm1*norm1)/(norm2 * norm2);
    for(i = 0; i < dim; i++){
      p[i] = p_tmp[i]*alpha + g[i];
    }
    printf("%3d norm1 = %7.4f", iter, norm1);
    for (i = 0; i < dim; i++) {
      printf(", x[%d] = %7.4f", i, x[i]);
    }
    putchar('\n');

    if (norm1 < 0.001) break;

    for (i = 0; i < dim; i++) {
      p[i] = g[i] + alpha *p_tmp[i];
    }
    for(i = 0; i < dim; i++){
      x[i] = x[i] + beta;
    }
     
  }
  return iter;
}

