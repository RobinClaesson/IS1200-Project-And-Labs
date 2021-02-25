#include <pic32mx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
  display_init();
  clear_buffer();
  int i;

  for (i = 0; i < 128; i++){
    display_pixel(i, 0);
  }

  display_update();

void display_rectangle(struct Rectangle rect){
  int i, j;

  for (i = 0; i <= rect.size.x; i++){
    for (j = 0; j <= rect.size.y; j++){
      display_pixel(rect.pos.x + i, rect.pos.y + j);
    }
  }
}

  /*
  screenSize = createPoint(128, 32); //128x32 screen size
  resetGame();

  update();*/
	return 0;
}
