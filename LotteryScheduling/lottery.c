#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct process{
    char name;
    int ticket;
    struct process *next;
}PROCESS;

int main(int argc, char *argv[]){
    //PROCESS A, PROCESS B 정의
    PROCESS A;
    PROCESS B;
    A.name='A';
    A.ticket=75;
    A.next=&B;
    B.name='B';
    B.ticket=25;
    srand((unsigned)time(NULL));
    
    for(int i=0;i<100;i++){
        //counter: used to track if we've found the winner yet
        int counter=0;

        //winner: use some call to a random number generator to get a Value, between 0 and the total of tickets(100)
        int winner=rand()%100;

        //current: use this to walk thorugh the list of jobs
        PROCESS *current = &A;
    
        //loop until the sum of ticket values is > the winner
        while(current){
            counter=counter+current->ticket;
            if(counter>winner){
                printf("%c ",current->name);
                break;//found the winner;
            }
            current=current->next;
        }
    }

    printf("\n");
}