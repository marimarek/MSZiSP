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
    randomPopulation(pSize1);
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
        
        //start and end of creating new paths
        vector<int>::const_iterator it_start = pom.begin(), it_end = pom.begin() + 1;
        double pathLength = G[0][*it_start];
        for(;it_end != pom.end();  ++it_end)
        {     
              //end path with chance (pathLengtrh/maxAllowedPathLength)^(2.5)
             if(1.0*rand()/RAND_MAX <= pow((pathLength + G[*(it_end-1)][0])/maxPathLength, 2.5) )
             {
                  //add salesman path
                  pop[i].push_back(vector<int>(it_start, it_end));
                  it_start = it_end;     
                  pathLength = G[0][*it_start];
             }
             else
             {
                 pathLength += G[*(it_end-1)][*it_end];        
             }
        }
        
        //add salesman path
        pop[i].push_back(vector<int>(it_start, it_end));
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
    int x1 = rand()%v.size();
    int y1 = rand()%v[x1].size();
    int x2 = rand()%v.size();
    int y2 = rand()%v[x2].size();
    
    swap(v[x1][y1], v[x2][y2]);
}

void Population::CR(const chromosom &P1, const chromosom &P2)
{     
    int start = rand()%P1.size();
    int end = start + rand()%(P1.size() - start);
    
    chromosom ch;
    vector<bool> used(G.size(), false);
    
    for(int i=start ; i<end ; ++i)
    {
        ch.push_back(P1[i]);
        for(int j = 0; j < P1[i].size(); ++j)
           used[P1[i][j]] = true;
    }
    
    vector<int> pom;
    for(int i = 0; i < P2.size(); ++i)
        for(int j = 0; j < P2[i].size(); ++j)
            if( !used[P2[i][j]] )
                 pom.push_back(P2[i][j]);
    
    if(pom.size() > 0)
    {     
        vector<int>::const_iterator it_start = pom.begin(), it_end = pom.begin() + 1;
        double pathLength = G[0][*it_start];
        for(;it_end != pom.end();  ++it_end)
        {     
              //end path with chance (pathLengtrh/maxAllowedPathLength)^(2.5)
             if(1.0*rand()/RAND_MAX <= pow((pathLength + G[*(it_end-1)][0])/maxPathLength, 2.5) )
             {
                  //add salesman path
                  ch.push_back(vector<int>(it_start, it_end));
                  it_start = it_end;     
                  pathLength = G[0][*it_start];
             }
             else
             {
                 pathLength += G[*(it_end-1)][*it_end];        
             }
        }
        
        //add salesman path
        ch.push_back(vector<int>(it_start, it_end));
    }
    
    pop.push_back(ch);
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
    in>>P.pSize;
    
    P.pop = vector<chromosom>(P.pSize);
    for(int i=0 ; i<P.pSize ; ++i)
    {
        int numOfPaths;
        in>>numOfPaths;
        P.pop[i] = chromosom(numOfPaths);
        for(int j=0 ; j<numOfPaths ; ++j)
        {
            int numOfCityInPath;
            in>>numOfCityInPath;
            P.pop[i][j] = vector<int>(numOfCityInPath);   
            for(int k=0 ; k<numOfCityInPath ; ++k)
                in>>P.pop[i][j][k];
        }
    }
    
    return in;
}

ostream & operator<<(ostream & out, const Population & P)
{
    out<<P.pSize<<'\n';
    
    for(int i=0 ; i<P.pSize ; ++i)
    {
        out<<P.pop[i].size()<<'\n';
        for(int j=0 ; j<P.pop[i].size() ; ++j)
        {
            out<<P.pop[i][j].size()<<'\n';
            for(int k=0 ; k<P.pop[i][j].size() ; ++k)
                out<<P.pop[i][j][k]<<' ';
            out<<'\n';
        } 
    }
    
    return out;
}
