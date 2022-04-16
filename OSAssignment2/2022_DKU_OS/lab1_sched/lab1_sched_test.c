#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct nodetype{
	int key;
	struct nodetype *link;
}node_pointer;


void distribute(int* masterlist, int size, int exp, node_pointer *list[]){
	for(int i=0;i<10;i++){ //더미를 비운다
		(*list[i]).key=NULL;
		(*list[i]).link=NULL;
	}
	for(int i=0;i<size;i++){
		//int exp=1;
		//for(int j=0;j<index;j++){exp*=10;}
		int idx=masterlist[i]/exp;
		(*list[idx])=*((*list[idx]).link);
		(*list[idx]).key=masterlist[i];
	}
}

void coalesce(int * masterlist, node_pointer *list[]){
	int idx=0;
	for (int i = 0; i < 10; i++){
		node_pointer tmp = *list[i];
		while(tmp.link!=NULL){
			masterlist[idx++]=tmp.key;
			tmp=*(tmp.link);
		}
	}
}

void radix_sort(int* masterlist, int size) {
	node_pointer *list[10];

	int maxValue = 0;
	int exp = 1;
	for (int i = 0; i < size; i++) {
		if (list[i] > maxValue) maxValue = list[i];
	}
	while (maxValue / exp > 0) { // 1의 자릿수 계산
		void distribute(masterlist, size, exp, list);
		void coalesce(masterlist, list);
	}
}


int main(){
	int a[10]={9,8,7,6,5,4,3,2,1,10};
	radix_sort(a,10);
	for(int i=0;i<10;i++){
		printf("%d ",a[i]);
	}
}