#include<iostream>
#include<cstdio>
#include<cmath>
#include<utility>
#include<vector>

using namespace std;

vector<pair<int, int> > points;

int main()
{
    srand(time(NULL));
    
     int n;
     int w,h;
     
     cin>>n>>w>>h;
     
     for(int i=0 ; i<n ; ++i)
             points.push_back(make_pair(rand()%w,rand()%h));
             
     cout<<n<<'\n';
     for(int a=0 ; a<n ; ++a)
        for(int b=0 ; b<n ; ++b)
        {
           double dx=points[a].first-points[b].first;
           double dy=points[a].second-points[b].second;
           cout<<a<<' '<<b<<' '<<sqrt(dx*dx + dy*dy)<<'\n';
        }
     
     double avergaDistanceX = w/(n+2.0);
     double avergaDistanceY = h/(n+2.0);
     double averageDistance = sqrt(avergaDistanceX*avergaDistanceX + avergaDistanceY*avergaDistanceY);
     cout<<averageDistance*(n/5.0)<<'\n';
     return 0;    
}
