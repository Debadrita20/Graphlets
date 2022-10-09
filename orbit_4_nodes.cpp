#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <ctime>
#include <iostream>
#include <fstream>
#include <set>
#include <unordered_map>
#include <algorithm>
#include "orca.cpp"
using namespace std;

int rhs[10];  //to store the value for the right-hand-side of the equations
int **orbits;  // orbits[x][o] - how many times does node x participate in orbit o

void resetrhs()
{
	for(int i=0;i<10;i++)
	{
		rhs[i]=0;
	}
}

void count4nodeorbits()
{
    orbits=(int **)malloc(n*sizeof(int *));
    for(int i=0;i<n;i++)
    {
        orbits[i]=(int *)malloc(11*sizeof(int));
    }
    int *countfull=(int *)malloc(n*sizeof(int));
	for(int i=0;i<n;i++)
	{
		countfull[i]=0;
	}
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<11;j++)
		orbits[i][j]=0;
	}
    int *neigh=(int *)malloc(n*sizeof(int));
    int t=0;
    for(int x=0;x<n;x++)
    {
        for (int nx=0;nx<deg[x];nx++) {
			int y=adj[x][nx];  //gets each endpoint of each edge from x, i.e., each adjacent node
			if (y >= x) break;  //so that edges are not worked upon more than once
			t=0;
			//printf("Current Node (y): %d\n",y);
			for (int ny=0;ny<deg[y];ny++) { //iterating over each adjacent node of y
				int z=adj[y][ny];
				if (z >= y) break; //edges are not worked upon more than once and the x-y edge is not considered again 
				if (adjacent(x,z)==0) continue;  //if not adjacent, i.e., no x-z edge
				neigh[t++]=z;  // there exists an x-z edge
				//printf("Neighbour added: %d\n",z);
			}
			for (int i=0;i<t;i++) { //iterating over each neighbour of x
				int z = neigh[i];
				//printf("Inside 2nd inner loop for iterating over neighbours\n");
				//printf("Current Node (z): %d\n",z);
				for (int j=i+1;j<t;j++) {
					int zz = neigh[j];
					//printf("Current Node (zz): %d\n",zz);
					if (adjacent(z,zz)) {
						countfull[x]++; countfull[y]++; countfull[z]++; countfull[zz]++;   //full graphlets
						//printf("count increased\n");
					}
				}
			}
		}
    }
    for(int x=0;x<n;x++)
    {
		resetrhs();
		//cout<<"x="<<x<<endl;
        for(int xi=0;xi<deg[x];xi++)
		{
			int y=adj[x][xi];
			//cout<<"y="<<y<<endl;
			for(int yi=0;yi<deg[y];yi++)
			{
				int z=adj[y][yi];
				//cout<<"z="<<z<<endl;
				if(z==x) continue;
				if(adjacent(x,z))
				{
					if(z<y) {
					int c=0;
                    for(int i1=0,j1=0;i1<deg[y]&&j1<deg[z];)
					{
						if(adj[y][i1]==adj[z][j1])
						{
                            c++; i1++; j1++;
						}
						else if(adj[y][i1]<adj[z][j1])
						{
							i1++;
						}
						else j1++;
					}
					rhs[8]+=(c-1);
					c=0;
					for(int i1=0,j1=0;i1<deg[y]&&j1<deg[x];)
					{
						if(adj[y][i1]==adj[x][j1])
						{
                            c++; i1++; j1++;
						}
						else if(adj[y][i1]<adj[x][j1])
						{
							i1++;
						}
						else j1++;
					}
					rhs[9]+=(c-1);
					c=0;
					for(int i1=0,j1=0;i1<deg[z]&&j1<deg[x];)
					{
						if(adj[z][i1]==adj[x][j1])
						{
                            c++; i1++; j1++;
						}
						else if(adj[z][i1]<adj[x][j1])
						{
							i1++;
						}
						else j1++;
					}
					rhs[9]+=(c-1);
					}
				}
				else
				{
					rhs[0]+=(deg[z]-1);
					rhs[1]+=(deg[x]-1);
					rhs[2]+=(deg[y]-2);
					//cout<<rhs[0]<<" "<<rhs[1]<<" "<<rhs[2]<<endl;
					int c=0;
                    for(int i1=0,j1=0;i1<deg[y]&&j1<deg[z];)
					{
						if(adj[y][i1]==adj[z][j1])
						{
                            c++; i1++; j1++;
						}
						else if(adj[y][i1]<adj[z][j1])
						{
							i1++;
						}
						else j1++;
					}
					rhs[5]+=c;
					c=0;
					for(int i1=0,j1=0;i1<deg[y]&&j1<deg[x];)
					{
						if(adj[y][i1]==adj[x][j1])
						{
                            c++; i1++; j1++;
						}
						else if(adj[y][i1]<adj[x][j1])
						{
							i1++;
						}
						else j1++;
					}
					rhs[6]+=c;
					c=0;
					for(int i1=0,j1=0;i1<deg[z]&&j1<deg[x];)
					{
						if(adj[z][i1]==adj[x][j1])
						{
                            c++; i1++; j1++;
						}
						else if(adj[z][i1]<adj[x][j1])
						{
							i1++;
						}
						else j1++;
					}
					rhs[4]+=(c-1);
				}
			}
			//cout<<rhs[0]<<" "<<rhs[1]<<" "<<rhs[2]<<" "<<rhs[3]<<" "<<rhs[4]<<" "<<rhs[5]<<" "<<rhs[6]<<" "<<rhs[7]<<" "<<rhs[8]<<" "<<rhs[9]<<endl;
			for(int xj=0;xj<deg[x];xj++)
			{
				int z=adj[x][xj];
				if(z>=y) break;
				if(!adjacent(z,y))
				{
					rhs[3]+=(deg[x]-2);
					int c=0,abc;
					/*cout<<"asdfghh"<<endl;
					cout<<"x="<<x<<"a="<<y<<"b="<<z<<endl;
					cout<<"Continue?"<<endl;
					cin>>abc;*/
					for(int i1=0,j1=0;i1<deg[y]&&j1<deg[x];)
					{
						if(adj[y][i1]==adj[x][j1])
						{
                            c++; i1++; j1++;
						}
						else if(adj[y][i1]<adj[x][j1])
						{
							i1++;
						}
						else j1++;
					}
					for(int i1=0,j1=0;i1<deg[z]&&j1<deg[x];)
					{
						if(adj[z][i1]==adj[x][j1])
						{
                            c++; i1++; j1++;
						}
						else if(adj[z][i1]<adj[x][j1])
						{
							i1++;
						}
						else j1++;
					}
					rhs[7]+=c;
				}
			}
			//cout<<rhs[0]<<" "<<rhs[1]<<" "<<rhs[2]<<" "<<rhs[3]<<" "<<rhs[4]<<" "<<rhs[5]<<" "<<rhs[6]<<" "<<rhs[7]<<" "<<rhs[8]<<" "<<rhs[9]<<endl;
		}
		//cout<<rhs[0]<<" "<<rhs[1]<<" "<<rhs[2]<<" "<<rhs[3]<<" "<<rhs[4]<<" "<<rhs[5]<<" "<<rhs[6]<<" "<<rhs[7]<<" "<<rhs[8]<<" "<<rhs[9]<<endl;
        orbits[x][10]=countfull[x];
		orbits[x][9]=(int)(0.5*(rhs[9]-6*orbits[x][10]));
		orbits[x][8]=rhs[8]-3*orbits[x][10];
		orbits[x][7]=(int)(0.5*(rhs[7]-2*orbits[x][9]));
		orbits[x][6]=rhs[6]-2*orbits[x][8];
		orbits[x][5]=(int)(0.5*(rhs[5]-2*orbits[x][8]));
		orbits[x][4]=(int)(0.5*(rhs[4]-2*orbits[x][8]));
		orbits[x][3]=(rhs[3]-2*orbits[x][7]-orbits[x][9])/3;
		orbits[x][2]=(rhs[2]-2*orbits[x][5]-orbits[x][6]-2*orbits[x][8])/2;
        orbits[x][1]=rhs[1]-2*orbits[x][4]-orbits[x][6]-2*orbits[x][8];
		orbits[x][0]=rhs[0]-2*orbits[x][4]-2*orbits[x][5]-2*orbits[x][8];
    }
	//cout<<"hello"<<endl;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<11;j++)
		{
			printf("%d ",orbits[i][j]);
		}
		printf("\n");
	}
}

