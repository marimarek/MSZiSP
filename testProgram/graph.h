#ifndef GRAPH_H_
#define GRAPH_H_

#include<vector>
#include<iostream>
#include<utility>
#include<cmath>

using namespace std;

class Graph 
{
        vector<vector<double> > G;
        
    public:
        Graph(){}
        
        const vector<double> & operator[](int i) const;
        double cost(int a, int b) const;
        int size() const;
        void setPoints(vector<pair<int, int> >& points);
        
        friend istream & operator>>(istream & in, Graph & G);
    
};

#endif
