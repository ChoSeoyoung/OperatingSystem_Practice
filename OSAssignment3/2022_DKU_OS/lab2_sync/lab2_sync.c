/*
*   DKU Operating System Lab
*           Lab2 (Hash Queue Lock Problem)
*           Student id : 32204298
*           Student name : 조서영
*
*   lab2_sync.c :
*       - lab2 main file.
*		- Thread-safe Hash Queue Lock code
*		- Coarse-grained, fine-grained lock code
*       - Must contains Hash Queue Problem function's declations.
*
*   Implement thread-safe Hash Queue for coarse-grained verison and fine-grained version.
*/

#include <aio.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab2_sync_types.h"

/*
 * TODO
 *  Implement function which init queue nodes for front and rear
 *  ( refer to the ./include/lab2_sync_types.h for front and rear nodes)
 */
void init_queue() {
	// You need to implement init_queue function.
	//pthread_mutex_init(&front->frontLock,NULL);
	//pthread_mutex_init(&rear->rearLock,NULL);
}

/*
 * TODO
 *  Implement function which add new_node at next rear node
 *
 *  @param queue_node *new_node		: Node which you need to insert at queue.
 */
void enqueue(queue_node *new_node) {
	// You need to implement enqueue function.
	if (rear == NULL) { //큐가 비어있을 때
		front = new_node;
	}else {
		new_node->prev = rear->prev;
	}
	rear = new_node;
}

/*
 * TODO
 *  Implement function which add new_node at next rear node
 *
 *  @param queue_node *new_node		: Node which you need to insert at queue in coarse-grained manner.
 */
void enqueue_cg(queue_node *new_node) {
	// You need to implement enqueue_cg function.
	//https://hellmath.tistory.com/41
	pthread_mutex_lock(&rear->rearLock);
	if (front->next == rear) {
		front->next = new_node;
		new_node->next = rear;
		rear->next = new_node;
	}
	else {
		rear->next->next = new_node;
		new_node->next = rear;
		rear->next = new_node;
	}
	pthread_mutex_unlock(&rear->rearLock);
}

/*
 * TODO
 *  Implement function which add new_node at next rear node
 *
 *  @param queue_node *new_node		: Node which you need to insert at queue in fine-grained manner.
 */
void enqueue_fg(queue_node *new_node) {
	// You need to implement enqueue_fg function.
	//https://hellmath.tistory.com/41
	if (front->next == rear) {
		front->next = new_node;
		pthread_mutex_lock(&rear->rearLock);
		new_node->next = rear;
		rear->next = new_node;
		pthread_mutex_unlock(&rear->rearLock);
	}
	else {
		pthread_mutex_lock(&rear->rearLock);
		rear->next->next = new_node;
		new_node->next = rear;
		rear->next = new_node;
		pthread_mutex_unlock(&rear->rearLock);
	}
}

/*
 * TODO
 *  Implement function which delete del_node at location that contains target key
 *
 *  @param queue_node *del_node		: Node which you need to delete at queue.
 */
void dequeue(queue_node *del_node) {
	// You need to implement dequeue function.
	if (front == NULL) {
		printf("Queue is empty!");
	}
	else {
		del_node->data = front->data;
		front = front->next;
	}
}

/*
 * TODO
 *  Implement function which delete del_node at location that contains target key
 *
 *  @param queue_node *del_node		: Node which you need to delete at queue in coarse-grained manner.
 */
void dequeue_cg(queue_node *del_node) {
	// You need to implement dequeue_cg function.
	pthread_mutex_lock(&front->frontLock);
	if (front->next == rear) {
		printf("Queue is empty!");
	}
	else{
		del_node = front->next;
		front->next = del_node;
	}
	pthread_mutex_unlock(&front->frontLock);
}

/*
 * TODO
 *  Implement function which delete del_node at location that contains target key
 *
 *  @param queue_node *del_node		: Node which you need to delete at queue in fine-grained manner.
 */
