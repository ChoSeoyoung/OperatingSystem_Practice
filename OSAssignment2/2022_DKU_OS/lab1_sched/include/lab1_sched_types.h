/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32204298
*	    Student name : 조서영
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H

/*
 * You need to Declare functions in  here
 */
typedef struct process{
    int number; //process 번호
    char name; //process 이름
    int arrive_time; //process 도착시간
    int service_time; //process 총 실행시간
}PROCESS;
typedef struct queue{
    int front; //큐의 앞, 새로운 원소 삽입
    int rear; //큐의 뒤, 원소 pop
    int time_quantum;
    int *p; //Process의 주소
}QUEUE;

//Queue Fucntions
void enqueue(QUEUE *q, int newP);
int dequeue(QUEUE *q);
//General Fucntions
int total_service_time(PROCESS process_list[], int size);
void sort_service_time(PROCESS process_list[], QUEUE *Q);
void sort_high_response_time(PROCESS process_list[], QUEUE *Q, int current);

//Scheduling Algorithms
void FCFS(PROCESS process_list[], int size,int process_arrive_time[]);
void RR(PROCESS process_list[], int size,int process_arrive_time[], int time_slice);
void SPN(PROCESS process_list[], int size,int process_arrive_time[]);
void HRRN(PROCESS process_list[], int size,int process_arrive_time[]);
void FeedBack(PROCESS process_list[], int size,int process_arrive_time[],int time_quantum);

#endif /* LAB1_HEADER_H*/



