/* use this command for compiling 
	g++ -Xpreprocessor -fopenmp /opt/homebrew/Cellar/libomp/14.0.5/lib/libomp.a
	*/
//single src shortest path

#include <iostream>
#include <string.h>
#include </opt/homebrew/Cellar/libomp/14.0.5/include/omp.h>
#include <sstream>
#include <fstream>
#include <climits>
#include <vector>
#include <algorithm>


using namespace std;

/*
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
*/

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
    int V = 1971259; // Number of vertices in graph
    int E = 5533214; // Number of edges in graph
    int graph[E][3];

	ifstream infile;
	infile.open("roadNet-CA.txt");
	string line;
	int i=0;

	while(getline(infile,line))
	{
		if(line.length()==0||line[0] < '0'||line[0]>'9'){
			continue;
		}

		stringstream ss(line);

		ss>>graph[i][0];
		ss>>graph[i][1];
		graph[i][2]=3;
		i++;
		
	}


    BellmanFord(graph, V, E, 0);

    return 0;
}


