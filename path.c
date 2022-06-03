//single src shortest path

#include <stdio.h>
#include<limits.h>
#include <stdlib.h>
#include<omp.h>

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


void printArr(int dist[], int n,double t1,double t2)
{
	printf("Vertex Distance from Source\n");
	for (int i = 0; i < n; ++i)
		printf("%d \t\t %d\n", i, dist[i]);

    printf("total time taken is %g",t2-t1);    
}


void BellmanFord(int graph[][3] ,int V,int E, int src)
{
	
	int dist[V];


    double t1=omp_get_wtime();
#pragma omp parallel num_threads(4)
{
	#pragma omp for
	for (int i = 0; i < V; i++)
		dist[i] = INT_MAX;

	dist[src] = 0;
    #pragma omp barrier

	for (int i = 1; i <= V - 1; i++) {
		for (int j = 0; j < E; j++) {
			int u = graph[j][0];
			int v = graph[j][1];
			int wt = graph[j][2];
			if (dist[u] != INT_MAX
				&& dist[u] + wt < dist[v])
                {
                #pragma omp critical
				dist[v] = dist[u] + wt;
                }
		}
	}
    #pragma omp barrier
}

    double t2=omp_get_wtime();
	
	for (int i = 0; i < E; i++) {
		int u = graph[i][0];
		int v = graph[i][1];
		int wt = graph[i][2];
		if (dist[u] != INT_MAX
			&& dist[u] + wt < dist[v]) {
			printf("Graph contains negative weight cycle");
			return;  // If negative cycle is detected,return
					
		}
	}

	printArr(dist, V,t1,t2);

	
}


int main(int argc,char *argv[])
{
	
	int V = 5; // Number of vertices in graph
	int E = 8; // Number of edges in graph

    int graph[][3] = { { 0, 1, -1 }, { 0, 2, 4 },
                       { 1, 2, 3 }, { 1, 3, 2 },
                       { 1, 4, 2 }, { 3, 2, 5 },
                       { 3, 1, 1 }, { 4, 3, -3 } };
 
    BellmanFord(graph, V, E, 0);
    return 0;


}
