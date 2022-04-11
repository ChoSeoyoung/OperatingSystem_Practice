/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32204298
*	    Student name : 조서영
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>
#define SIZE 5

#include "lab1_sched_types.h"

/*
 * you need to implement scheduler simlator test code.
 *
 */
PROCESS process_list[SIZE]={{0,'A',0,3},{1,'B',2,6},{2,'C',4,4},{3,'D',6,5},{4,'E',8,2}};
int process_arrive_time[20]={0,-1,1,-1,2,-1,3,-1,4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
//Queue Q={-1,4,0,arr};

int main(int argc, char *argv[]){
    FCFS(process_list, SIZE, process_arrive_time);
    RR(process_list, SIZE, process_arrive_time,1);
    RR(process_list, SIZE, process_arrive_time,4);
    SPN(process_list, SIZE, process_arrive_time);
    HRRN(process_list, SIZE, process_arrive_time);
    FeedBack(process_list, SIZE, process_arrive_time,1);
    FeedBack(process_list, SIZE, process_arrive_time,2);
    //void FeedBack_q1();
    //void FeedBack_q2();
}

