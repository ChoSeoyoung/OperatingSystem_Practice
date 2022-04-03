#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

/**
 * 실제 가리키는 주소를 print한 뒤, p의 값을 계속 증가시킨다.
 * 실행결과 서로 다른 메모리 주소를 가진 것처럼 보이지만, 실제 물리적 주소는 동일하다.
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
