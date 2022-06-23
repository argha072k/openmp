//single src shortest path

#include <stdio.h>
#include <string.h>
#include<limits.h>
#include <stdlib.h>
#include <omp.h>
#include<stdbool.h>



struct Edge {
	int src, dest, weight;
};

struct Graph {
	
	int V, E;
	struct Edge* edge;
};


struct Graph* createGraph(int V, int E)
{
	struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
	graph->V = V;
	graph->E = E;
	graph->edge = (struct Edge *)malloc(graph->E * sizeof(struct Edge));
	return graph;
}


void printans(int dist[], int n,double t1,double t2)
{
	printf("Vertex Distance from Source\n");
	for (int i = 0; i < n; ++i)
		printf("%d \t\t %d\n", i, dist[i]);
    
    printf("total time taken is %g",t2-t1);    
	
}

 
void BellmanFord(int graph[][3] ,int V,int E, int src)
{
	
int dist[V];
bool change = true;
omp_set_num_threads(8);

double t1=omp_get_wtime();

   #pragma omp parallel for
	for (int i = 0; i < V; i++)
		dist[i] = INT_MAX;

	dist[src] = 0;


	while(change){    
        change=false;
		#pragma omp parallel for default(none) shared(E,dist,graph,change)
		for (int j = 0; j < E; j++){
			int u = graph[j][0];
			int v = graph[j][1];
			int wt = graph[j][2];

				if (dist[u] != INT_MAX && dist[u] + wt < dist[v]){
					dist[v] = dist[u] + wt;
					change=true;
				}		
		}
	}


        #pragma omp single 
		{
        for(int j=0;j<E;j++)
			{
				int u = graph[j][0];
		        int v = graph[j][1];
		        int wt = graph[j][2];
		        if (dist[u] != INT_MAX && dist[u] + wt < dist[v])
				{
					printf("negative cycle is pres\n");
				}
			}
		}	   
				
double t2=omp_get_wtime();

    #pragma omp single
	printans(dist, V,t1,t2);
}



int main(int argc,char *argv[])
{ 
    int V = 5; // Number of vertices in graph
    int E = 10; // Number of edges in graph
    int graph[E][3];

     FILE* ptr;

 
    // Opening file in reading mode
    ptr = fopen("data", "r");
 
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }
 
    printf("content of this file are \n");

    char ch[50];

    int i=0,j=0;

   while( fgets (ch,10,ptr)!=NULL ) {

        char * token = strtok(ch, " ");
        while( token != NULL ) {
            if(j==3)
            {
            j=0;
            i++;
            }
          int t=atoi(token);
            graph[i][j]=t;
            j++;

         token = strtok(NULL, " ");
   }       }

 
    // Closing the file
    fclose(ptr);
	
    BellmanFord(graph, V, E, 0);

    return 0;
}
