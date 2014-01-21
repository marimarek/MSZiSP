#include "graph.h"
#include <iostream>
#include <cmath>

#define ITER 1000000
typedef vector< vector<int> > chromosom;

using namespace std;

double eval(const Graph &G, const chromosom &individual, double maxPathLength, double penalty = 10);
chromosom randChromosom(const Graph &G, double maxPathLength);


int main()
{   
    srand(time(NULL));
    
    //int ITER;
    //cin>>ITER;
    double maxPathLength;
    
    Graph G;
    cin>>G;
    cin>>maxPathLength;
    
    chromosom bestCh = randChromosom(G, maxPathLength);
    double bestScore = eval(G, bestCh, maxPathLength);
    
    
    chromosom pom;
    double pom1;
    
    for(int i=0 ; i<ITER ; ++i)
    {
         pom = randChromosom(G, maxPathLength);
         pom1 = eval(G, pom, maxPathLength);
         
         if(pom1 < bestScore)
         {
              bestCh = pom;
              bestScore = pom1;   
         }
    }
    
    cout<<bestScore;
    
    
    return 0;    
}

//penalty for every next km after maxPathLength reached
double eval(const Graph &G, const chromosom &individual, double maxPathLength, double penalty)
{
    double ret = 0;
    
    //iterate over paths
    chromosom::const_iterator ch_it;
    for( ch_it = individual.begin(); ch_it != individual.end(); ++ch_it)
    {
         //iterate over city in path
         vector<int>::const_iterator path_it = (*ch_it).begin();
         double pathLength = G[0][*path_it];
    
        for(; (path_it+1) != (*ch_it).end() ; ++path_it)
             ret+=G[*path_it][*(path_it+1)];
        pathLength+=G[*path_it][0];
         
         ret += pathLength;
         if(pathLength > maxPathLength)
             ret += (pathLength - maxPathLength)*(penalty - 1);
    }
    
    return ret;
}

chromosom randChromosom(const Graph &G, double maxPathLength)
{
    chromosom ret;
          
    vector<int> pom(G.size() - 1);
    for(int j=1 ; j< G.size() ; ++j)
        pom[j-1]=j;
    //shuffle city order
    std::random_shuffle ( pom.begin(), pom.end() );
    
    //start and end of creating new paths
    vector<int>::const_iterator it_start = pom.begin(), it_end = pom.begin() + 1;
    double pathLength = G[0][*it_start];
    for(;it_end != pom.end();  ++it_end)
    {     
          //end path with chance (pathLengtrh/maxAllowedPathLength)^(2.5)
         if(1.0*rand()/RAND_MAX <= pow((pathLength + G[*(it_end-1)][0])/maxPathLength, 2.5) )
         {
              //add salesman path
              ret.push_back(vector<int>(it_start, it_end));
              it_start = it_end;     
              pathLength = G[0][*it_start];
         }
         else
         {
             pathLength += G[*(it_end-1)][*it_end];        
         }
    }
    
    //add salesman path
    ret.push_back(vector<int>(it_start, it_end)); 
    
    return ret;
}
