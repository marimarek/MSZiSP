#include "population.h"

#include <algorithm>
#include <cmath>

using std::swap;
using std::make_pair;
using std::pow;

Population::Population(Graph &G1, evalFunction_ptr eval1, double maxPathLength1, double penalty1) : G(G1), eval(eval1), pSize(0), maxPathLength(maxPathLength1), penalty(penalty1)
{   
    srand(time(NULL));   
}

Population::Population(Graph &G1, evalFunction_ptr eval1, int pSize1, double maxPathLength1, double penalty1)
   : G(G1), eval(eval1), maxPathLength(maxPathLength1), penalty(penalty1)
{
    srand(time(NULL));  
    randomPopulation2(pSize1);
}

Population::Population(Graph &G1, evalFunction_ptr eval1, const vector<chromosom > &v, double maxPathLength1, double penalty1)
    : G(G1), eval(eval1), maxPathLength(maxPathLength1), penalty(penalty1)
{
    srand(time(NULL));  
    
    pop = v;
    pSize = pop.size();
}

void Population::randomPopulation(int pSize1)
{
    
    pSize = pSize1;
    pop = vector<chromosom>(pSize);
    
    if( G.size() < 2)
        return;
    
    //tmp helper vector with city order
    vector<int> pom(G.size() - 1);

    for(int i=0 ; i<pSize ; ++i)    
    {
        for(int j=1 ; j< G.size() ; ++j)
            pom[j-1]=j;
        //shuffle city order
        std::random_shuffle ( pom.begin(), pom.end() );
        
        pop[i].push_back(0);
        vector<int>::const_iterator it = pom.begin();
        double pathLength;
        for( it; it != pom.end(); ++it)
        {
             pathLength += G[*(pop[i].end()-1)][*it];
             pop[i].push_back(*it);
             if(1.0*rand()/RAND_MAX <= pow((pathLength + G[*it][0])/maxPathLength, 2.5))
             {
                 pop[i].push_back(0);
                 pathLength = 0;
             }
        }
        if(*(pop[i].end()-1) != 0)
            pop[i].push_back(0);
    }
}

void Population::randomPopulation1(int pSize1)
{
    
    pSize = pSize1;
    pop = vector<chromosom>(pSize);
    
    if( G.size() < 2)
        return;
    
    //tmp helper vector with city order
    vector<int> pom(G.size() - 1);

    for(int i=0 ; i<pSize ; ++i)    
    {
        for(int j=1 ; j< G.size() ; ++j)
            pom[j-1]=j;
        //shuffle city order
        std::random_shuffle ( pom.begin(), pom.end() );
        
        pop[i].push_back(0);
        vector<int>::const_iterator it = pom.begin();
        for( it; it != pom.end(); ++it)
        {
             pop[i].push_back(*it);
             if(1.0*rand()/RAND_MAX <= 0.8)
                 pop[i].push_back(0);
        }
        pop[i].push_back(0);
    }
}


void Population::randomPopulation2(int pSize1)
{
    
    pSize = pSize1;
    pop = vector<chromosom>(pSize);
    
    if( G.size() < 2)
        return;
    
    //tmp helper vector with city order
    vector<int> pom(G.size() - 1);

    for(int i=0 ; i<pSize ; ++i)    
    {
        for(int j=1 ; j< G.size() ; ++j)
            pom[j-1]=j;
        //shuffle city order
        std::random_shuffle ( pom.begin(), pom.end() );
        
        double pathLength = G[0][pom[0]];
        for(int j=1 ; j< pom.size() ; ++j)
            pathLength += G[pom[j-1]][pom[j]];
        pathLength += G[*(pom.end()-1)][0];    
        
        pop[i].push_back(0);
        vector<int>::const_iterator it = pom.begin();
        for( it; it != pom.end(); ++it)
        {
             pop[i].push_back(*it);
             if(1.0*rand()/RAND_MAX < 1.0/(pathLength/maxPathLength))
                 pop[i].push_back(0);
        }
        if(*(pop[i].end()-1) != 0)
            pop[i].push_back(0);
    }
}

bool Population::add(chromosom c)
{
    if(!pop.empty())
        if(c.size() != pop[0].size())
            return false;
        
    pop.push_back(c);
    ++pSize;
    return true;
}

