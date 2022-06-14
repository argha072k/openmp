//single src shortest path

#include <stdio.h>
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
	int numt=omp_get_max_threads();
	bool change;
	bool local_change[numt];
	int k=0;

    double t1=omp_get_wtime();
#pragma omp parallel 
{
	
	#pragma omp for
	for (int i = 0; i < V; i++)
		dist[i] = INT_MAX;

	dist[src] = 0;

    #pragma omp barrier
    
    
    int tid=omp_get_thread_num();
	for (int i = 1; i <= V - 1; i++) 
	{

		local_change[tid]=false;
        
		for (int j = 0; j < E; j++)
		{
			int u = graph[j][0];
			int v = graph[j][1];
			int wt = graph[j][2];
			if (dist[u] != INT_MAX
				&& dist[u] + wt < dist[v])
                {
                local_change[tid]=true;
				dist[v] = dist[u] + wt;
                }
		}
	}
    #pragma omp barrier
    #pragma omp single
	{
		k++;
		change=false;
		for(int i=0;i<numt;i++)
		{
			change = change | local_change[i];
		}
	}
	       


   if(k==V-1)
   {
        change=false;
		for (int i = 0; i < E; i++)
		{
			#pragma omp parallel for reduction (|:change)
			for(int j=0;j<E;j++)
			{
				int u = graph[i][0];
		        int v = graph[i][1];
		        int wt = graph[i][2];
		        if (dist[u] != INT_MAX && dist[u] + wt < dist[v])
				{
					change=true;
				}
			
			}
			
		}
        if(change)
		{
			printf("graph has negative cycle \n");
		}
   }
	
	
	double t2=omp_get_wtime();

    #pragma omp single
	printans(dist, V,t1,t2);
}
}	


int main(int argc,char *argv[])
{ 
    int V = 5; // Number of vertices in graph
    int E = 8; // Number of edges in graph
    
    int graph[][3] = { { 0, 1, 6 }, { 0, 2, 7 },{ 1, 3, 5 }, { 1, 4, -4 },{ 1, 2, 8 }, {2, 3, -3},{ 2, 4, 9 }, { 3, 1, -2 },{ 4, 0, 2 }, { 4, 3, 7 }};
	
	int size = 0;
	
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 5; j++)
            if (graph[i][j] != 0)
                size++;
 
    // number of columns in compactMatrix (size) must be
    // equal to number of non - zero elements in
    // graph
    int compactMatrix[3][size];
 
    // Making of new matrix
    int k = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 5; j++)
            if (graph[i][j] != 0)
            {
                compactMatrix[0][k] = i;
                compactMatrix[1][k] = j;
                compactMatrix[2][k] = graph[i][j];
                k++;
            }
 
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<size; j++)
            printf("%d ", compactMatrix[i][j]);
 
        printf("\n");
 
    }
 
    BellmanFord(graph, V, E, 0);

    return 0;
}

/*
Vertex Distance from Source
0                0
1                2
2                7
3                4
4                -2
total time taken is 0.00300002
*/
