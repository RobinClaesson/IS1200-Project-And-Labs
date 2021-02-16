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
void draw();

void printRect(struct Rectangle rect);
void printPoint(struct Point point);
void printGame();
void printGameStatus();

int rectBot(struct Rectangle rect);
int rectRight(struct Rectangle rect);

int collisionRP(struct Rectangle rect, struct Point point);
int collisionRR(struct Rectangle rect1, struct Rectangle rect2);

void moveUp(struct Rectangle* rect);
void moveDown(struct Rectangle* rect);


//Global Variables
struct Point screenSize;

struct Rectangle ball;
double ballAngle = 0;

struct Rectangle player1;
struct Rectangle player2;

int main() {
  screenSize = createPoint(128, 32); //128x32 screen size
  resetGame();

  update();
	return 0;
}

void update()
{
  ball.pos.x += cos(ballAngle);
  ball.pos.y += sin(ballAngle);


  if(collisionRR(player1, ball) || collisionRR(player2, ball)){

    ballAngle += acos(-1);

    if(ballAngle >= 2*acos(-1))
      ballAngle -= 2*acos(-1);
  }

  draw();

  char c = getchar();
  if(c =='r')
    resetGame();

  else if(c == 'w')
    moveUp(&player1);

  else if(c == 's')
    moveDown(&player1);

  else if(c == 'u')
    moveUp(&player2);

  else if(c == 'j')
    moveDown(&player2);


  //loop
  if(c != 'e')
    update();
}


void draw()
{
  //draw
  clearScreen();
  printGame();
  printGameStatus();
}


void resetGame(){
  player1 = createRect(3, screenSize.y/2 - 3, 1, 6);
  player2 = createRect(screenSize.x - 4., screenSize.y/2 - 3, 1, 6);


  ball = createRect(screenSize.x/2 - 1, screenSize.y/2 - 1, 2, 2);
  ballAngle = acos(-1);

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

int collisionRR(struct Rectangle rect1, struct Rectangle rect2){
    //If rect1 is to the right of rect2
    if(rect1.pos.x > rect2.pos.x + rect2.size.x)
      return 0;

    //if rect1 is to the left of rect2
    else if(rect2.pos.x > rect1.pos.x + rect1.size.x)
      return 0;

    //if rect1 is above rect2
    else if(rect1.pos.y + rect1.size.y < rect2.pos.y)
      return 0;

    //if rect 1 is below rect2
    else if(rect1.pos.y >  rect2.pos.y + rect2.size.y)
      return 0;

    return 1;
}

void moveUp(struct Rectangle* rect)
{
  if(rect->pos.y > 0)
    rect->pos.y--;
}

void moveDown(struct Rectangle* rect)
{
  if(rect->pos.y + rect->size.y < screenSize.y)
    rect->pos.y++;
}

int rectBot(struct Rectangle rect)
{
  return rect.pos.y + rect.size.y;
}


//Print to terminal functions (To be removed in final product)
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

   printf("\nPlayer 2: ");
   printRect(player2);

   printf("\nBall: ");
   printRect(ball);
   printf("\nBallangle: %.3f\n", ballAngle);
}
