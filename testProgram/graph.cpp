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

void Graph::setPoints(vector<pair<int, int> >& points)
{
  G.resize(points.size(), vector<double>(points.size(), 0.0));   
     
  for(int a=0 ; a<points.size() ; ++a)
    for(int b=0 ; b<points.size() ; ++b)
    {
       double dx=points[a].first-points[b].first;
       double dy=points[a].second-points[b].second;
       G[a][b] = sqrt(dx*dx + dy*dy);
    }
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

