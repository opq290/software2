#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
int main(){
  char *index[3];
  char tmp[2];
 tmp[0] = '1';
 tmp[1] = '0';
 index[0] = malloc(sizeof(char) *3);
 strcpy(index[0],tmp);
 printf("%s\n",index[0]);
}
  
