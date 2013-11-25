#include "graph.h"
#include <iostream>
#include <cstdlib>

#define ITER 2000000
typedef vector<int> chromosom;

using namespace std;

double eval(const Graph &G, const chromosom &ind);
chromosom randChromosom(int cSize);


int main()
{   
    srand(time(NULL));
    
    //int ITER;
    //cin>>ITER;
    
    Graph G;
    cin>>G;
    
    chromosom bestCh = randChromosom(G.size()-1);
    double bestScore = eval(G, bestCh);
    
    
    chromosom pom;
    double pom1;
    
    for(int i=0 ; i<ITER ; ++i)
    {
         pom = randChromosom(G.size()-1);
         pom1 = eval(G, pom);
         
         if(pom1 < bestScore)
         {
              bestCh = pom;
              bestScore = pom1;   
         }
    }
    
    cout<<bestScore<<'\n';
    
    
    return 0;    
}

double eval(const Graph &G, const chromosom &ind)
{
    double ret = 0;
    
    for(chromosom::const_iterator it = ind.begin() ; it != ind.end() ; ++it)
    {
        ret+=G[0][*it];

        for(int i=0 ; i<2 && (it+1) != ind.end() ; ++i, ++it)
             ret+=G[*it][*(it+1)];
        
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