int main()
{
    fin.open("graph.in", fstream::in);
    // read input graph
	fin >> n >> m;
	int d_max=0;
	edges = (PAIR*)malloc(m*sizeof(PAIR));
	deg = (int*)calloc(n,sizeof(int));
	for (int i=0;i<m;i++) {
		int a,b;
		fin >> a >> b;
		if (!(0<=a && a<n) || !(0<=b && b<n)) {
			cerr << "Node ids should be between 0 and n-1." << endl;
			return 0;
		}
		if (a==b) {
			cerr << "Self loops (edge from x to x) are not allowed." << endl;
			return 0;
		}
		deg[a]++; deg[b]++;
		edges[i]=PAIR(a,b);
	}
	for (int i=0;i<n;i++) d_max=max(d_max,deg[i]);
	printf("nodes: %d\n",n);
	printf("edges: %d\n",m);
	printf("max degree: %d\n",d_max);
	fin.close();
	if ((int)(set<PAIR>(edges,edges+m).size())!=m) {
		cerr << "Input file contains duplicate undirected edges." << endl;
		return 0;
	}
	// set up adjacency matrix if it's smaller than 100MB
	if ((int64)n*n < 100LL*1024*1024*8) {
		adjacent = adjacent_matrix;
		adj_matrix = (int*)calloc((n*n)/adj_chunk+1,sizeof(int));
		for (int i=0;i<m;i++) {
			int a=edges[i].a, b=edges[i].b;
			adj_matrix[(a*n+b)/adj_chunk]|=(1<<((a*n+b)%adj_chunk));
			adj_matrix[(b*n+a)/adj_chunk]|=(1<<((b*n+a)%adj_chunk));
		}
	} else {
		adjacent = adjacent_list;
	}
	// set up adjacency, incidence lists
	adj = (int**)malloc(n*sizeof(int*));
	for (int i=0;i<n;i++) adj[i] = (int*)malloc(deg[i]*sizeof(int));
	inc = (PII**)malloc(n*sizeof(PII*));
	for (int i=0;i<n;i++) inc[i] = (PII*)malloc(deg[i]*sizeof(PII));
	int *d = (int*)calloc(n,sizeof(int));
	for (int i=0;i<m;i++) {
		int a=edges[i].a, b=edges[i].b;
		adj[a][d[a]]=b; adj[b][d[b]]=a;
		inc[a][d[a]]=PII(b,i); inc[b][d[b]]=PII(a,i);
		d[a]++; d[b]++;
	}
	for (int i=0;i<n;i++) {
		sort(adj[i],adj[i]+deg[i]);
		sort(inc[i],inc[i]+deg[i]);
	}
    count4nodeorbits();
    return 0;
}