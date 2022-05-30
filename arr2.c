// Program to add all array elements

#include <stdio.h>
#include <omp.h>
#define N 10000000

int a[N];
int main(int argc, char *argv[]){
    int i,sum=0;
    

for(int i=0;i<N;i++)
{
    a[i]=1;
}

    #pragma omp parallel default(shared) num_threads(4)
    {
      int psum=0;
     
     #pragma omp for
      for(i=0;i<N;i++)
      {
          psum+=a[i];
      } 
     
    
     #pragma omp critical
          sum+=psum;
      
    
    }
    printf("the sum is %d\n",sum);
    
    return 0;
}