#ifdef DEBUG_COLLATZ

#include <stdio.h>
#include <stdlib.h>

int collatz(int n);



int main(int argc, char *argv[])

{
	int num;

	if(argc!=2){
		fprintf(stderr,"Error \n");
		return 1;
	}

	num=atoi(argv[1]);

	collatz(num);

	return 0;

}


int collatz(int n)
{
	int count=0;
	int val=n;
	while(val!=1){
		if(val%2==0)
			val=val/2;
		else
			val=3*val+1;
		count++;
		printf(" [%d]:[%d]", count,val);

	}

	return count;
}
#endif

