#include "semaphore_lib.h"
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include "philosopher.h"


int chopsticks[5];
pid_t philosopher_pid[5];
int eat_count =0;
int think_count = 0;


int pickup_chopstick(int which_stick) {
	//try to pick up chopstick 'which_stick' [0-4]
	usleep(10000);
	return semaphore_lock(which_stick);
	
}

int setdown_chopstick(int which_stick) {
	usleep(10000);
	return semaphore_unlock(which_stick);
	
}

int eat_from_plate() {
	eat_count++;
	sleep(1);
}

int think() {
	think_count++;
	sleep(1);
}

void sigterm_handler(int sig_num) {
	pid_t mypid = getpid();
	printf("Child[%i] eat_count=%i think_count=%i\n", mypid, eat_count, think_count);
	exit(0);
}


void sigchild_handler(int sig_num) {
	int status;
	pid_t pid;
	while((pid = waitpid(-1, &status, WNOHANG|WCONTINUED)) > 0) {
		printf("CHILD: %i exited with status %i\n", pid, status);
	} 
}


void create_5_philosophers(int num) {
	//initialize chopsticks
	int i;
	for(i =0; i<5; i++) {
		chopsticks[i] = create_semaphore(i);
	}
	//set sigterm handler
	if (signal(SIGTERM, sigterm_handler) == SIG_ERR) {
		printf("Error setting SIGTERM handler\n");
		exit(1);
	}
	if (num == 0 ) {

		//create 5 philosophers "deadlocking"
		for(i=0; i<5; i++) {
			pid_t temp = fork();
			if (temp == 0) {
				philosopher_algorithm(i);
				exit(0);
			} else {
				philosopher_pid[i] = temp;
			}
		}
	} else {
		//create 5 philosophers "correct"
        for(i=0; i<5; i++) {
            pid_t temp = fork();
            if (temp == 0) {
                philosopher_algorithm_correct(i);
                exit(0);
            } else {
                philosopher_pid[i] = temp;
            }
        }
	}
	if (signal(SIGCHLD, sigchild_handler) == SIG_ERR) {
		printf("Error setting SIGCHLD handler\n");
	}
}

void kill_5_philosophers(){
	int i;
	for(i=0; i<5; i++) {
		printf("Sending SIGTERM to child %i\n", philosopher_pid[i]);
		kill(philosopher_pid[i], SIGTERM);
	}
}

void philosopher_algorithm(int num){
	int chopstick1 = chopsticks[num];
	int chopstick2 = chopsticks[(num+1)%5];
	pid_t pid = getpid();	
	while(1) { //basic algorithm
		printf("CHILD %i: grabbing chopstick 1\n", pid);
		pickup_chopstick(chopstick1);
		printf("CHILD %i: grabbing chopstick 2\n", pid);
		pickup_chopstick(chopstick2);
		printf("CHILD %i: eating\n", getpid());
		eat_from_plate();
		printf("CHILD %i: setting down chopstick 1\n", pid);
		setdown_chopstick(chopstick1);
		printf("CHILD %i: setting down chopstick 2\n", pid);
		setdown_chopstick(chopstick2);
		printf("CHILD %i: thinking\n", pid);
		think();
	}
}

void philosopher_algorithm_correct(int num) {
	int chopstick1 = chopsticks[num];
    int chopstick2 = chopsticks[(num+1)%5];
    pid_t pid = getpid();
    while(1) { //correct algorithm
		if(num < ((num+1)%5)) {
			printf("CHILD %i: picked up chopstick %i\n", pid, num);
			pickup_chopstick(chopstick1);
			printf("CHILD %i: picked up chopstick %i\n", pid, ((num+1)%5));
			pickup_chopstick(chopstick2);
		} else {
			printf("CHILD %i: picked up chopstick %i\n", pid, ((num+1)%5));
			pickup_chopstick(chopstick2);
			printf("CHILD %i: picked up chopstick %i\n", pid, num);
			pickup_chopstick(chopstick1);
		}
		printf("CHILD %i: eating\n", pid);
		eat_from_plate();
		printf("CHILD %i: setting down chopsticks\n", pid);
		if(num < ((num+1)%5)) {
			printf("CHILD %i: set down chopstick %i\n", pid, num);
			setdown_chopstick(chopstick1);
			printf("CHILD %i: set down chopstick %i\n", pid, ((num+1)%5));
			setdown_chopstick(chopstick2);
		} else {
			printf("CHILD %i: set down chopstick %i\n", pid, ((num+1)%5));
			setdown_chopstick(chopstick2);
			printf("CHILD %i: set down chopstick %i\n", pid, num);
			setdown_chopstick(chopstick1);
		}
		printf("CHILD %i: thinking\n", pid);
		think();
    }
	
}




















