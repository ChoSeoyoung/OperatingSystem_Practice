#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

/**
 * 실제 가리키는 주소를 print한 뒤, p의 값을 계속 증가시킨다.
 * 제한된 메모리 공간을 가졌지만, 메모리 가상화를 통해서 각 process마다 독립적인 메모리를 사용하는 것처럼 보임
 */
int main(int argc, char *argv[]){
	int *p = malloc(sizeof(int));
	assert(p != NULL);
	printf("(%d) address pointed to by p: %p\n", getpid(), p);
	while(1){
		Spin(1);
		*p=*p+1;
		printf("(%d) p: %d\n",getpid(),*p);
	}
	return 0;
}
