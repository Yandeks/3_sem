#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define points  10000
#define threads 479001600
#define floor 0
#define roof 1
int start =  0;
#define BILLION  1000000000L
float* randval;	
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* routine(void *args) {
	while(start < points - 1) {
		pthread_mutex_lock(&mutex);
		float x =drand48()*(roof-floor) + floor;
		randval[start] = exp(x)*sin(x);
		start++;
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}


int main() {
	randval = (float*)malloc(sizeof(float)*points);
	struct timespec start_, stop;
	float time;
	 printf ("value: threads: time:\n");
	for (int i = 1; i < (threads); i++) {
		if((threads%(i+1)) == 0) {
			int num =i;
			if( clock_gettime( CLOCK_MONOTONIC, &start_) == -1 ) {
   				perror( "clock gettime" );
    			return EXIT_FAILURE;
   			}
			pthread_t IDbuf[num];
			for (int j = 0; j < num; j++) {
				int err = pthread_create(&IDbuf[j], NULL, routine, NULL);
				if (err != 0 ) {
					printf("thread creation is failed");
					return -1;
				}
			}
			for(int l =0; l < num; l++){
				pthread_join(IDbuf[l], NULL);
			}
			float integral = 0;
			for (int j = 0; j < points; j++) {
				integral += (roof-floor)*randval[j]/points;
			}

			if( clock_gettime( CLOCK_MONOTONIC, &stop) == -1 ) {
    			perror( "clock gettime" );
     			return EXIT_FAILURE;
 		   	}
			 time = ( stop.tv_sec - start_.tv_sec )
                   	 + (float)( stop.tv_nsec - start_.tv_nsec )
                   	 /(float)BILLION;
                        printf("%f %d %f\n", integral, num, time);
			start =0;
		}
	}
	free(randval);
	return 0;
}