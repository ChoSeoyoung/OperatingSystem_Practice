#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include "common.h"

/*
 * Spin을 무한호출
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
