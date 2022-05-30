// Program to increment the array elements

#include <stdio.h>
#include <omp.h>
#define N 100

int a[N];
int main(int argc, char *argv[])
{
    int i,thread_id;
    

 for(int i=0;i<N;i++)
  {
    a[i]=1;    // initialize array elements with 1
  }

#pragma omp parallel default(shared) private(thread_id) num_threads(4)
    {
          
     #pragma omp for
      for(i=0;i<N;i++)
      {
          a[i]=a[i]+1;     
      } 
     
    
    }
    for(int i=0;i<N;i++)
    {
    printf("%d",a[i]);
    }
    
    return 0;
}