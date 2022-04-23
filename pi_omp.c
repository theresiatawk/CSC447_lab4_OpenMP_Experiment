#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

// KEY for randomizing the array
#define KEY 2345

int* array;
int length;

double calculatePI()
{
    int i;
    int total_number = 0;
    int number_inside_circle = 0;

    omp_set_num_threads(1);

    srand(time(NULL));

#pragma omp parallel firstprivate(local_in_circle)
    {
#pragma omp for
        for (i = 0; i < length; i++)
        {
            // Generating random x and y

            double x = (rand() % KEY) / (double)(KEY - 1);
            double y = (rand() % KEY) / (double)(KEY - 1);

            array[i] = sqrt((x * x) + (y * y)) <= 1 ? 1 : 0;

            if (array[i] == 1)
            {
                number_inside_circle++;
            }
        }
#pragma omp critical
        {
            total_number += number_inside_circle;
        }
    }
    return ((double)total_number / length) * 4.0;
}

int main(int argc, char* argv[])
{
    length = 100000000;
    double pi;
    array = calloc(length, sizeof(int));


    clock_t time_start = clock();
    pi = calculatePI();
    clock_t time_end = clock();
    double time_spent = ((double)(time_end - time_start) / CLOCKS_PER_SEC) * 1000.0;
    printf("Pi is equal %f and it's execution time in parallel is %fms\n", pi, time_spent);

    clock_t time_startt = clock();
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        double x = (rand() % KEY) / (double)(KEY - 1);
        double y = (rand() % KEY) / (double)(KEY - 1);
        array[i] = sqrt((x * x) + (y * y)) <= 1 ? 1 : 0;
        if (array[i] == 1)
        {
            count++;
        }
    }
    clock_t time_endd = clock();
    double time_spentt = ((double)(time_endd - time_startt) / CLOCKS_PER_SEC) * 1000.0;
    pi = 4.0 * ((double)count / length);
    printf("Pi is equal %f and it's execution time in parallel is %fms\n", pi, time_spent);
    return 0;
}