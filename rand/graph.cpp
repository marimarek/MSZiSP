#include "graph.h"


const vector<double> & Graph::operator[](int i) const
{
    return G[i];
}

double Graph::cost(int a, int b) const
{
    return G[a][b];    
}

int Graph::size() const
{
    return G.size();
}

istream & operator>>(istream & in, Graph & g)
{
    int n;
    in>>n;
    
    g.G.resize(n, vector<double>(n, 0.0));
    
    int a,b;
    for(int i=0 ; i<n*n ; ++i)
    {
        in>>a>>b;
        in>>g.G[a][b];
    }    
        
    return in;
}

