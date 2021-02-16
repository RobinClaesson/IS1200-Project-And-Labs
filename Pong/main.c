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
void drawPoint(struct Point point);
vid drawRectagle(struct Rectangle rect);


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

//Game Update function
void update()
{
  ball.pos.x += cos(ballAngle);
  ball.pos.y += sin(ballAngle);


  if(collisionRR(player1, ball) || collisionRR(player2, ball)){

    ballAngle += acos(-1);

    if(ballAngle >= 2*acos(-1))
      ballAngle -= 2*acos(-1);
  }

  //draw
  draw();

}

//Main draw function
void draw()
{
  //Add drawing logic here
}

//Resets game to startstate
void resetGame(){
  player1 = createRect(3, screenSize.y/2 - 3, 1, 6);
  player2 = createRect(screenSize.x - 4., screenSize.y/2 - 3, 1, 6);


  ball = createRect(screenSize.x/2 - 1, screenSize.y/2 - 1, 2, 2);
  ballAngle = acos(-1);

}

//Returns a point at the (x,y) coardinate
struct Point createPoint(int x, int y){
  struct Point p;
  p.x = x;
  p.y = y;
  return p;
}

//Returns a rectangle at (x,y) coardinate with size (width, height)
struct Rectangle createRect(int x, int y, int width, int height){
  struct Rectangle rect;
  rect.pos = createPoint(x, y);
  rect.size = createPoint(width, height);

  return rect;
}

//Returns true if point is inside rect
int collisionRP(struct Rectangle rect, struct Point point){
  if(point.x >= rect.pos.x && point.x < rect.pos.x + rect.size.x &&
     point.y >= rect.pos.y && point.y < rect.pos.y + rect.size.y)
      return 1;

  else return 0;
}

//Returns true if rect1 and rect2 is overlapping anyhwere
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

//Moves the rectanle up 1 pixel
void moveUp(struct Rectangle* rect)
{
  if(rect->pos.y > 0)
    rect->pos.y--;
}
//Moves the rectanle down 1 pixel
void moveDown(struct Rectangle* rect)
{
  if(rect->pos.y + rect->size.y < screenSize.y)
    rect->pos.y++;
}

//Turns the pixel on at coardinate (x,y)
void drawPoint(struct Point point)
{

}

//Turns all the pixels on inside the rectangle
vid drawRectagle(struct Rectangle rect)
{

}
