#include<iostream>
#include<utility>
#include<vector>
#include<set>
#include "graph.h"
#include "population.h"

#define POP 100
#define ITER 10000
#define SIZE_GROUP 10
#define MUT_CHANCE 0.01
#define REPR 20

#define PRINT_STEP 100

using namespace std;

double eval(const Graph &G, const chromosom &ind);

int main()
{   
    //int POP, ITER, SIZE_GROUP, MUT_CHANCE, REPR, PRINT_STEP;
    //cin>>POP>>ITER>>SIZE_GROUP>>MUT_CHANCE>>REPR>>PRINT_STEP;
    
    Graph G;
    cin>>G;
    
    Population pop(G, eval);
    pop.randomPopulation(POP, G.size()-1);
    //cout<<pop<<'\n';
    
    for(int i=0 ; i<ITER ; ++i)
    {
         pop.nextGeneration(SIZE_GROUP, MUT_CHANCE, REPR);    
         //cout<<pop<<'\n';
         if(i%PRINT_STEP == 0)
             cout<<eval(G, pop.bestSolution())<<'\n';
    }
    
    cout<<'\n'<<eval(G, pop.bestSolution())<<'\n';
    
    return 0;    
}

double eval(const Graph &G, const chromosom &ind)
{
    double ret = 0;
    
    return ret;
}
