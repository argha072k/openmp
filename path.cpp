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


struct Edge {
	int src, dest, weight;
};
/*
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

void printans(vector<long> dist, int n,double t1,double t2)
{
	//storing in a file

	ofstream MyFile("parallel_output.txt");
	MyFile<<"Vertex Distance from Source\n";
	MyFile<<"NodeId\tShortest distance from source \n";
	for (int i = 0; i < n; ++i)
		MyFile<<i<<" \t\t "<<dist[i]<<"\n";

  // Close the file
  MyFile.close();

  //displaying in terminal
	printf("Vertex Distance from Source\n");
	for (int i = 0; i < n; ++i)
		printf("%d \t\t %ld\n", i, dist[i]);
    
    printf("total time taken is %g",t2-t1);    
	
}

 
void BellmanFord(vector <Edge> graph ,int V,int E, int src)
{
vector<long> dist(V);	

bool change = true;
omp_set_num_threads(16);

double t1=omp_get_wtime();
   #pragma omp parallel for
	for (int i = 0; i < V; i++)
		dist[i]=INT_MAX;

	dist[src] = 0;


	while(change){    
        change=false;
		#pragma omp parallel for default(none) shared(E,dist,graph,change)
		for (int j = 0; j < E; j++){
			int u = graph[j].src;
			int v = graph[j].dest;
			int wt = graph[j].weight;

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
				int u = graph[j].src;
		        int v = graph[j].dest;
		        int wt = graph[j].weight;
		        if (dist[u] != INT_MAX && dist[u] + wt < dist[v])
				{
					printf("negative cycle is pres\n");
				}
			}
		}	   
				
double t2=omp_get_wtime();

	printans(dist, V,t1,t2);
}



int main(int argc,char *argv[])
{ 
    int V = 1971259; // Number of vertices in graph
    int E = 5533214; // Number of edges in graph
    vector <Edge>g;

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
		Edge inp;
		ss>>inp.src;
		ss>>inp.dest;
		inp.weight=2;
		i++;
		
		g.push_back(inp);
	}

    BellmanFord(g, V, E, 0);

    return 0;
}


