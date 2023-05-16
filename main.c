#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <sys/syscall.h>
#include <linux/kernel.h>

sem_t required;
sem_t match;
sem_t paper;
sem_t tobacco;


void *agent ()
{
int i=0;
int x=1;
int s=0,p=0,m=0;
while (1)
{
      int number = rand() % 3;
      if(i==10)
      {
           printf("\n\nNumber of times the  smoker smoked a cigarette in total: %d\n",s);
           printf("\n Total number of times smoker  has smoked using paper as a smoking material: %d\n",p);
	   printf("\nTotal number of times that someone has smoked with match: %d\n",m);
           exit(0);
      }
      sleep(1);
      switch (number)
      {
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

void *smoker_with_tobacco ()
{
  while (1)
  {
      sem_wait (&match); /* grab match from table */
      if (sem_trywait (&paper) == 0) /* grab paper */
        {
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

void *smoker_with_match ()
{
  while (1)
    {
      sem_wait (&paper); /* grab match from table */
      if (sem_trywait (&tobacco) == 0) /* grab paper */
        {
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

void *smoker_with_paper ()
{
  while (1)
    {
      sem_wait (&tobacco); /* grab match from table */
      if (sem_trywait (&match) == 0) /* grab paper */
        {
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

int main() {
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
