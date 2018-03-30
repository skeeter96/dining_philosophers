#include <stdio.h>
#include <stdlib.h>
#include "philosopher.h"
#include "semaphore_lib.h"

int main(int argc, char**argv) {

	semaphore_init(); //initialize semaphores
	int algorithm;
	int wait_time = 60;

	if(argc > 1) {
		wait_time = atoi(argv[1]);
		algorithm = 1;
	} else {
		algorithm = 0;
	}

	create_5_philosophers(algorithm);
	
	printf("PARENT: sleeping for %i seconds\n", wait_time);
	sleep(wait_time);

	kill_5_philosophers();
	
	sleep(5); //children die
	semaphore_destroy(); //destroy semaphores
	return 0;

}
