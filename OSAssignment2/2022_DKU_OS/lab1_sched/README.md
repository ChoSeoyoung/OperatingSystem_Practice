# Scheduling_simultor
FCFS, RR, SPN, HRRN, MLQS를 구현하였다.
## 프로그램 구조
- Makefile: 실습 자료들을 컴파일 하기 위한 파일 
- include: 실습에 사용할 구조체 및 구현할 함수에 대한 헤더파일 
- lab1_sched.c: scheduler algorithm 관련 소스 파일 
- lab1_sched_test.c: lab1_sched에서 구현한 scheduler algorithm을 수행할 test code
## 주요 기능 소개
- 구조체 
process번호, process이름, 도착시간, 실행시간의 정보를 가진 PROCESS구조체를 정의하여 사용
하였다. 또한 readyQueue를 구현하기 위해 QUEUE구조체를 정의하여 사용하였다. 
- 큐 함수 
Queue와 관련된 함수 enqueue(큐에 원소 삽입), dequeue(큐의 원소 pop)를 정의하여 사용하였
다. 
- 기능구현을 위한 함수 
FCFS, RR, SPN, HRRN, FeedBack을 구현하기 위해 필요한 함수를 구현하였다. 
-total_service_time: 수행될 모든 process들의 수행시간을 return한다. 
-sort_servie_time: SPN 알고리즘 구현을 위해 정의되었다. Process들을 service_time을 기준으
로 오름차순으로 정렬시킨다. 
-sort_high_response_time: HRRN 알고리즘 구현을 위해 정의되었다. Response_time을 기준으
로 내림차순으로 정렬시킨다. 
- 스케줄링 알고리즘 
FCFS, RR, SPN, HRRN, FeedBack 알고리즘을 구현하였다.

# FCFS 
FCFS(FIFO)는 먼저 도착하는 순서대로 프로세스를 스케줄링하는 기법이다. Arrive_time을 기준
으로 ready_queue에 삽입되고, ready_queue에서 원소를 하나씩 꺼내 service_time만큼 실행시
킨다. 
구현을 위해서 ready_queue를 process의 개수만큼 동적할당 하였고, 도착하는 순서대로 
ready_queue에 삽입하였다. 이후, ready_queue에서 원소를 하나씩 꺼내 service_time만큼 
process의 name을 출력하였다. 
# RR 
RR(Round Robin)은 도착한 순서대로 ready_queue에 삽입되고, ready_queue에서 원소를 하나씩 
꺼내 time_slice만큼 process를 수행한다. 수행 후, 만약 process의 service_time이 남았다면 다
시 ready_queue로 process를 삽입한다. RR은 위 과정을 반복한다. 
구현을 위해서 process 정보를 복사하였다. Process들에 대한 정보를 process_list라는 배열의 
형태로 RR 함수에 전달하였기 때문에, 원본을 변경하지 않기 위해서 proess_list_copy에 
proess_list를 복사하여 사용하였다. 도착하는 순서대로 process들을 ready_queue에 삽입한 후, 
ready_queue에서 원소를 하나씩 꺼내 time_slice만큼 process를 수행시켰다. 수행시킨 시간만큼 
복사한 process의 service_time을 감소시키고, service_time이 0이 아니라면 다시 ready_queue
에 삽입하였다. 위 과정을 반복하여 RR(Round Robin)을 구현하였다. 
# SPN 
SPN(SJF)는 프로세스의 실행시간이 짧은 순서대로 프로세스를 스케줄링하는 기법이다. 만약 동
일한 시간에 도착한 두 개의 프로세스가 있다면 실행시간이 짧은 프로세스를 먼저 스케줄링한다. 
구현을 위해서 ready_queue를 동적할당하였고, 프로세스가 도착하면 ready_queue에 삽입하였
다. 이때, 프로세스의 실행시간이 짧은 순서대로 프로세스를 스케줄링하기 위해서, ready_queue
에서 원소를 하나씩 꺼내기 전에 ready_queue를 실행시간을 기준으로 오름차순 정렬하여 원소를 
꺼냈다. 이를 위해서 sort_service_time 함수를 사용하였다. 
 
# HRRN 
HRRN은 프로세스의 Response Ratio가 좋은 순서대로 프로세스를 스케줄링하는 기법이다. 이때, 
Response Ratio는 (waiting time+required CPU time)/(required CPU time)으로 계산한다. 
구현을 위해서 ready_queue를 동적할당하였고, 프로세스가 도착하면 ready_queue에 삽입하였
다. SPN과 비슷하게, 프로세스를 ready_queue에서 꺼내기 전에 response_ratio를 기준으로 먼저 
정렬시키고 ready_queue에서 프로세스를 하나씩 꺼내 service_time만큼 실행시켰다.이때, 프로
세스를 response_ratio를 기준으로 정렬시키기 위해서 sort_high_response_time 함수를 사용하
였다. 
 
# FeedBack(q=1, q=2^i) 
MLFQ는 queue에 우선순위를 다르게 부여하여 우선순위가 높은 queue부터 프로세스를 스케줄
링하는 기법이다. 앞선 스케줄링 정책과의 차이점은 ready_queue가 여러 개 필요하다는 점이다. 
처음 스케줄링되는 프로세스에게는 가장 높은 우선순위를 부여한다. 만약 해당 프로세스가 
time_quantum안에 수행된다면 프로세스의 우선순위를 유지하고, time_quantum안에 수행되지 못
하면 프로세스의 우선순위를 내린다. 
구현을 위해서 Q0, Q1, Q2, Q3를 각각 동적할당하였고, 프로세스가 도착하면 먼저 가장 높은 우
선순위를 가진 Q0에 삽입하였다. 이후에 Q0, Q1, Q2, Q3에서 원소를 하나씩 꺼내 time_quantum
만큼 프로세스를 수행시켰다. 이때, Q0>Q1>Q2>Q3 순으로 우선시하였다. 예를 들어, Q0에 원소
가 있다면 Q1에 원소가 있더라고 Q0의 원소를 먼저 꺼내 수행시켰다. Time_quantum만큼 수행
시킨 후, 프로세스의 service_time이 남아있다면, 우선순위를 한단계 내려 다시 queue에 삽입하
였다. 
