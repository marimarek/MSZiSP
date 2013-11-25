#include "population.h"
#include<cstdlib>

using std::swap;
using std::make_pair;

Population::Population(Graph &G1, double (*eval1)(const Graph &G, const chromosom &ind)) : G(G1), eval(eval1), pSize(0)
{   
    srand(time(NULL));   
}

Population::Population(Graph &G1, double (*eval1)(const Graph &G, const chromosom &ind), int pSize1, int cSize)
   : G(G1), eval(eval1)
{
    srand(time(NULL));  
    randomPopulation(pSize1, cSize);
}

Population::Population(Graph &G1, double (*eval1)(const Graph &G, const chromosom &ind), const vector<chromosom > &v)
    : G(G1), eval(eval1)
{
    srand(time(NULL));  
    
    pop = v;
    pSize = pop.size();
}

void Population::randomPopulation(int pSize1, int cSize)
{
    pSize = pSize1;
    pop = vector<chromosom>(pSize, chromosom(cSize));
    
    vector<int> pom(cSize);
    
    
    for(int i=0 ; i<pSize ; ++i)    
    {
        for(int j=0 ; j<cSize ; ++j)
            pom[j]=j+1;
            
        for(int j=0 ; j<cSize ; ++j)
        {
             int index = rand()%(cSize-j);
             
             pop[i][j] = pom[index]; 
             
             swap(pom[index],pom[cSize-j-1]);
        } 
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
        double worstCost = eval(G, pop[worst]);
        
        for(int i=1 ; i<groupSize ; ++i)
        {
            int pom = rand()%pop.size();
            double pomCost = eval(G, pop[pom]);
            
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
        double bestCost = eval(G, pop[best]);
        
        for(int i=1 ; i<groupSize ; ++i)
        {
            int pom = rand()%pSize;
            double pomCost = eval(G, pop[pom]);
            
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
    }
    
    for(int i=0 ; i<pop.size();++i)
        if(1.0*rand()/RAND_MAX < mutChance)
            mutation(pop[i]);
            
    selection(groupSize);
}

void Population::mutation(chromosom &v)
{
    int i1 = rand()%v.size();
    int i2 = rand()%v.size();
    
    swap(v[i1], v[i2]);
}

void Population::CR(const chromosom &P1, const chromosom &P2)
{
    int start = rand()%(P1.size()-3);
    int end = start + 3 + rand()%(P1.size() - start - 3);
    
    chromosom ch1, ch2;
    vector<bool> used1(P1.size()+1, false), used2(P1.size()+1, false);
    
    for(int i=start ; i<end ; ++i)
    {
        ch1.push_back(P1[i]);
        used1[P1[i]] = true;
        
        ch2.push_back(P2[i]);
        used2[P2[i]] = true;
    }
    
    for(int i=0 ; i<P1.size() ; ++i)
    {
        if(!used1[P2[i]])
            ch1.push_back(P2[i]);
        
        if(!used2[P1[i]])
            ch2.push_back(P1[i]);
    }
    
    pop.push_back(ch1);
    pop.push_back(ch2);
}


chromosom Population::bestSolution()
{
      int best = 0;
      double bestCost = eval(G ,pop[0]);
      for(int i=1 ; i<pSize ; ++i)
      {
            double pomCost =  eval(G ,pop[i]);
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
    in>>pSize>>cSize;
    
    P.pop = vector<chromosom>(pSize, chromosom(cSize, 0) );
    for(int i=0 ; i<pSize ; ++i)
        for(int j=0 ; j<cSize ; ++j)
            in>>P.pop[i][j];
    
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
