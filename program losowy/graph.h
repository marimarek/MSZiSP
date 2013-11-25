#ifndef GRAPH_H_
#define GRAPH_H_

#include<vector>
#include<iostream>

using std::vector;
using std::istream;

class Graph 
{
        vector<vector<double> > G;
        
    public:
        Graph(){}
        
        const vector<double> & operator[](int i) const;
        double cost(int a, int b) const;
        int size() const;
        
        friend istream & operator>>(istream & in, Graph & G);
    
};

#endif
