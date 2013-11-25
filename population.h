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

typedef vector< vector<int> > chromosom;

class Population
{
        vector<chromosom > pop;
        int pSize;
        
        Graph &G;
        double (*eval)(const Graph &G, const chromosom &ind); 
        
        void mutation(chromosom &v);
        void CR(const chromosom &P1, const chromosom &P2);
        void selection(int groupSize);
        
        pair<chromosom, chromosom> chooseParents(int groupSize);
    public:
        Population(Graph &G1, double (*eval1)(const Graph &G, const chromosom &ind));
        Population(Graph &G1, double (*eval1)(const Graph &G, const chromosom &ind), int pSize1, int cSize);
        Population(Graph &G1, double (*eval1)(const Graph &G, const chromosom &ind), const vector<chromosom > &);
        
        void randomPopulation(int pSize, int cSize);
        
        bool add(chromosom);
        
        void nextGeneration(int groupSize,  double mutChance, int reproduction);
        
        chromosom bestSolution();
        
        friend istream & operator>>(istream & in, Population & P);
        friend ostream & operator<<(ostream & out, const Population & P);
    
};

#endif
