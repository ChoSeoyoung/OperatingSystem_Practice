/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32204298
*	    Student name : 조서영
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
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

#include "lab1_sched_types.h"

/*
 * you need to implement FCFS, RR, SPN, HRRN, MLFQ scheduler. 
 */
//General Functions
void enqueue(QUEUE *q, int newP){
    (*q).rear+=1;
    (*q).p[(*q).rear]=newP;
}
int dequeue(QUEUE *q){
    (*q).front+=1;
    return (*q).p[(*q).front];
}
int total_service_time(PROCESS process_list[], int size){
    int result=0;
    for(int i=0;i<size;i++){
        result+=process_list[i].service_time;
    }
    return result;
}
void sort_service_time(PROCESS process_list[], QUEUE *Q){
    int tmp;
    for(int i=(*Q).front+1;i<(*Q).rear;i++){
        for(int j=i+1;j<(*Q).rear+1;j++){
            if(process_list[(*Q).p[i]].service_time>process_list[(*Q).p[j]].service_time){
                tmp=(*Q).p[i];
                (*Q).p[i]=(*Q).p[j];
                (*Q).p[j]=tmp;
            }
        }
    }
}
void sort_high_response_time(PROCESS process_list[], QUEUE *Q, int current){
    int tmp;
    for(int i=(*Q).front+1;i<(*Q).rear;i++){
        float tmp_i=(current-process_list[(*Q).p[i]].arrive_time+process_list[(*Q).p[i]].service_time)/(process_list[(*Q).p[i]].service_time);
        for(int j=i+1;j<(*Q).rear+1;j++){
            float tmp_j=(process_list[(*Q).p[j]].arrive_time-current+process_list[(*Q).p[i]].service_time)/(process_list[(*Q).p[j]].service_time);
            if(tmp_i<tmp_j){
                tmp=(*Q).p[i];
                (*Q).p[i]=(*Q).p[j];
                (*Q).p[j]=tmp;
            }
        }
    }
}

//Scheduling Algorithms
void FCFS(PROCESS process_list[], int size,int process_arrive_time[]){
    printf("FCFS: ");
    //모든 process가 수행되는데 걸리는 시간
    int time=total_service_time(process_list, size);
    
    //tmp 동적할당
    int *arr = (int *)malloc(size*sizeof(int));
    QUEUE readyQ={-1,-1,0,arr};


    for(int i=0;i<time;i++){
        int idx=process_arrive_time[i];
        if(idx!=-1){
            enqueue(&readyQ,idx);
        }
    }

    int s=readyQ.rear-readyQ.front;
    for(int i=0;i<s;i++){
        int cur=dequeue(&readyQ);
        for(int i=0;i<process_list[cur].service_time;i++){
            printf("%c ",process_list[cur].name);
        }
    }
    printf("\n");

    //동적할당 해제
    free(arr);
}

void RR(PROCESS process_list[], int size,int process_arrive_time[], int time_slice){
    printf("RR(tq=%d): ",time_slice);
    
    //process_list의 복사본을 만듦
    PROCESS *process_list_copy=(PROCESS *)malloc(size*sizeof(PROCESS));
    for(int i=0;i<size;i++){
        process_list_copy[i].number=process_list[i].number;
        process_list_copy[i].name=process_list[i].name;
        process_list_copy[i].arrive_time=process_list[i].arrive_time;
        process_list_copy[i].service_time=process_list[i].service_time;
    }

    //모든 process가 수행되는데 걸리는 시간
    int time=total_service_time(process_list, size);

    //tmp 동적할당
    int *arr = (int *)malloc(time*sizeof(int));
    QUEUE readyQ={-1,-1,0,arr};

    int cur=-1;
    int start_time=0;
    for(int i=0;i<time;i++){
        int idx=process_arrive_time[i];
        
        if(idx!=-1){
            enqueue(&readyQ,idx);
        }

        if((cur!=-1)&&(process_list_copy[cur].service_time!=0)){
            enqueue(&readyQ,cur);
        }

        if(start_time<=i){
            cur=dequeue(&readyQ);
            for(int j=0;j<time_slice;j++){
                printf("%c ",process_list[cur].name);
                process_list_copy[cur].service_time-=1;
                start_time+=1;
                if(process_list_copy[cur].service_time==0)
                    break;
            }
        }
    }
    printf("\n");

    //동적할당 해제
    free(arr);
    free(process_list_copy);
}

void SPN(PROCESS process_list[], int size,int process_arrive_time[]){
    printf("SPN: ");

    //모든 process가 수행되는데 걸리는 시간
    int time=total_service_time(process_list, size);
    
    //tmp 동적할당
    int *arr = (int *)malloc(time*sizeof(int));
    QUEUE readyQ={-1,-1,0,arr};

    int start_time=0;
    for(int i=0;i<time;i++){
        int idx=process_arrive_time[i];
        if(idx!=-1){
            enqueue(&readyQ,idx);
        }
        if(start_time<=i){
            sort_service_time(process_list, &readyQ);
            int cur=dequeue(&readyQ);
            for(int i=0;i<process_list[cur].service_time;i++){
                printf("%c ",process_list[cur].name);
                start_time+=1;
            }
        }
    }

    printf("\n");

    //동적할당 해제
    free(arr);
}

