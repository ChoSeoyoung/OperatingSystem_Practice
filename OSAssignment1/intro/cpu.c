#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include "common.h"

/*
 * Spin을 무한호출
 * ./cpu A & ./cpu B & ./cpu C 등 여러개의 프로세스를 동시에 실행
 * 마치 여러 개의 프로세스가 동시에 실행되는 것처럼 보임
 * ->실제로는 번갈아가면서 실행되는 것(Time sharing)
 */
int main(int argc, char *argv[]){
	if(argc != 2){ //인자가 2개가 아니면 종료
		fprintf(stderr, "usage: cpu <string>\n");
		exit(1);
	}
	char *str = argv[1];
	while(1){
		Spin(1);
		printf("%s\n",str);
	}
	return 0;
}
