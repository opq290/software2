#ifndef _OPTIMIZE_H_
#define _OPTIMIZE_H_

int optimize( const int dim, double x[], 
	     void (*calc_grad)(const double [], double []));

#endif
