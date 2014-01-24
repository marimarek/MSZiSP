#include<iostream>
#include<utility>
#include<vector>
#include<set>
#include "graph.h"
#include "population.h"

#define POP_SIZE 100
#define ITER 10000
#define SIZE_GROUP 10
#define MUT_CHANCE 0.05
#define REPR 20

#define PRINT_STEP 100

using namespace std;

double eval(const Graph &G, const chromosom &ind, double maxPathLength, double penalty = 10);

int main()
{   
    //int POP_SIZE, ITER, SIZE_GROUP, MUT_CHANCE, REPR, PRINT_STEP;
    //cin>>POP_SIZE>>ITER>>SIZE_GROUP>>MUT_CHANCE>>REPR>>PRINT_STEP;
    double maxPathLength;
    
    Graph G;
    cin>>G;
    cin>>maxPathLength;

    Population pop(G, eval, POP_SIZE, maxPathLength);
//    cout<<pop<<'\n';
    
    for(int i=0 ; i<ITER ; ++i)
    {
         pop.nextGeneration(SIZE_GROUP, MUT_CHANCE, REPR);    
         //cout<<pop<<'\n';
         if(i%PRINT_STEP == 0)
             cout<<eval(G, pop.bestSolution(), maxPathLength)<<'\n';
    }
    
    cout<<'\n'<<eval(G, pop.bestSolution(), maxPathLength)<<'\n';
    
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
