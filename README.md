# OPERATING SYSTEMS
# SYSTEM CALL USING SEMAPHORE 
# CHAINSMOKER PROBLEM

# Instructor Name:
Dr.Gufran 

Team Members Section 4A;
Samia Ilyas 21K-4947
Khadijah Mansoor 21K-3199
Sameel Asher 21K-4659


# INTRODUCTION:
The goal of this project is to develop a system call that addresses the issue of chain smoking. In computing, a system call refers to a mechanism by which a running program can request a specific service or operation from the operating system. Such requests can be made either directly by user-level programs without the need for the kernel's involvement or through the kernel itself in what is called a kernel system call.

# FEATURES:
The main task of the program is to manage threads and semaphores for creating and destroying them. This program involves four processes consisting of three smokers and one agent. Each smoker process creates and smokes a cigarette using a combination of tobacco, paper, and matches. Each smoking process requires only one of the three procedures, with each procedure available in infinite supply to the agent. To put it another way, one procedure uses tobacco, another uses paper, and yet another uses matches.The agent places two of the three object on the table, and the smoker with the third object uses it to light the cigarette.

# LIBRARIES:

#include <stdio.h>
#include <unistd.h> /* Symbolic Constants */
#include <sys/types.h> /* Primitive System Data Types */
#include <errno.h> /* Errors */
#include <stdio.h> /* Input/Output */
#include <stdlib.h> /* General Utilities */
#include <pthread.h> /* POSIX Threads */
#include <string.h> /* String handling */
#include <semaphore.h> /* Semaphore */
#include <sys/syscall.h>
#include <linux/kernel.h>


# CODE:

sem_t required;
sem_t match;
sem_t paper;
sem_t tobacco;

void *agent (){
int i=0;
int x=1;
int s=0,p=0,m=0;
while (1){
      int number = rand() % 3;
      if(i==10){
           printf("\n\nNumber of times the  smoker smoked a cigarette in total: %d\n",s);
           printf("\n Total number of times smoker  has smoked using paper as a smoking material: %d\n",p);
	   printf("\nTotal number of times that someone has smoked with match: %d\n",m);
           exit(0);
      }
      sleep(1);
      switch (number){
           case 0: sem_post (&match); /* match and paper */
                   sem_post (&paper);
			syscall(333,"The agent has placed the match onto the table\n");
			//printf("Agent has put match and paper on the table\n");
			printf("SMOKING A CIGAR %d time\n", x++);
			s++;
                   break;
           case 1: sem_post (&match); /* match and tobacco */
                   sem_post (&tobacco);
			syscall(333,"The agent has placed the  match and tobacco onto the table\n");
			//printf("Agent has put match and tobacco on the table\n");
				printf("SMOKING A CIGAR %d time\n", x++);
			p++;
                   break;
           case 2: sem_post (&paper); /* tobacco and paper */
                   sem_post (&tobacco);
			syscall(333,"The agent has placed paper and tobacco onto the table\n");
			//printf("Agent has put paper and tobacco on the table\n");
				printf("SMOKING A CIGAR %d time\n", x++);
			m++;
                   break;
        }
      sem_wait (&required); /* wait for request for more */
	i++;
    }
}

void *smoker_with_tobacco (){
  while (1){
      sem_wait (&match); /* grab match from table */
      if (sem_trywait (&paper) == 0) /* grab paper */{
          /* roll cigarette and smoke */
		syscall(333,"Retrieve Match and Paper");
		syscall(333,"Smoker with Tobacco is Smoking\n");
		//printf("tobacco smoking\n");
		sleep(0.5);
          	sem_post (&required); /* signal to agent */
        }
      else sem_post (&match); /* drop the match */
    }
}

void *smoker_with_match (){
  while (1){
      sem_wait (&paper); /* grab match from table */
      if (sem_trywait (&tobacco) == 0) /* grab paper */{
          /* roll cigarette and smoke */
		syscall(333,"Reterieve Tobacco and Paper");
		syscall(333,"Smoker with Match is Smoking\n");
		//printf("match smoking\n");
		sleep(0.5);
          	sem_post (&required); /* signal to agent */
        }
      else sem_post (&paper); /* drop the match */
    }
}

void *smoker_with_paper (){
  while (1){
      sem_wait (&tobacco); /* grab match from table */
      if (sem_trywait (&match) == 0) /* grab paper */{
		/* roll cigarette and smoke */
		syscall(333,"Retreieve Tobacco and Paper");
		syscall(333,"Smoker with Paper is Smoking\n");
		//printf("paper smoking\n");
		sleep(0.5);
         	sem_post (&required); /* signal to agent */
        }
      else sem_post (&tobacco); /* drop the match */
    }
}

int main(){
	pthread_t thread1, thread2, thread3, thread4;
	sem_init(&required,0,1);
	sem_init(&match,0,0);
	sem_init(&paper,0,0);
	sem_init(&tobacco,0,0);




          pthread_create(&thread1,NULL,agent,NULL);
	pthread_create(&thread2,NULL,smoker_with_tobacco,NULL);
	pthread_create(&thread3,NULL,smoker_with_paper,NULL);
          pthread_create(&thread4,NULL,smoker_with_match,NULL);


          pthread_join(thread1,NULL);
          pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);
	pthread_join(thread4,NULL);

	return 0;

}

# DEALING WITH CONSTRAINTS AND PREVENTING DEADLOCKS:
The main argument of the chainsmokers problem is that there is no solution using traditional semaphores as they existed when the problem was first introduced. At that time, semaphores only allowed incrementing or decrementing their internal value by one. This problem demonstrates that if we are restricted to using only these operations, there are certain scenarios in which it is impossible to avoid deadlock. Regardless of how we construct the agent and smoker threads, as long as the agent's structure remains the same, any arrangement of the smokers will potentially lead to a deadlock situation.
The hold and wait condition is broken by using the sem_trywait() function in the smoker threads. This function attempts to lock the semaphore (grab the ingredient), but if the semaphore is already locked (the ingredient isn't available), it immediately returns with an error instead of waiting. This prevents a smoker from holding one ingredient and waiting for another, which could lead to deadlock.

The circular wait condition is broken by the agent's behavior. The agent only puts two ingredients on the table at a time, and always waits for a smoker to signal that they've smoked a cigarette before putting out more ingredients. This ensures that there's always a smoker who can proceed (the smoker who has the remaining ingredient), and prevents the situation where each smoker is waiting for an ingredient held by another.

The agent's behavior of only putting two ingredients at a time also breaks the mutual exclusion condition, as only one smoker can take the ingredients and smoke at a time.

By breaking these conditions, the program prevents deadlock and ensures that the smokers can keep smoking indefinitely (or until the agent decides to stop).
# CONCLUSION:
After collaborative efforts, our team was successful in devising a solution that prevents the occurrence of deadlock in the chain smokers problem. The implemented system call is immune to race conditions, and serves as an illustration of how the operating system effectively averts deadlocks in numerous processes.












































APPLICATIONS USED:
Programming Language: C language
VMware Work Station 16 
Platform: Ubuntu 16.04