void dequeue_fg(queue_node *del_node) {
	// You need to implement dequeue_fg function.
	if (front->next == rear) {
		printf("Queue is empty!");
	}
	else{
		pthread_mutex_lock(&front->frontLock);
		del_node = front->next;
		front->next = del_node;
		pthread_mutex_unlock(&front->frontLock);
	}
}

/*
 * TODO
 *  Implement function which init hashlist(same as hashtable) node.
 */
void init_hlist_node() {
	// You need to implement init_hlist_node function.
	for (int i = 0; i < HASH_SIZE; i++){
		//pthread_mutex_init(&(hashlist[i]->lock), NULL);
	}
}

/*
 * TODO
 *  Implement function which calculate hash value with modulo operation.
 */
int hash(int val) {
	// You need to implement hash function.
	int hs;

	hs=val%HASH_SIZE;
	if(hs<0)
		hs+=HASH_SIZE;
	return hs;
}

/*
 * TODO
 *  Implement function which insert the resilt of finding the location 
 *  of the bucket using value to the entry and hashtable
 *
 *  @param hlist_node *hashtable		: A pointer variable containing the bucket
 *  @param int val						: Data to be stored in the queue node
 */
void hash_queue_add(hlist_node *hashtable, int val) {
	// You need to implement hash_queue_add function.
	queue_node* new_node = (queue_node*)malloc(sizeof(queue_node*));
	new_node->prev = NULL;
	new_node->next = NULL;
	new_node->data = val;
	enqueue(new_node);

	hlist_node* new_pnode = (hlist_node*)malloc(sizeof(hlist_node)); //(1)
	new_pnode->next = hashtable;
	new_pnode->q_loc = new_node;
	hashtable = new_pnode;
}

/*
 * TODO
 *  Implement function which insert the resilt of finding the location 
 *  of the bucket using value to the entry and hashtable
 *
 *  @param hlist_node *hashtable		: A pointer variable containing the bucket
 *  @param int val						: Data to be stored in the queue node
 */
pthread_mutex_t lock2;
void hash_queue_add_cg(hlist_node *hashtable, int val) {
	// You need to implement hash_queue_add_cg function.
	queue_node* new_node = (queue_node*)malloc(sizeof(queue_node*));
	new_node->data = val;
	enqueue_fg(new_node);
	
	hlist_node* new_hlist_node = (hlist_node*)malloc(sizeof(hlist_node*));

	int hs = hash(val);
	//pthread_mutex_lock(&(hashtable[hs].lock));
	pthread_mutex_lock(&lock2);
	if (hashtable[hs].q_loc == NULL) {
		hashtable[hs].q_loc = new_node;
	}
	else {
		hlist_node* curr = &hashtable[hs];
		while (curr->next != NULL) {
			curr = curr->next;
		}
		curr->q_loc = new_node;
		curr->next = new_hlist_node;
	}
	//pthread_mutex_unlock(&(hashtable[hs].lock));
	pthread_mutex_unlock(&lock2);
}

/*
 * TODO
 *  Implement function which insert the resilt of finding the location 
 *  of the bucket using value to the entry and hashtable
 *
 *  @param hlist_node *hashtable		: A pointer variable containing the bucket
 *  @param int val						: Data to be stored in the queue node
 */
void hash_queue_add_fg(hlist_node *hashtable, int val) {
	// You need to implement hash_queue_add_fg function.
	queue_node* new_node = (queue_node*)malloc(sizeof(queue_node*));
	new_node->data = val;
	enqueue_fg(new_node);
	
	hlist_node* new_hlist_node = (hlist_node*)malloc(sizeof(hlist_node*));

	int hs = hash(val);
	
	if (hashtable[hs].q_loc == NULL) {
		hashtable[hs].q_loc = new_node;
	}
	else {
		pthread_mutex_lock(&lock2);
		hlist_node* curr = &hashtable[hs];
		while (curr->next != NULL) {
			curr = curr->next;
		}
		curr->q_loc = new_node;
		curr->next = new_hlist_node;
		pthread_mutex_unlock(&lock2);
	}
}

