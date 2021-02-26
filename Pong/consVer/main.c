//#include <pic32mx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define clearScreen() printf("\033[H\033[J")

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
struct Point createPoint(int x, int y);
struct Rectangle createRect(int x, int y, int width, int height);

void resetGame();
void resetPlayers();
void game_init();

void update();
void update_ball();
void update_menu();
void update_chooseDiff();
void update_highscore();
void update_player1();
void update_player2();
void update_AI();
void player_score(int* player_score);
void resetBall();
void display_menu();

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

void printGame();
void printGameStatus();
void update_input();
void print_menu();
void print_chooseDiff();

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

char input;
double debug = 0;
//-----------------------------------------------
//Main / Init / Resets
//-----------------------------------------------
int main(void) {

  game_init();


  update();
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



//-----------------------------------------------
//Update functions
//-----------------------------------------------
void update(){

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
    update_input();
    update();
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
    player_score(&score_p2);
  else if(rectRight(ball) > screenSize.x)
    player_score(&score_p1);
}

void player_score(int* player_score)
{
    resetBall();
    resetPlayers();

    //display_score(score_p1, score_p2);

    (*player_score)++;


    if((*player_score) > 3)
      resetGame();
}

//Control player 1
void update_player1 (){
  if (input == 'w'){
    moveUp(&player1);
  }

  if (input == 's'){
    moveDown(&player1);
 }
}

//Control player 2
void update_player2 (){
  if (input == 'u'){
    moveUp(&player2);
  }

  if (input == 'j'){
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

  if(input == 'w')
    menu_up();
  else if(input == 's')
    menu_down();
  else if(input == 'd')
  {
    gameState = menuState;
  }


}

void update_chooseDiff(){
  if(input == 'w'){
    ai_diff--;
    if( ai_diff < 0)
      ai_diff = 3;
  }

  else if(input == 's'){
    ai_diff++;
    if( ai_diff > 3)
      ai_diff = 0;
  }

  else if(input == 'd')
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
  clearScreen();
  switch(gameState){
    case VsHuman:
    case VsAI:


    printGame();
    printGameStatus();

    break;

    case Menu:
    print_menu();
    break;

    case ChooseDiff:
    print_chooseDiff();
    break;
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


//-----------------------------------------------
// Terminal functions for the console version
//-----------------------------------------------
void printPoint(struct Point point){
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

        if(x == -1)
          printf("|");

        else if(x == screenSize.x)
          printf("|%d", y);

        else if(y == -1 || y == screenSize.y)
          printf("-");

        else if(collisionRP(ball, pos))
          printf("O");

        else if(collisionRP(player1, pos) || collisionRP(player2, pos))
          printf("#");

        else
          printf(" ");
    }

    printf("\n");
  }
}

void printGameStatus()
{
   /*printf("Player 1: ");
   printRect(player1);

   printf("\nPlayer 2: ");
   printRect(player2);*/

   printf("\nBall: ");
   printRect(ball);
   printf("\nBallangle: %.3f\n", ballAngle);

   printf("AI: %d\n", ai_diff);
   printf("Player1 %d - %d player2\n", score_p1, score_p2);

   printf("\nDebug: %.2f\n", debug);
}

void update_input()
{
  scanf("%c", &input);
}

void print_menu()
{
  if(menuState == 0)
    printf("PvP <--\n");
  else
    printf("PvP\n");

  //PvE
  if(menuState == 4)
    printf("PvE <--\n");
  else
    printf("PvE\n");

  //Highscore
  if(menuState == 2)
    printf("HighScores <--\n");
  else
    printf("HighScores\n");

}

void print_chooseDiff()
{
    if(ai_diff == 0)
      printf("Easy <--\n");
    else
      printf("Easy\n");

      if(ai_diff == 1)
        printf("Medium <--\n");
      else
        printf("Medium\n");

      if(ai_diff == 2)
        printf("Hard <--\n");
      else
        printf("Hard\n");

      if(ai_diff == 3)
          printf("Impossible <--\n");
      else
          printf("Impossible\n");
}
