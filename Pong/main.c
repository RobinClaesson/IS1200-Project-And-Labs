//#include <pic32mx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define clearScreen() printf("\033[H\033[J")

//structures
struct Point{
  double x;
  double y;
};

struct Rectangle{
  struct Point pos;
  struct Point size;
};

//Predefining functions
struct Point createPoint(int x, int y);
struct Rectangle createRect(int x, int y, int width, int height);

void resetGame();
void update();

void printRect(struct Rectangle rect);
void printPoint(struct Point point);
void printGame();
void printGameStatus();

int collisionRP(struct Rectangle rect, struct Point point);
int collisionRR(struct Rectangle rect1, struct Point rect2);

//Global Variables
struct Point screenSize;

struct Rectangle ball;
double ballAngle = 0;

struct Rectangle player1;
struct Rectangle player2;

int main() {
  srand(time(NULL));   // Initialization, should only be called once.
  resetGame();

  update();
	return 0;
}

void update()
{
  ball.pos.x += cos(ballAngle);
  ball.pos.y += sin(ballAngle);

  //ball.pos.x += -1;
  //ball.pos.y += -1;

  //draw
  clearScreen();
  printGame();
  printGameStatus();

  //loop
  char c = getchar();
  if(c =='r')
    resetGame();
    
  update();
}

void resetGame(){

  screenSize = createPoint(128, 32); //128x32 screen size

  player1 = createRect(1, screenSize.y/2 - 3, 1, 6);
  player2 = createRect(screenSize.x - 2., screenSize.y/2 - 3, 1, 6);


  ball = createRect(screenSize.x/2 - 1, screenSize.y/2 - 1, 2, 2);
  ballAngle = (double)(rand() % 628) / 100;

}

struct Point createPoint(int x, int y){
  struct Point p;
  p.x = x;
  p.y = y;
  return p;
}

struct Rectangle createRect(int x, int y, int width, int height){
  struct Rectangle rect;
  rect.pos = createPoint(x, y);
  rect.size = createPoint(width, height);

  return rect;
}

int collisionRP(struct Rectangle rect, struct Point point){
  if(point.x >= rect.pos.x && point.x < rect.pos.x + rect.size.x &&
     point.y >= rect.pos.y && point.y < rect.pos.y + rect.size.y)
      return 1;

  else return 0;
}

int collisionRR(struct Rectangle rect1, struct Point rect2)
{
  return 0;
}



void printPoint(struct Point point)
{
  printf("x=%.2f | y=%.2f", point.x, point.y);
}

void printRect(struct Rectangle rect)
{
  printf("x=%.2f | y=%.2f | w=%.2f | h=%.2f", rect.pos.x, rect.pos.y, rect.size.x, rect.size.y);
}

void printGame()
{

  struct Point pos;

  for(int y = -1; y <= screenSize.y; y++)
  {
    for(int x = -1; x <= screenSize.x; x++)
    {
        pos.x = x;
        pos.y = y;

        if(x == -1 || x == screenSize.x)
          printf("|");

        else if(y == -1 || y == screenSize.y)
          printf("-");

        else if(collisionRP(ball, pos) || collisionRP(player1, pos) || collisionRP(player2, pos))
          printf("#");

        else
          printf(" ");
    }

    printf("\n");
  }
}

void printGameStatus()
{
   printf("Player 1: ");
   printRect(player1);

   printf("\nPlayer2: ");
   printRect(player2);

   printf("\nBall: ");
   printRect(ball);
   printf("\nBallangle: %.3f\n", ballAngle);
}
