//server
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

struct sembuf r_unlock[]={0,-1,0};
struct sembuf r_lock[] = {0,0,0,0, 1,0};
struct sembuf s_unlock[]={1,-1,0};
struct sembuf s_lock[] = {1,0,0,1,1,0};

int main(int argc, char **argv) {

 	char *shm_addres;
	char str[20];
	key_t key = ftok("c", 'e');

	int shmid = shmget (key, sizeof(char)*20, IPC_CREAT | 0666);
	int semid = semget (key, 2, IPC_CREAT | 0666);
	
	shm_addres = shmat (shmid, NULL, 0); 
		
	for (int i=0; i<2; i++) {

		semop(semid, s_lock, 2);
		printf("\nSend: ");
		gets(str);	
		strcpy(shm_addres, str);		
		semop(semid, s_unlock, 1);

		semop(semid, r_lock, 2);
		printf("Recieved: %s\n", shm_addres);
		semop(semid, r_unlock, 1);
		semop(semid, s_unlock, 1);
		}

	shmdt (shm_addres);
	shmctl (semid, IPC_RMID, 0);
	semctl (shmid, IPC_RMID, 0);

	return 0;
}
