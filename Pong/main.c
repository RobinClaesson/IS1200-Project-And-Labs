#include <pic32mx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265359

//-----------------------------------------------
//Structures
//-----------------------------------------------
struct Point{
  double x;
  double y;
};

struct Rectangle{
  struct Point pos;
  struct Point size;
};

//-----------------------------------------------
//Predefining functions
//-----------------------------------------------
struct Point createPoint(int x, int y);
struct Rectangle createRect(int x, int y, int width, int height);

void resetGame();
void game_init();

void update();
void update_ball();
void update_menu();
void update_highscore();
void update_player1();
void update_player2();

void draw();
void display_rectangle(struct Rectangle rect);
void timer_init();

void menu_up();
void menu_down();

int rectBot(struct Rectangle rect);
int rectRight(struct Rectangle rect);
struct Point rectCenter(struct Rectangle rect);

int collisionRP(struct Rectangle rect, struct Point point);
int collisionRR(struct Rectangle rect1, struct Rectangle rect2);

void moveUp(struct Rectangle* rect);
void moveDown(struct Rectangle* rect);

void setBallAngle(double angle);
void ballPaddleAngle(struct Rectangle rect);

//-----------------------------------------------
//Global Variables
//-----------------------------------------------
struct Point screenSize;

struct Rectangle ball;
double ballAngle = 0;

struct Rectangle player1;
struct Rectangle player2;

enum GameState{VsHuman, VsAI, HighScore, Menu}gameState, menuState;
char* menuText[3] = {"PvP", "PvE", "HighScores"};
char* menuSelect = "<-";


//-----------------------------------------------
//Main / Init / Resets
//-----------------------------------------------
int main(void) {

  SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
  SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
  while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
  OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
  while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
  SYSKEY = 0x0;  /* Lock OSCCON */

  display_init();
  game_init();
  timer_init();

  while(1);

  return 0;
}

void resetGame(){
  player1 = createRect(3, screenSize.y/2 - 3, 1, 6);
  player2 = createRect(screenSize.x - 4., screenSize.y/2 - 3, 1, 6);

  ball = createRect(screenSize.x/2 - 1, screenSize.y/2 - 1, 2, 2);
  ballAngle = PI;
}

void game_init(){
  screenSize = createPoint(127, 31); //128x32 screen size
  gameState = VsHuman;

  resetGame();
}

void timer_init(){
  // timer 2 clear
  T2CON = 0x0;

  // set the prescale to 256
  T2CONSET = 0x7 << 4;

  // set period register to ~60hz
  PR2 = 5208;

  // set as ON
  T2CONSET = 1 << 15;

  // enable interrupt for timer 2
  IEC(0) |= (1 << 8);

  // set priority for timer 2
  IPC(2) |= 4;

  enable_interrupt();
}


//-----------------------------------------------
//Update functions
//-----------------------------------------------
void user_isr(){

  if (IFS(0) & 0x100){
    //reset interrupt flag
    IFS(0) &= 0xfffffeff;
    update_input();



    switch(gameState){

      case VsHuman:
      update_ball();
      update_player1();
      update_player2();

      break;

      case VsAI:
      update_ball();
      break;

      case HighScore:
      update_highscore();
      break;

      default:
      case Menu:
      update_menu();
      break;
    }

    draw();
  }

}

void update_ball(){
 ball.pos.x += cos(ballAngle);
  ball.pos.y += sin(ballAngle);


  //Ball player1 collision
  if(collisionRR(player1, ball)){
      ballPaddleAngle(player1);
      ball.pos.x  = rectRight(player1);
  }

  //Ball player2 collision
  else if(collisionRR(player2, ball)){
      ballPaddleAngle(player2);
      ball.pos.x = player2.pos.x - ball.size.x;
  }

  //Ball bot/top collisionRP
  if(rectBot(ball) >= screenSize.y)
  {
    setBallAngle(-ballAngle);
    ball.pos.y = screenSize.y - ball.size.y;
  }

  else if(ball.pos.y <= 0)
  {
    setBallAngle(-ballAngle);
    ball.pos.y = 0;
  }


  //Someone scores
  if(ball.pos.x < 0 || rectRight(ball) > screenSize.x)
    resetGame();
}

void update_player1 (){
  if (btn4_down()){
    moveUp(&player1);
  }

  if (btn3_down()){
    moveDown(&player1);
  }
}

void update_player2 (){
  if (btn2_down()){
    moveUp(&player2);
  }

  if (btn1_down()){
    moveDown(&player2);
  }
}