void HRRN(PROCESS process_list[], int size,int process_arrive_time[]){
    printf("HRRN: ");

    //모든 process가 수행되는데 걸리는 시간
    int time=total_service_time(process_list, size);

    //tmp 동적할당
    int *arr = (int *)malloc(time*sizeof(int));
    QUEUE readyQ={-1,-1,0,arr};

    int start_time=0;
    for(int i=0;i<time;i++){
        int idx=process_arrive_time[i];
        if(idx!=-1){
            enqueue(&readyQ,idx);
        }
        if(start_time<=i){
            sort_high_response_time(process_list, &readyQ, start_time);
            int cur=dequeue(&readyQ);
            for(int i=0;i<process_list[cur].service_time;i++){
                printf("%c ",process_list[cur].name);
                start_time+=1;
            }
        }
    }
    printf("\n");

    //동적할당 해제
    free(arr);
}

void FeedBack(PROCESS process_list[], int size,int process_arrive_time[],int time_quantum){
    printf("FreedBack(q=%d): ",time_quantum);

    //process_list의 복사본을 만듦
    PROCESS *process_list_copy=(PROCESS *)malloc(size*sizeof(PROCESS));
    for(int i=0;i<size;i++){
        process_list_copy[i].number=process_list[i].number;
        process_list_copy[i].name=process_list[i].name;
        process_list_copy[i].arrive_time=process_list[i].arrive_time;
        process_list_copy[i].service_time=process_list[i].service_time;
    }

    //모든 process가 수행되는데 걸리는 시간
    int time=total_service_time(process_list, size);

    //tmp 동적할당
    int *arr0 = (int *)malloc(time*sizeof(int));
    int *arr1 = (int *)malloc(time*sizeof(int));
    int *arr2 = (int *)malloc(time*sizeof(int));
    int *arr3 = (int *)malloc(time*sizeof(int));
    QUEUE Q0={-1,-1,0,arr0};
    QUEUE Q1={-1,-1,0,arr1};
    QUEUE Q2={-1,-1,0,arr2};
    QUEUE Q3={-1,-1,0,arr3};
    int *tmp_arr = (int *)malloc(time*sizeof(int));
    QUEUE tmp_Q={-1,-1,0,tmp_arr};


    int start_time=0;
    int cur;
    int flag=0;
    for(int i=0;i<time;i++){
        int idx=process_arrive_time[i];
        if(idx!=-1){
            //printf("Q1enqueue(%d)%c\n", start_time,process_list[idx].name);
            enqueue(&Q0,idx);
            flag+=1;
        }

        if(flag<2){
            while(tmp_Q.front!=tmp_Q.rear){
                cur=dequeue(&tmp_Q);
                if(process_list_copy[cur].service_time!=0){
                    enqueue(&Q0,cur);
                }
            }
        }else{
            while(tmp_Q.front!=tmp_Q.rear){
                cur=dequeue(&tmp_Q);
                if(process_list_copy[cur].service_time!=0){
                    enqueue(&Q1,cur);
                }
            }
        }

        if(start_time<=i){
            if(Q0.front!=Q0.rear){
                int cur=dequeue(&Q0);
                for(int j=0;j<1;j++){
                    printf("%c ",process_list[cur].name);
                    process_list_copy[cur].service_time-=1;
                    start_time+=1;
                    if(process_list_copy[cur].service_time==0)
                        break;
                }
                if(process_list_copy[cur].service_time!=0)
                    enqueue(&tmp_Q,cur);
            }else if(Q1.front!=Q1.rear){
                int cur=dequeue(&Q1);
                for(int j=0;j<time_quantum;j++){
                    printf("%c ",process_list[cur].name);
                    process_list_copy[cur].service_time-=1;
                    start_time+=1;
                    if(process_list_copy[cur].service_time==0)
                        break;
                }
                if(process_list_copy[cur].service_time!=0)
                    enqueue(&Q2,cur);
            }else if(Q2.front!=Q2.rear){
                int cur=dequeue(&Q2);
                for(int j=0;j<time_quantum*time_quantum;j++){
                    printf("%c ",process_list[cur].name);
                    process_list_copy[cur].service_time-=1;
                    start_time+=1;
                    if(process_list_copy[cur].service_time==0)
                        break;
                }
                if(process_list_copy[cur].service_time!=0)
                    enqueue(&Q3,cur);
            }else if(Q3.front!=Q3.rear){
                int cur=dequeue(&Q3);
                for(int j=0;j<time_quantum*time_quantum*time_quantum;j++){
                    printf("%c ",process_list[cur].name);
                    process_list_copy[cur].service_time-=1;
                    start_time+=1;
                    if(process_list_copy[cur].service_time==0)
                        break;
                }
                if(process_list_copy[cur].service_time!=0)
                    enqueue(&Q3,cur);
            }
        }
    }
    printf("\n");

    //동적할당 해제
    free(arr0);
    free(arr1);
    free(arr2);
    free(arr3);
    free(tmp_arr);
}