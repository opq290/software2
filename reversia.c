//学籍番号　141168G
//氏名　　　セキ　キンヨウ
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#define INFINITY 64
void win_lose();
double min_node();
int pass(int turn);
struct xy
{
  int x;
  int y;
};

struct xy directions[8] = {
  { -1, -1 }, {  0, -1 }, { 1, -1 }, 
  { -1,  0 },             { 1,  0 }, 
  { -1,  1 }, {  0,  1 }, { 1,  1 } 
};

int board[8][8];
int board_copy1[8][8];
int board_copy2[8][8];
void init_board()
{
  int x, y;
  for (y = 0; y < 8; y++) 
    for (x = 0; x < 8; x++)
      board[x][y] = 0;

  board[4][3] = board[3][4] =  1;  // black disks
  board[3][3] = board[4][4] = -1;  // white disks
}

void print_board()
{
  int x, y;
  puts("\n abcdefgh");
  for (y = 0; y < 8; y++) {
    printf("%d", y + 1);
    for (x = 0; x < 8; x++) {
      const int d = board[x][y];
      char c = '.';
      if (d ==  1) c = 'O'; // black disk
      if (d == -1) c = '#'; // white disk
      putchar(c);
    }
    putchar('\n');
  }
  putchar('\n');
}

int can_flip(const int side,  
	     struct xy sq, const struct xy dir) // square, direction
{
  assert(board[sq.x][sq.y] == 0);
  int n = 0;
  do {
    sq.x += dir.x; 
    sq.y += dir.y; 
    n++;
    if (sq.x < 0 || sq.x >= 8 || sq.y < 0 || sq.y >= 8) return 0;
  } while (board[sq.x][sq.y] == -side);
  if (board[sq.x][sq.y] == 0) return 0;
  if (n <= 1) return 0;
  return 1;
}

int is_legal_move(int side, struct xy sq)
{
  assert(sq.x >= 0 && sq.x < 8 && sq.y >= 0 && sq.y < 8);
  int i;
  if (board[sq.x][sq.y] != 0) return 0;
  for (i = 0; i < 8; i++) {
    if (can_flip(side, sq, directions[i])) return 1;
  }
  return 0;
}

int place_disk(const int side, const struct xy sq)
{
  assert(is_legal_move(side, sq));
  int n = 0, i;
  for (i = 0; i < 8; i++) {
    const struct xy dir = directions[i];
    if (!can_flip(side, sq, dir)) continue;
    int x = sq.x + dir.x;
    int y = sq.y + dir.y;
    while (board[x][y] == -side) {
      board[x][y] = side;
      n++;
      x += dir.x;
      y += dir.y;
    }
  }
  board[sq.x][sq.y] = side;
  assert(n > 0);
  return n;
}



double eval_func(){
  double diskn,bn,wn;
  int x,y;
  struct xy corner[4] ={{0,0},{0,7},{7,0},{7,7}};
  struct xy near_corner[12] ={{0,1},{1,0},{1,1},{1,6},{0,6},{1,7},{6,0},{6,6},{7,6},{6,7},{7,1},{6,1}};
  bn = 0;
  wn = 0;
  for(x = 1; x < 7; x++){
    for(y = 1; y < 7; y++){
      if(board[x][y] == 1){
	bn ++;
      }
      else if(board[x][y] == -1){
	wn ++;
      }
    }
  }

  for(y = 2; y < 6; y++){
    if(board[0][y] == 1){
      bn ++;
    }
    if(board[7][y]== 1){
      bn++;
    }
    if(board[0][y] == -1){
      wn++;
    }
    if (board[7][y] == -1){
      wn++;
    }
  }
  for(x = 2; x < 6; x++){
    if (board[x][0] == 1){
      bn++;
    }
    if(board[x][7] == 1){
      bn++;
    }
    if(board[x][0] == -1){
      wn++;
    }
    if(board[x][7] == -1){
      wn++;
    }
  }


  int i;
  for(i = 0; i < 4; i++){
    const struct xy cor = corner[i];
    if(board[cor.x][cor.y] == -1){
      wn += 1.5;
    }
    else if(board[cor.x][cor.y] == 1){
      bn += 1.5;
    }
  }
  for(i = 0; i < 12; i++){
    const struct xy ncor = near_corner[i];
    if(board[ncor.x][ncor.y] == -1){
      wn += 0.5;
    }
    else if(board[ncor.x][ncor.y] == 1){
      bn += 0.5;
    }
  }
  diskn = bn - wn;
  // printf("kokomade2\n");
  return diskn;
}

void save_board(int board_copy[8][8]){
  int x,y;
  // printf("save_board works\n");
  for(x = 0; x < 8; x++){
    for(y = 0; y < 8; y++){
      board_copy[x][y] = board[x][y];
    }
  }
  // printf("save_board finished\n");

}

  

void undo_board(int board_copy[8][8]){
  int x,y;
  // printf("undo_board work\n");
  for(x = 0; x < 8; x++){
    for(y = 0; y < 8; y++){
      board[x][y] = board_copy[x][y];
    }
  }
}


