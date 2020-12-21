#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <math.h>
#include <errno.h>
extern "C"
{
#include <bm_kit.h>
}
#define TEST_N_ACCESSES 1000000

static int comp_int64(const void * elem1, const void * elem2) 
{
    int64_t f = *((int64_t*)elem1);
    int64_t s = *((int64_t*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

void average_int64(int64_t* arr , int arr_size, double* mean, double* stddev) 
{
    double sum=0;
    int i;
    for (i = 0; i < arr_size; i++)
        sum += arr[i];
    *mean = sum / arr_size;
    sum=0;
    for (i = 0; i < arr_size; i++)
        sum += pow((arr[i]-*mean), 2);
    *stddev = sqrt((sum/arr_size));
}

int run_test()
{
	double mean, stddev;
	int64_t *measured = (int64_t*)malloc(TEST_N_ACCESSES * sizeof(int64_t));
	printf("Measuring...\n");
	for(int i = 0; i < TEST_N_ACCESSES; i++)
	{
		measured[i] = bm_kit_measure();
		if(i % (TEST_N_ACCESSES/100) == 0)
			printf("\tIteration %d -> %ld cycles\n", i, measured[i]);
	}
	qsort(measured, TEST_N_ACCESSES, sizeof(int64_t), comp_int64);
	printf("Min: %ld cycles\n", measured[0]);
	printf("Max: %ld cycles\n", measured[TEST_N_ACCESSES-1]);
	printf("Median: %ld cycles\n", measured[TEST_N_ACCESSES/2]);
	average_int64(measured, TEST_N_ACCESSES, &mean, &stddev);
	printf("Mean: %.1f cycles (std dev: %.1f cycles)\n", mean, stddev);
	int j;
	for(j = 0; measured[TEST_N_ACCESSES-1-j] >= 1000; j++);
	printf("%d/%d measurements > 1000 cycles\n", j, TEST_N_ACCESSES);
}

int main(int argc, char *argv[])
{		
	int result;
	if (bm_kit_init() < 0) 
	{
		printf("ERROR: Kit init failed\n");
		return -1;
	}
	printf("Kit initialized...\n");
	printf("\n");	
	
	result = run_test();

	bm_kit_exit();
	
	return result;
}

