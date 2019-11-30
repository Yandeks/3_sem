#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#define top 1000000
#define dots 20000000
#define left 0
#define right 1
#define down 0
#define up 3

struct _Thread_
{
        float result;
        pthread_t th_id;
        float x;
        float dx;
        int points;
};



float f(float x)
{
	return x*exp(x)*sin(x); 
}

int Random(int from, int to)
{
	return rand()%(from-to)+from;
}

void* Integrate(void* input)
{
	struct _Thread_* temp = (struct _Thread_*)input;
	int t1 = (int)(temp->x * top);
	int t2 = (int)(temp->dx * top);
	for (int i = 0; i<temp->points; i++)
	{
		float _1_rand =(float)Random(t1, t1+t2)/top;
		float _2_rand = (float)Random((int)(down*top), (int)(up*top))/top;
		if(f(_1_rand)>_2_rand)
			temp->result++;
	}	

}

int main()
{
	srand(time(NULL));
	printf("Integral:   N:   Time:\n");
	struct timespec start,stop;
	for (int i=1;i<=sqrt(dots); i++)
	{
		if(dots%i==0)
		{	
			float dx = (float)(right-left)/i;
			struct _Thread_* arr = (struct _Thread_*)malloc(i*sizeof(struct _Thread_));
			for (int th_count=0; th_count<i; th_count++)
			{
				arr[th_count].x = th_count*dx+left;
				arr[th_count].dx = dx;
				arr[th_count].points = dots/i;
				arr[th_count].result = 0;
			}
			if(clock_gettime(CLOCK_MONOTONIC, &start) == -1)
			{
				perror("clock_gettime_error");
				return -1;
			}
			for (int th_count = 0; th_count < i; th_count++)
				pthread_create(&(arr[th_count].th_id),NULL,Integrate,&arr[th_count]);
			for (int th_count=0; th_count < i; th_count++)
				pthread_join(arr[th_count].th_id, NULL);
			if(clock_gettime(CLOCK_MONOTONIC, &stop) == -1)
                        {
                                perror("clock_gettime_error");
                                return -1;
                        }
			double time_sec = (stop.tv_sec - start.tv_sec) +(double)(stop.tv_nsec-start.tv_nsec)/1000000000;
			float answer = 0;
			for (int th_count=0; th_count<i; th_count++)
			{
				answer+=arr[th_count].result;
			}
			answer = answer/dots*(right-left)*(up-down);
			printf("%f     %d       %f\n", answer, i, time_sec);	
		}
	}
}