/*
 * TODO
 *  Implement function which check if the data(value) to be stored is in the hashtable
 *
 *  @param int val						: variable needed to check if data exists
 *  @return								: status (success or fail)
 */
int value_exist(int val) {
	// You need to implement value_exist function.
	int bucket = hash(val);
	for(hlist_node* start=hashlist[bucket];start!=NULL;start=start->next){
		if(start->q_loc->data==val){
			return 1;
		}
	}
	return 0;
}

/*
 * TODO
 *  Implement function which find the bucket location using target
 */
void hash_queue_insert_by_target() {
	// You need to implement hash_queue_insert_by_target function.
	int idx = hash(target);
	hash_queue_add(hashlist[idx], target);
}

/*
 * TODO
 *  Implement function which find the bucket location using target
 */
void hash_queue_insert_by_target_cg() {
	// You need to implement hash_queue_insert_by_target_cg function.
	hash_queue_add_cg((hlist_node *)hashlist, target);
}

/*
 * TODO
 *  Implement function which find the bucket location using target
 */
void hash_queue_insert_by_target_fg() {
	// You need to implement hash_queue_insert_by_target_fg function.
	hash_queue_add_fg((hlist_node *)hashlist, target);
}

/*
 * TODO
 *  Implement function which find the bucket location and stored data
 *  using target and delete node that contains target
 */
void hash_queue_delete_by_target() {
	// You need to implement hash_queue_delete_by_target function.
	if(value_exist(target)){
		queue_node* del_node = (queue_node*)malloc(sizeof(queue_node*));
		dequeue(del_node);
		
		int bucket = hash(target);
		for(hlist_node* start=hashlist[bucket];start!=NULL;start=start->next){
			if(start->q_loc==del_node){
				start->next=start->next->next;
			}
		}
	}
}

/*
 * TODO
 *  Implement function which find the bucket location and stored data
 *  using target and delete node that contains target
 */
void hash_queue_delete_by_target_cg() {
	// You need to implement hash_queue_delete_by_target_cg function.queue_node del_node;
	int bucket = hash(target);
	hlist_node* curr = hashlist[bucket];
	//pthread_mutex_lock(&(hashlist[bucket]->lock));
	pthread_mutex_lock(&lock2);
	queue_node del_node;
	del_node.data = target;
	dequeue_cg(&del_node);
	
	while (curr->next != NULL) {
		curr = curr->next;
		if (curr->q_loc->data == target) {
			if(curr->next->next!=NULL){
				curr->next=curr->next->next;
			}else{
				curr->next=NULL;
			}
		}
	}
	pthread_mutex_unlock(&lock2);
	//pthread_mutex_unlock(&(hashlist[bucket]->lock));
}

/*
 * TODO
 *  Implement function which find the bucket location and stored data
 *  using target and delete node that contains target
 */
void hash_queue_delete_by_target_fg() {
	// You need to implement hash_queue_delete_by_target_fg function.
	if(value_exist(target)){
		queue_node* new_node = (queue_node*)malloc(sizeof(queue_node*));
		enqueue_fg(new_node);
	}
	value_exist(target);
	queue_node del_node;
	del_node.data = target;
	dequeue_fg(&del_node);

	int bucket = hash(target);
	hlist_node* curr = hashlist[bucket];
	pthread_mutex_lock(&lock2);
	while (curr->next != NULL) {
		curr = curr->next;
		if (curr->q_loc->data == target) {
			if(curr->next->next!=NULL){
				curr->next=curr->next->next;
			}else{
				curr->next=NULL;
			}
		}
	}
	pthread_mutex_unlock(&lock2);
}

