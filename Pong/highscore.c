#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Highscore names
char* names[3][9] = {{"---", "---", "---", "---", "---", "---", "---", "---", "---",},
                     {"---", "---", "---", "---", "---", "---", "---", "---", "---",},
                     {"---", "---", "---", "---", "---", "---", "---", "---", "---",}};

//Highscore results
int scores[3][9] = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0, 0}};

//Ads a HighScore to the lists
void add_highscore(char* name, int score, int difficulty)
{
    int i, j;

    for(i = 0; i < 9; i++)
    {
      //Searches from the top for the first score we beat
      if(score > scores[difficulty][i])
      {
          //Starts at the bottom and goes to the found index
          //coying the score and name of the next person on the list
          for(j = 8; j > i; j--)
          {
            names[difficulty][j] = names[difficulty][j-1];
            scores[difficulty][j] = scores[difficulty][j-1];
          }

          //Places the new score
          names[difficulty][i] = name;
          scores[difficulty][i] = score;

          break;
      }
    }
}

//Returns name of highscore holder
char* get_name(int difficulty, int index)
{
  return names[difficulty][index];
}
//Returns score of highscore holder
int get_score(int difficulty, int index)
{
  return scores[difficulty][index];
}


char buff[13] =  {'i',':',' ','a','a','a',' ', '-', '-','-', '-', '-', 0x00};
//Returns a string with the placing, name and score of a higchscore
char* get_highscore(int difficulty, int index)
{
    int i;
    buff[0] = (char)(index + 49);
    buff[3] = names[difficulty][index][0];
    buff[4] = names[difficulty][index][1];
    buff[5] = names[difficulty][index][2];

    int score = scores[difficulty][index];

    buff[7] = (char)((score/10000) + 48);
    score %= 10000;

    buff[8] = (char)((score/1000) + 48);
    score %= 1000;

    buff[9] = (char)((score/100) + 48);
    score %= 100;

    buff[10] = (char)((score/10) + 48);
    score %= 10;

    buff[11] = (char)((score) + 48);

    return buff;
}
