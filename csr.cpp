#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct edge{
  int src,dest,weig;
};


int main()
{
  int V = 5; // Number of vertices in graph
  int E = 10; // Number of edges in graph
  edge g[E];
  vector <int> edges(E);
  vector <int> offset(V+1);

	ifstream infile;
	infile.open("smallgraph.txt");
	string line;
	int i=0;

	while(getline(infile,line))
	{
		if(line.length()==0||line[0] < '0'||line[0]>'9'){
			continue;
		}

		stringstream ss(line); 

		ss>>g[i].src;
		ss>>g[i].dest;
		ss>>g[i].weig;
		i++;
		
	}
  
  
  vector <int> offset2(V+1);

  for(int i=0; i<E ; i++)
  {
      cout<<g[i].src<<","<<g[i].dest<<" -> "<<g[i].weig<<endl;

      int u=g[i].src;
      int v=g[i].dest;

      offset[u+1]+=1;
  }

  for(int i=1; i<=V ; i++)
  {
    offset[i]+=offset[i-1];
  }

  for(int i=1; i<=V ; i++)
  {
    offset2[i]=offset[i];
  }

  for(int i=0; i<E; i++)
  {
    int u=g[i].src;
    int v=g[i].dest;

    edges[offset2[u]]=v;
    offset2[u]++;
  }

  cout<<"The offset array is : \n";
  for(int i=0; i<=V; i++)
  cout<<offset[i]<<",";

  cout<<endl;
  
  cout<<"The edge array is : \n";
  for(int i=0; i<E; i++)
  cout<<edges[i]<<",";

  cout<<endl;
  
//   for each edge (u, v) in graph   // calculate du degree
//     degree(v) += 1
//     offset2(v) += 1
// begin = 0
// for node u in graph
//     end = offset1(u)
//     for i in (begin, end)
//         index = indice1(i)
//         index2 = offset2[index]
//         offset2[index] += 1
//         indice2[index2] = u
//     begin = end

  
   return 0;
}
