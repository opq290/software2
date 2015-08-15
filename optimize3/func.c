#include <math.h>
#include "func.h"

int f_dimension()
{
  return 3;
}

double f_value(const double x[])
{
  return (x[0]-2) * (x[0]-2) + (x[1]-3) * (x[1]-3) + x[2] * x[2];
}

void f_gradient(const double x[], double g[])
{
  g[0] = 2 * (x[0]-2);
  g[1] = 2 * (x[1]-3);
  g[2] = 2 * x[2];
}