void Population::selection(int groupSize)
{
    while(pop.size()>pSize)
    {
        int worst = rand()%pop.size();
        double worstCost = eval(G, pop[worst], maxPathLength, penalty);
        
        for(int i=1 ; i<groupSize ; ++i)
        {
            int pom = rand()%pop.size();
            double pomCost = eval(G, pop[pom], maxPathLength, penalty);
            
            if(pomCost>worstCost)
            {
                worst = pom;
                worstCost = pomCost;    
            }
        }
        
        swap(pop[worst], pop[pop.size()-1]);
        pop.pop_back();
    }    
}

pair<chromosom, chromosom> Population::chooseParents(int groupSize)
{
    chromosom ret[2];
    
    for(int j=0 ; j<2 ; ++j)    
    {
        int best = rand()%pSize;
        double bestCost = eval(G, pop[best], maxPathLength, penalty);
        
        for(int i=1 ; i<groupSize ; ++i)
        {
            int pom = rand()%pSize;
            double pomCost = eval(G, pop[pom], maxPathLength, penalty);
            
            if(pomCost<bestCost)
            {
                best = pom;
                bestCost = pomCost;    
            }
        }
        
        ret[j]=pop[best];
    }
    
    return make_pair(ret[0], ret[1]);
}

void Population::nextGeneration(int groupSize,  double mutChance, int reproduction)
{
    for(int i=0 ; i<reproduction ; ++i)
    {  
        pair<chromosom, chromosom> pom = chooseParents(groupSize);
        CR(pom.first, pom.second); 
        CR(pom.second, pom.first);
    }
    
    for(int i=0 ; i<pop.size();++i)
        if(1.0*rand()/RAND_MAX < mutChance)
            mutation(pop[i]);
            
    selection(groupSize);
}

void Population::mutation(chromosom &v)
{
    int i1 = rand()%(v.size()-2)+1;
    int i2 = rand()%(v.size()-2)+1;
    
    swap(v[i1], v[i2]);
    if(v[i1] == 0 &&(v[i1+1] == 0 || v[i1-1] == 0))
    {
        v.erase(v.begin()+i1);
        if(i2 >= i1)
            --i2;
    }
    if(v[i2] == 0 &&(v[i2+1] == 0 || v[i2-1] == 0))
       v.erase(v.begin()+i2);
}

void Population::CR(const chromosom &P1, const chromosom &P2)
{
    int start = rand()%(P1.size()-3);
    int end = start + 3 + rand()%(P1.size() - start - 3);
    
    chromosom cildren;
    vector<bool> used(P1.size()+1, false);
    
    if(P1[start] != 0)
        cildren.push_back(0);
    
    for(int i=start ; i<end ; ++i)
    {
        cildren.push_back(P1[i]);
        if(P1[i] != 0)
            used[P1[i]] = true;
    }
    
    for(int i=0 ; i<P2.size() ; ++i)
    {
        if(!used[P2[i]] && !(P2[i] == 0 && *(cildren.end()-1) == 0) && !(P2[i] == 0 && i >= start && i < end))
            cildren.push_back(P2[i]);
    }
    if(*(cildren.end()-1) != 0)
        cildren.push_back(0);  
                           
    pop.push_back(cildren);
}

chromosom Population::bestSolution()
{
      int best = 0;
      double bestCost = eval(G ,pop[0], maxPathLength, penalty);
      for(int i=1 ; i<pSize ; ++i)
      {
            double pomCost =  eval(G ,pop[i], maxPathLength, penalty);
            if(pomCost<bestCost)
            {
                best = i;    
                bestCost = pomCost;
            }
      }  
      
      return pop[best];
}


istream & operator>>(istream & in, Population & P)
{
    int pSize, cSize;
    in>>pSize;

    P.pop = vector<chromosom>(pSize );
    for(int i=0 ; i<pSize ; ++i)
    {
        in>>cSize;
        P.pop[i] = chromosom(cSize);
        for(int j=0 ; j<cSize ; ++j)
            in>>P.pop[i][j];
    }

    return in;
}

ostream & operator<<(ostream & out, const Population & P)
{
    out<<P.pSize<<'\n';

    for(int i=0 ; i<P.pSize ; ++i)
    {
        for(int j=0 ; j<P.pop[i].size() ; ++j)
            out<<P.pop[i][j]<<' ';
        out<<'\n';
    }

    return out;
}