void update_menu(){



}

void update_highscore(){

}

//-----------------------------------------------
//Draw - Functions
//-----------------------------------------------

//Main Draw function
void draw(){
  clear_buffer();

  switch(gameState){
    case VsHuman:
    case VsAI:

      display_rectangle(player1);
      display_rectangle(player2);
      display_rectangle(ball);

    break;

    case HighScore:

    break;

    default:
    case Menu:

    break;
  }

  display_update();
}

//Draw a filled rectangle
void display_rectangle(struct Rectangle rect){
  int i, j;

  for (i = 0; i < rect.size.x; i++){
    for (j = 0; j < rect.size.y; j++){
      display_pixel((int)rect.pos.x + i, (int)rect.pos.y + j);
    }
  }
}


//-----------------------------------------------
// Point and Rectangle Helpfunctions
//-----------------------------------------------

//Returns Point at (x,y)
struct Point createPoint(int x, int y){
  struct Point p;
  p.x = x;
  p.y = y;
  return p;
}

//Returns Rectangle at (x,y) with size (width, heigth)
struct Rectangle createRect(int x, int y, int width, int height){
  struct Rectangle rect;
  rect.pos = createPoint(x, y);
  rect.size = createPoint(width, height);

  return rect;
}

//Return 1 if Point and Rectangle intersect
int collisionRP(struct Rectangle rect, struct Point point){
  if(point.x >= rect.pos.x && point.x < rect.pos.x + rect.size.x &&
     point.y >= rect.pos.y && point.y < rect.pos.y + rect.size.y)
      return 1;

  else return 0;
}

//Return 1 if Rectangle and Rectangle intersect
int collisionRR(struct Rectangle rect1, struct Rectangle rect2){
    //If rect1 is to the right of rect2
    if(rect1.pos.x > rectRight(rect2))
      return 0;

    //if rect1 is to the left of rect2
    else if(rect2.pos.x > rectRight(rect1))
      return 0;

    //if rect1 is above rect2
    else if(rectBot(rect1) < rect2.pos.y)
      return 0;

    //if rect 1 is below rect2
    else if(rect1.pos.y >  rectBot(rect2))
      return 0;

    return 1;
}

//Returns the y coardnate for the bottom of the rectangle
int rectBot(struct Rectangle rect)
{
  return rect.pos.y + rect.size.y;
}

//Recturns the x coardinate for the right of the rectangle
int rectRight(struct Rectangle rect)
{
  return rect.pos.x + rect.size.x;
}

//Returns the center of the rect
struct Point rectCenter(struct Rectangle rect)
{
    struct Point center;
    center.x = (rect.pos.x + rectRight(rect))/2;
    center.y = (rect.pos.y + rectBot(rect))/2;
    return center;
}

//Moves a rectangle up without exiting the top of the screen
void moveUp(struct Rectangle* rect)
{
  if(rect->pos.y > 0)
    rect->pos.y--;
}

//Moves a rectangle down without exithing the bottom of the screen
void moveDown(struct Rectangle* rect)
{
  if(rect->pos.y + rect->size.y < screenSize.y)
    rect->pos.y++;
}



//-----------------------------------------------
// Bal angle functions
//-----------------------------------------------

//Sets the ball angle to the corresponding angle between 0 and 2 PI
void setBallAngle(double angle){
    ballAngle = angle;

    if(ballAngle > 2*PI)
      ballAngle -= 2*PI;
    else if(ballAngle < 0)
      ballAngle += 2*PI;
}

//Calculates the angle of the ball when colliding with a paddle
void ballPaddleAngle(struct Rectangle player){
  setBallAngle(PI - ballAngle);

  double dist = rectCenter(player).y - rectCenter(ball).y;
  double offset = (PI/3)*(dist/player.size.y); //Max offset * percentage distance from middle

  //The offset have different sign for the two players
  if(ball.pos.x > player.pos.x)
    offset *= -1;

  setBallAngle(ballAngle + offset);
}

//-----------------------------------------------
// Menu Functions
//-----------------------------------------------
void menu_up()
{
  switch (menuState)
  {

    case VsHuman:
      menuState = HighScore;
      break;

    default:
    case VsAI:
      menuState = VsHuman;
      break;

    case HighScore:
      menuState = VsAI;
      break;

  }
}

void menu_down()
{
  switch (menuState)
  {
      case VsHuman:
      menuState = VsAI;
      break;

    case VsAI:
      menuState = HighScore;
      break;

    default:
    case HighScore:
      menuState = VsHuman;
      break;
  }
}
