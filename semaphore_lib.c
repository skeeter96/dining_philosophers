#include "semaphore_lib.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>


int sem_count = 0;
int all_locks[5];

void semaphore_init() {
	int i;
	for(i=0; i<5; i++) {
		all_locks[i] = 0;
	}
}

void semaphore_destroy() {
	int i;
	for(i=0; i<5; i++) {
		semctl(all_locks[i], 0, IPC_RMID);
	}
}

int semaphore_lock(int lock_num) {
	
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = -1;
	op.sem_flg = 0;

	return semop(lock_num, &op, 1);

}


int semaphore_unlock(int lock_num) {

    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = 0;

    return semop(lock_num, &op, 1);

}


int create_semaphore(int sem_count) {
	key_t semkey = ftok(".", (1234)+sem_count);
	sem_count++;
	int lock = semget(semkey, 1, IPC_PRIVATE | IPC_CREAT | 0666);
	
	union semun {
		int val;
		struct semid_ds *buf;
		ushort *array;
	} argument;
	argument.val =1;
	
	semctl(lock, 0, SETVAL, argument);
	
	all_locks[sem_count] = lock;
	
	return lock;

}


















