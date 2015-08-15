#include <stdio.h>
#include <unistd.h>
#include<string.h>
int full[3][2][2]={{{2,2},{1,1}},{{3,3},{4,4}},{{3,3},{4,6}}};
int compare(int i, int k){
  int l,m,state;
  state =1;
  for (l =0 ; l < 2; l++){
    for (m = 0; m < 2; m++){
      if(memcmp(full[i][l][m], full[k][l][m],2) ==0){
	state =0;
	break;
      }
    }
  }
  if (state == 1){
    return 0;
  }
  else
    return 1;
}


main(){
  if (compare(full[1],full[0]) == 0){
    printf("jsdhakd\n");
  }
}
