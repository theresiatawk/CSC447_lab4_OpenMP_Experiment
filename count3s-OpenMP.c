/*
   See "Principles of Parallel Programming", page 195.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int * array;
int length;
int count;

int count3s()
{
   omp_set_num_threads(1);
   int i;
   count = 0;
   int count_p = 0;  /*** needed to move this declaration ***/
   #pragma omp parallel firstprivate(count_p) /*** need firstprivate() ***/
   {
      #pragma omp for  /*** don't need schedule(static) private(i) ***/
      for(i = 0; i < length; i++)
      {
         if(array[i] == 3)
         {
            count_p++;
         }
      }
      #pragma omp critical
      {
         count += count_p;
      }
   }
   return count;
}


int main(int argc, char *argv[])
{
   int i;
   length = 100000000;

   array = calloc(length, sizeof(int));

   /* initialize the array with random integers between 0 and 9 */
   srand(time(NULL));  /* seed the random number generator with current time */
   for(i = 0; i < length; i++)
   {
      array[i] = rand()%10;
   }

   clock_t time_start = clock();
   count3s();  /* do the actual calculation */
   clock_t time_end = clock();

   double time_spent = ((double)(time_end - time_start) / CLOCKS_PER_SEC) * 1000.0;
   printf("The number of 3's is %d and the execution time using OpenMp is %fms\n", count, time_spent);

   /* As a test, let us count 3's the slow, serial way. */
   count = 0;
   for(i = 0; i < length; i++)
      if(array[i] == 3)
         count++;
   printf("The number of 3's is %d\n", count);

   return 0;
}