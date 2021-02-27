#include <pic32mx.h>
#include <stdlib.h>


struct Node{

    char* name;
    int score;
    struct Node* next;

}heads[3];


void add_highscore(char* name, int seconds, int difficulty)
{
    struct Node head = heads[difficulty];
    if(head == NULL)
    {

    }
}
