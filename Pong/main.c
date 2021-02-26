#include <pic32mx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286

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
//Structure functions
struct Point createPoint(int x, int y);
struct Rectangle createRect(int x, int y, int width, int height);
int rectBot(struct Rectangle rect);
int rectRight(struct Rectangle rect);
struct Point rectCenter(struct Rectangle rect);

//Collision
int collisionRP(struct Rectangle rect, struct Point point);
int collisionRR(struct Rectangle rect1, struct Rectangle rect2);

//Resets
void resetGame();
void resetBall();
void resetPlayers();

//Inits
void game_init();
void timer_init();

//Update
void update();

//Ball updates
void update_ball();
void setBallAngle(double angle);
void ballPaddleAngle(struct Rectangle rect);

//Player updates
void update_player1();
void update_player2();
void update_AI();

void moveUp(struct Rectangle* rect);
void moveDown(struct Rectangle* rect);
void player_score(int* player_score);

//Graphics
void draw();
void display_rectangle(struct Rectangle rect);
void display_menu();
void display_chooseDiff();

//Menu
void update_menu();
void menu_up();
void menu_down();

void update_highscore();
void update_chooseDiff();

//-----------------------------------------------
//Global Variables
//-----------------------------------------------
struct Point screenSize;

struct Rectangle ball;
double ballAngle = 0;

struct Rectangle player1;
struct Rectangle player2;

enum GameState{VsHuman, VsAI, HighScore, Menu, ChooseDiff}gameState, menuState;


int score_p1, score_p2;

int ai_diff = 0, ai_tick = 0;

//-----------------------------------------------
//Main / Init / Resets
//-----------------------------------------------
int main(void) {

  /* FROM LAB 3
This will set the peripheral bus clock to the same frequency
as the sysclock. That means 80 MHz, when the microcontroller
is running at 80 MHz. Changed 2017, as recommended by Axel.
*/
  SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
  SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
  while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
  OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
  while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
  SYSKEY = 0x0;  /* Lock OSCCON */

  display_init();
  game_init();
  timer_init();
  led_init();

  while(1);

  return 0;
}

void resetGame(){
  resetPlayers();
  resetBall();

  score_p1 = 0;
  score_p2 = 0;
}

void resetBall(){
  ball = createRect(screenSize.x/2 - 1, screenSize.y/2 - 1, 2, 2);
  ballAngle = PI;
}

void resetPlayers()
{
  player1 = createRect(3, screenSize.y/2 - 3, 1, 6);
  player2 = createRect(screenSize.x - 4., screenSize.y/2 - 3, 1, 6);
}

void game_init(){
  screenSize = createPoint(128, 32); //128x32 screen size
  gameState = Menu;
  menuState = VsHuman;

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
void update(){

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
      update_player1();
      update_AI();
      break;

      case HighScore:
      update_highscore();
      break;

      default:
      case Menu:
      update_menu();
      break;

      case ChooseDiff:
      update_chooseDiff();
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
  if(ball.pos.x < 0)
    player_score(&score_p1);
  else if(rectRight(ball) > screenSize.x)
    player_score(&score_p2);
}

void player_score(int* score)
{
    resetBall();
    resetPlayers();

    (*score)++;

    if((*score) > 3)
      resetGame();
}

//Control player 1
void update_player1 (){
  if (btn4_down()){
    moveUp(&player1);
  }

  if (btn3_down()){
    moveDown(&player1);
  }
}

//Control player 2
void update_player2 (){
  if (btn2_down()){
    moveUp(&player2);
  }

  if (btn1_down()){
    moveDown(&player2);
  }
}

void update_AI(){

  ai_tick++;
  if(ai_tick >= 4 - ai_diff)
  {
    ai_tick = 0;
    double dist = rectCenter(player2).y - rectCenter(ball).y;
    if(dist > player2.size.y/2)
      moveUp(&player2);
    else if(dist < -player2.size.y/2)
      moveDown(&player2);
  }

}

//Update menu
void update_menu(){

  if(btn4_pressed())
    menu_up();
  else if(btn3_pressed())
    menu_down();
  else if(btn1_pressed())
  {
    gameState = menuState;
  }
}


void update_chooseDiff(){
  if(btn4_pressed()){
    ai_diff--;
    if( ai_diff < 0)
      ai_diff = 3;
  }

  else if(btn3_pressed()){
    ai_diff++;
    if( ai_diff > 3)
      ai_diff = 0;
  }

  else if(btn1_pressed())
  {
    gameState = VsAI;
  }

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

    case Menu:
      display_menu();
    break;

    case ChooseDiff:
    display_chooseDiff();
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

//Displays the menu on the screen
void display_menu()
{
  display_string(0, ("ABCD"));
  display_pixel(31, 31);

  display_update();
/*  //PvP
  if(menuState == 0)
    display_string(0, "PvP <--");
  else
    display_string(0, "PvP");

  //PvE
  if(menuState == 1)
    display_string(1, "PvE <--");
  else
    display_string(1, "PvE");

  //Highscore
  if(menuState == 2)
    display_string(3, "HighScores <--");
  else
    display_string(3, "HighScores");
*/
}

void display_chooseDiff()
{
   //PvP
    if(ai_diff == 0)
      display_string(0, "Easy <--");
    else
      display_string(0, "Easy");

    if(ai_diff == 1)
      display_string(1, "Medium <--");
    else
      display_string(1, "Medium");

    if(ai_diff == 2)
      display_string(2, "Hard <--");
    else
      display_string(2, "Hard");

    if(ai_diff == 3)
      display_string(3, "Impossible <--");
    else
      display_string(3, "Impossible");

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


  //Contstrains angle to not be to vertical
  if(ballAngle > PI/3 && ballAngle <= PI/2)
    ballAngle = PI/3;
  else if(ballAngle < 2*PI/3 && ballAngle > PI/2)
    ballAngle = 2*PI/3;
  else if(ballAngle > 4*PI/3 && ballAngle <= 3*PI/2)
    ballAngle = 4*PI/3;
  else if(ballAngle < 5*PI/3 && ballAngle > 3*PI/2)
      ballAngle = 5*PI/3;

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
    case ChooseDiff:
      menuState = VsHuman;
      break;

    case HighScore:
      menuState = ChooseDiff;
      break;

  }
}

void menu_down()
{
  switch (menuState)
  {
      case VsHuman:
      menuState = ChooseDiff;
      break;

    case ChooseDiff:
      menuState = HighScore;
      break;

    default:
    case HighScore:
      menuState = VsHuman;
      break;
  }
}
