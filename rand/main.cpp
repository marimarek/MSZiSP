#include "graph.h"
#include <iostream>
#include <cmath>

#define ITER 1000000
typedef vector<int> chromosom;

using namespace std;

double eval(const Graph &G, const chromosom &individual, double maxPathLength, double penalty = 10);
chromosom randChromosom(const Graph &G, double maxPathLength);
chromosom randChromosom1(const Graph &G, double maxPathLength);
chromosom randChromosom2(const Graph &G, double maxPathLength);

int main()
{   
    srand(time(NULL));
    
    //int ITER;
    //cin>>ITER;
    double maxPathLength;
    
    Graph G;
    cin>>G;
    cin>>maxPathLength;
    
    chromosom bestCh = randChromosom2(G, maxPathLength);
    double bestScore = eval(G, bestCh, maxPathLength);
    
    
    chromosom pom;
    double pom1;
    
    for(int i=0 ; i<ITER ; ++i)
    {
         pom = randChromosom2(G, maxPathLength);
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
    
    chromosom::const_iterator it = individual.begin() + 1;
    double pathLength = 0;
    for( ; it != individual.end(); ++it)
    {
         pathLength += G[*(it-1)][*it];
         if(*it == 0)
         {
             ret += pathLength;
             if(pathLength > maxPathLength)
                 ret += (pathLength - maxPathLength) *(penalty -1);
             pathLength = 0;
         }
    }
    
    return ret;
}

chromosom randChromosom(const Graph &G, double maxPathLength)
{
    chromosom ret;
          
    vector<int> pom(G.size()-1);
    for(int j=1 ; j< G.size() ; ++j)
        pom[j-1]=j;
    //shuffle city order
    std::random_shuffle ( pom.begin(), pom.end() );
    
    ret.push_back(0);
    vector<int>::const_iterator it = pom.begin();
    double pathLength;
    for( it; it != pom.end(); ++it)
    {
         pathLength += G[*(ret.end()-1)][*it];
         ret.push_back(*it);
         if(1.0*rand()/RAND_MAX <= pow((pathLength + G[*it][0])/maxPathLength, 2.5))
         {
             ret.push_back(0);
             pathLength = 0;
         }
    }
    ret.push_back(0);
    
    return ret;
}


chromosom randChromosom1(const Graph &G, double maxPathLength)
{
    chromosom ret;
        
    vector<int> pom(G.size()-1);
    for(int j=1 ; j< G.size() ; ++j)
        pom[j-1]=j;
    //shuffle city order
    std::random_shuffle ( pom.begin(), pom.end() );
    
    ret.push_back(0);
    vector<int>::const_iterator it = pom.begin();
    for( it; it != pom.end(); ++it)
    {
         ret.push_back(*it);
         if(1.0*rand()/RAND_MAX <= 0.9)
             ret.push_back(0);
    }
    if(*(ret.end()-1) != 0)
        ret.push_back(0);
    
    return ret;
}


chromosom randChromosom2(const Graph &G, double maxPathLength)
{
    chromosom ret;
        
    vector<int> pom(G.size()-1);
    for(int j=1 ; j< G.size() ; ++j)
        pom[j-1]=j;
    //shuffle city order
    std::random_shuffle ( pom.begin(), pom.end() );
    
    double pathLength = G[0][pom[0]];
    for(int j=1 ; j< pom.size() ; ++j)
        pathLength += G[pom[j-1]][pom[j]];
    pathLength += G[*(pom.end()-1)][0];    
    
    ret.push_back(0);
    vector<int>::const_iterator it = pom.begin();
    for( it; it != pom.end(); ++it)
    {
         ret.push_back(*it);
         if(1.0*rand()/RAND_MAX < 1.0/(pathLength/maxPathLength))
             ret.push_back(0);
    }
    if(*(ret.end()-1) != 0)
        ret.push_back(0);
    
    return ret;
}