double max_node(int turn, int depth, int max_depth,struct xy *bestmove){
  double  best;
   
  if (depth == max_depth){
  
    return eval_func();
  }
  else{
    best = -INFINITY;
  int x,y;
  int nmoves=0, nempty = 0;
  struct xy legal_moves[60];
  for( x = 0; x < 8; x++){
    for(y = 0; y < 8; y++){
       if(board[x][y] != 0) continue;
	nempty ++;
	struct xy tmp1;
	tmp1.x = x;
	tmp1.y = y;
	if( !is_legal_move(turn,tmp1)) continue;
	assert(nmoves < 60);
	if(is_legal_move(turn,tmp1)){
	legal_moves[nmoves++] = tmp1;
	}
    }
  }
   save_board(board_copy1);
   //  print_board();

  if(nempty == 0) {
    return -65;
  }
    if(nmoves == 1){
    place_disk(turn, legal_moves[0]);
   *bestmove = legal_moves[0];
     return eval_func();
    }
  if(nmoves == 0){
   return -66;
 }

 else if (nmoves > 1 && nempty != 0){
  int i;
  for(i = 0; i < nmoves; i++){
    //  printf("check starts\n");
    assert(is_legal_move(turn, legal_moves[i]));
    place_disk(turn,legal_moves[i]);
    struct xy bestmove1;
    
    double v = min_node(turn*(-1),depth + 1,max_depth, &bestmove1);
    if (v > best){
      // printf("%f\n",v);
      best = v;
      *bestmove = legal_moves[i];
    }
    undo_board(board_copy1);  
      
  }
 
  return best;
 }
  }
}	





double min_node(int turn, int depth, int max_depth,struct xy *bestmove){
  // printf("min starts\n");
  double best;
  if(depth == max_depth){
    return eval_func();
  }
  else{
  best = INFINITY;
  int x,y;
  int nmoves = 0, nempty = 0;
   struct xy legal_moves2[60];
  for( x = 0; x < 8; x++){
    for(y = 0; y < 8; y++){
       if(board[x][y] != 0) continue;
	nempty ++;
	struct xy tmp2;
	tmp2.x = x;
	tmp2.y = y;
	if( !is_legal_move(turn,tmp2)) continue;
	assert(nmoves < 60);
	if(is_legal_move(turn, tmp2)){
	legal_moves2[nmoves++] = tmp2;
	}
    }
  }
  save_board(board_copy2);
  //  print_board();

  if(nempty == 0){
    return -65;
  }
 else if(nmoves == 0) {
    return -66;
  }


 else if(nempty != 0 && nmoves != 0){
    // printf("kokomade\n");
  int i;
  // printf("nmoves = %d\n",nmoves);
  for(i = 0; i < nmoves; i++){
    // printf("lalla\n");
    assert(is_legal_move(turn, legal_moves2[i]));
    place_disk(turn,legal_moves2[i]);
    
    struct xy bestmove2;
    double v = max_node(turn*(-1), depth + 1,max_depth, &bestmove2);
    if (v < best){
      best = v;
     *bestmove = legal_moves2[i];
    }
    // printf("right\n");
    undo_board(board_copy2);

  }
 
  return best;
 }
  }
}

int pass(int turn){
  int x,y,flag;
  struct xy move;
  flag = 0;
  for(x = 0; x < 8; x ++){
    for(y = 0; y < 8; y++){
      move.x = x;
      move.y = y;
      if (is_legal_move(turn,move)){
	flag = 1;
	return 0;
	break;
      }
    }
  }
  if(flag == 0){
    return 1;
  }
	
}

void win_lose(){
  int x,y;
  int n1,n2;
  n1 = 0;
  n2 = 0;
  int result = 0;
  for(x =0; x < 8; x++){
    for(y = 0; y < 8; y ++){
      if(board[x][y] == 1){
	n1++;
	  }
      else if(board[x][y] == -1){
	n2++;
	  }
    }
  }
  result = n1 - n2;
  if(result > 0){
    printf(" O has won the game\n");
  }
  else if(result == 0){
    printf("Drew\n");
  }
  else{
    printf(" # has won this game\n");
  }
	
}

int main(int argc, char **argv)
{
 const int human_side = (argc >= 2) ? atoi(argv[1]) : 0;

  init_board();
  print_board();
  int state1 ,state2 ,state3;
  int turn;
  for (turn = 1;; turn *= -1) {
    int moves = 0;
    struct xy move;
    if (turn == human_side) {
      if(!pass(turn)){
	state1 = 0;
	moves = 1;
      do {
	char buf[1000];
	printf("Where? ");
	scanf("%s", buf);
	move.x = buf[0] - 'a';
	move.y = buf[1] - '1';
      } while (!is_legal_move(turn, move));
      }
      else{
	printf("pass\n");
	state1 = 1;
       }
    } else {
      
      const double r = max_node(turn,0,2,&move);
     
      if (r == -65) break;     // no empty square
      if (r ==  -66 ) {
	if(turn == -1){
	state2 = 1;
	printf("pass\n");
      } // pass (no legal move)
      else if (turn == 1){
	state3 = 1;
       printf("pass\n");
      }
      }
      else{
	if(turn == 1){
	  state3 = 0;
	  moves = 1;
	}
	else if(turn == -1){
	  state2 = 0;
	  moves = 1;
	}
      }
      printf("turn = %d, move = %c%c\n", turn, 'a' + move.x, '1' + move.y);
     
    }
    if(moves == 1){
    if(is_legal_move(turn,move)){
      place_disk(turn, move);
    }
    }
    print_board();
    int count = state1 +state2 +state3;
    // printf("%d %d %d\n",state1,state2,state3);
    if(count == 2){
      break;
    }
  }
 win_lose();
}
