#ifndef POPULATION_H_
#define POPULATION_H_

#include<vector>
#include<iostream>
#include<utility>
#include "graph.h"

using std::vector;
using std::pair;
using std::istream;
using std::ostream;

typedef vector<int> chromosom;
typedef double (*evalFunction_ptr)(const Graph &, const chromosom &, double, double);

class Population
{
        vector<chromosom > pop;
        int pSize;
        
        Graph &G;
        evalFunction_ptr eval; 
        double maxPathLength;
        double penalty;
        
        void mutation(chromosom &v);
        void CR(const chromosom &P1, const chromosom &P2);
        void selection(int groupSize);
        
        pair<chromosom, chromosom> chooseParents(int groupSize);
    public:
        Population(Graph &G1, evalFunction_ptr eval1, double maxPathLength1, double penalty1 = 10);
        Population(Graph &G1, evalFunction_ptr eval1, int pSize1, double maxPathLength1, double penalty1 = 10);
        Population(Graph &G1, evalFunction_ptr eva1l, const vector<chromosom > & pop, double maxPathLength1, double penalty1 = 10);
        
        void randomPopulation(int pSize);
        void randomPopulation1(int pSize1);
        void randomPopulation2(int pSize1);
        
        bool add(chromosom);
        
        void nextGeneration(int groupSize,  double mutChance, int reproduction);
        
        chromosom bestSolution();
        
        friend istream & operator>>(istream & in, Population & P);
        friend ostream & operator<<(ostream & out, const Population & P);
    
};

#endif
