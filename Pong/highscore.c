#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* names[3][9] = {{"---", "---", "---", "---", "---", "---", "---", "---", "---",},
                     {"---", "---", "---", "---", "---", "---", "---", "---", "---",},
                     {"---", "---", "---", "---", "---", "---", "---", "---", "---",}};


int scores[3][9] = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0, 0}};

//Ads a HighScore to the lists
void add_highscore(char* name, int score, int difficulty)
{
    int i, j;

    for(i = 0; i < 9; i++)
    {
      if(score > scores[difficulty][i])
      {
          for(j = 8; j > i; j--)
          {
            names[difficulty][j] = names[difficulty][j-1];
            scores[difficulty][j] = scores[difficulty][j-1];
          }

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

char buff[14] =  {'i',':',' ','a','a','a',' ','-', '-', '-','-', '-', '-', 0x00};
char* get_highscore(int difficulty, int index)
{
    int i;
    buff[0] = (char)(index + 49);
    buff[3] = names[difficulty][0][0];
    buff[4] = names[difficulty][0][1];
    buff[5] = names[difficulty][0][2];

    int score = scores[difficulty][index];

    if(score / 100000)
      buff[7] = (char)((score/100000) + 48);
    else
      buff[7] = '0';

    score %= 100000;

    if(score / 10000)
      buff[8] = (char)((score/10000) + 48);
    else
      buff[8] = '0';

    score %= 10000;

    if(score / 1000)
        buff[9] = (char)((score/1000) + 48);
      else
        buff[9] = '0';

    score %= 1000;

    if(score / 100)
        buff[10] = (char)((score/100) + 48);
      else
        buff[10] = '0';

    score %= 100;

    if(score / 10)
        buff[11] = (char)((score/10) + 48);
      else
        buff[11] = '0';

    score %= 10;


    buff[12] = (char)((score) + 48);



    return buff;
}

/*int main()
{

    add_highscore("roc", 100, 0);
    add_highscore("aaa", 90, 0);
    add_highscore("aba", 95, 0);
    add_highscore("abc", 90, 0);
    add_highscore("knu", 123456, 0);

    for(int i = 0; i < 9; i++)
      printf("N: %s | S: %d\n", names[0][i], scores[0][i]);

    printf("På plats 3: %s, %d\n", get_name(0, 3), get_score(0,3));

    printf("%s\n", get_highscore(0, 0));

  return 0;
}*/

/*int main()
{
   int i;
   char buf[12];

   for (i = 0; i < 100; i++) {
      snprintf(buf, 12, "pre_%d_suff", i); // puts string into buffer
      printf("%s\n", buf); // outputs so you can see it
   }
}*/
