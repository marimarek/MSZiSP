#include<iostream>
#include<utility>
#include<vector>
#include<set>
#include "graph.h"
#include "population.h"

#define POP 100
#define ITER 1000
#define ITER1 200000
#define SIZE_GROUP 10
#define MUT_CHANCE 0.01
#define REPR 20

#define PRINT_STEP 100
typedef vector<int> chromosom;

using namespace std;

double eval(const Graph &G, const chromosom &ind);
chromosom randChromosom(int cSize);

vector<pair<int, int> > points;


   

int main()
{   
    //int POP, ITER, SIZE_GROUP, MUT_CHANCE, REPR, PRINT_STEP;
    //cin>>POP>>ITER>>SIZE_GROUP>>MUT_CHANCE>>REPR>>PRINT_STEP;
     
    
     int n;
     int w,h;
     int iter;
     
     double gbestScore1 = 0;
     double gbestScore = 0;
     
     double bestDiff = 0;
     double dbestScore1 = 0;
     double dbestScore = 0;
     
     double worstDiff = 999999999;
     double dworstScore1 = 0;
     double dworstScore = 0;
     
     cin>>n>>w>>h>>iter;
     for(int k = 0; k < iter; ++k)
     {
             
     Graph G;
     G.setPoints(points);
    
    Population pop(G, eval);
    pop.randomPopulation(POP, G.size()-1);
    //cout<<pop<<'\n';
    
    for(int i=0 ; i<ITER ; ++i)
    {
         pop.nextGeneration(SIZE_GROUP, MUT_CHANCE, REPR);    
         //cout<<pop<<'\n';
         /*if(i%PRINT_STEP == 0)
             cout<<eval(G, pop.bestSolution())<<'\n';*/
    }
    
    double bestScore1 = eval(G, pop.bestSolution());
    
    chromosom bestCh = randChromosom(G.size()-1);
    double bestScore = eval(G, bestCh);
    
    chromosom pom;
    double pom1;
    
    for(int i=0 ; i<ITER1 ; ++i)
    {
         pom = randChromosom(G.size()-1);
         pom1 = eval(G, pom);
         
         if(pom1 < bestScore)
         {
              bestCh = pom;
              bestScore = pom1;   
         }
    }
    
    if(bestDiff < bestScore1/bestScore)
    {
        bestDiff = bestScore1/bestScore;
        dbestScore1 = bestScore1;
        dbestScore = bestScore;
    }
    
    if(worstDiff > bestScore1/bestScore)
    {
        worstDiff = bestScore1/bestScore;
        dworstScore1 = bestScore1;
        dworstScore = bestScore;
    }
    
    gbestScore1 += bestScore1;
    gbestScore += bestScore;
    }
    
    cout<<"Best:\n";
    cout<<bestDiff<<'\n';
    cout<<dbestScore1<<'\n';
    cout<<dbestScore<<'\n';
    
    cout<<"Worst:\n";
    cout<<worstDiff<<'\n';
    cout<<dworstScore1<<'\n';
    cout<<dworstScore<<'\n';
    
     cout<<"Average:\n";
    cout<<gbestScore1/gbestScore<<'\n';
    cout<<gbestScore1<<'\n';
    cout<<gbestScore<<'\n';
    
    return 0;    
}

double eval(const Graph &G, const chromosom &ind)
{
    double ret = 0;
    
    for(chromosom::const_iterator it = ind.begin() ; it != ind.end() ; ++it)
    {
        ret+=G[0][*it];

        for(int i=0 ; i<2 && (it+1) != ind.end() ; ++i, ++it)
        {
             ret+=G[*it][*(it+1)];
        }
        
        ret+=G[*it][0];
    }
    
    return ret;
}

chromosom randChromosom(int cSize)
{
    vector<int> ret(cSize);
    vector<int> pom(cSize);
    
    
    for(int j=0 ; j<cSize ; ++j)
        pom[j]=j+1;
            
    for(int j=0 ; j<cSize ; ++j)
    {
         int index = rand()%(cSize-j);
             
         ret[j]=pom[index]; 
             
         swap(pom[index],pom[cSize-j-1]);
    }  
    
    return ret;
}
