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
#include <vector>
#include <sstream>
#include "orca.cpp"
using namespace std;

int rhs[406];  //to store the value for the right-hand-side of the equations
int **orbits;  // orbits[x][o] - how many times does node x participate in orbit o

void resetrhs()
{
	for(int i=0;i<406;i++)
	{
		rhs[i]=0;
	}
}

void countnode6orbits()
{
	fstream fout;
	fout.open("node-6.out_tt",ios::out);
	orbits=(int **)malloc(n*sizeof(int *));
    for(int i=0;i<n;i++)
    {
        orbits[i]=(int *)malloc(480*sizeof(int));
    }
    int *countfull=(int *)malloc(n*sizeof(int));
	for(int i=0;i<n;i++)
	{
		countfull[i]=0;
	}
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<480;j++)
		orbits[i][j]=0;
	}
	int **common=(int **)malloc(n*sizeof(int *));
	for(int i=0;i<n;i++)
	{
		common[i]=(int *)malloc(n*sizeof(int));
	}
	unordered_map<TRIPLE, int, hash_TRIPLE> common3;
	for (int x=0;x<n;x++) {
		for (int n1=0;n1<deg[x];n1++) {
			int a=adj[x][n1];
			for (int n2=n1+1;n2<deg[x];n2++) {
				int b=adj[x][n2];
				for (int n3=n2+1;n3<deg[x];n3++) {
					int c=adj[x][n3];
					TRIPLE abc=TRIPLE(a,b,c);
					common3[abc]++;
				}
			}
		}
	}
	// precompute triangles that span over edges
	int **tri=(int **)malloc(n*sizeof(int *));
	for(int i=0;i<n;i++)
	{
		tri[i]=(int *)malloc(n*sizeof(int));
	}
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			tri[i][j]=0;
		}
	}
	for (int i=0;i<m;i++) {
		int x=edges[i].a, y=edges[i].b;
		for (int xi=0,yi=0; xi<deg[x] && yi<deg[y]; ) {
			if (adj[x][xi]==adj[y][yi]) { tri[x][y]++; tri[y][x]++; xi++; yi++; }
			else if (adj[x][xi]<adj[y][yi]) { xi++; }
			else { yi++; }
		}
	}

    //counting full graphlets
	int *c479=(int *)malloc(n*sizeof(int));
	for(int i=0;i<n;i++)
	{
		c479[i]=0;
	}
    for(int x=0;x<n;x++)
	{
        for(int nx1=0;nx1<deg[x];nx1++)
		{
			int y1=adj[x][nx1];
			for(int nx2=nx1+1;nx2<deg[x];nx2++)
			{
				int y2=adj[x][nx2];
				if(!adjacent(y1,y2)) continue;
				for(int nx3=nx2+1;nx3<deg[x];nx3++)
				{
					int y3=adj[x][nx3];
					if((!adjacent(y3,y1))||(!adjacent(y3,y2))) continue;
					for(int nx4=nx3+1;nx4<deg[x];nx4++)
					{
						int y4=adj[x][nx4];
						if((!adjacent(y4,y1))||(!adjacent(y4,y2))||(!adjacent(y4,y3))) continue;
						for(int nx5=nx4+1;nx5<deg[x];nx5++)
						{
							int y5=adj[x][nx5];
							if((!adjacent(y5,y1))||(!adjacent(y5,y2))||(!adjacent(y5,y3))||(!adjacent(y5,y4))) continue;
							c479[x]++;
							c479[y1]++;
							c479[y2]++;
							c479[y3]++;
							c479[y4]++;
							c479[y5]++;
						}
					}
				}
			}
		}
	}
	for(int x=0;x<n;x++)
	{
		resetrhs();
		//setting up rhs of equations
		for(int nx1=0;nx1<deg[x];nx1++)
		{
			int a=adj[x][nx1];
			for(int nx6=0;nx6<deg[x];nx6++)
			{
				int b=adj[x][nx6];
				if(b==a) continue;
				for(int nx7=0;nx7<deg[x];nx7++)
				{
					int c=adj[x][nx7];
					if(c==b||c==a) continue;
					for(int nx8=0;nx8<deg[x];nx8++)
					{
						int d=adj[x][nx8];
						if(d==a||d==b||d==c) continue;
						if((!adjacent(d,a))&&(!adjacent(d,b))&&(!adjacent(d,c))&&(!adjacent(a,b))&&(!adjacent(a,c))&&(!adjacent(b,c)))  //p23
						{
							rhs[1]+=(deg[x]-4);
							rhs[5]+=(deg[a]-1+deg[b]-1+deg[c]-1+deg[d]-1);
							rhs[8]+=(tri[x][a]+tri[x][b]+tri[x][c]+tri[x][d]);
							rhs[24]+=(tri[a][b]-1+tri[a][c]-1+tri[b][c]-1+tri[a][d]-1+tri[b][d]-1+tri[c][d]-1);
							rhs[28]+=(common3[TRIPLE(x,a,b)]+common3[TRIPLE(x,a,c)]+common3[TRIPLE(x,b,c)]+common3[TRIPLE(x,a,d)]+common3[TRIPLE(x,b,d)]+common3[TRIPLE(x,c,d)]);
							rhs[84]+=(common3[TRIPLE(a,b,c)]-1+common3[TRIPLE(a,b,d)]-1+common3[TRIPLE(a,c,d)]-1+common3[TRIPLE(b,c,d)]-1);
							vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commxb)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[88]++;
							}
							if(find(commad.begin(),commad.end(),v)!=commad.end())
							{
								rhs[88]++;
							}
						}
						for(int v:commxd)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[88]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[88]++;
							}
						}
						for(int v:commad)
						{
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[213]++;
							}
						}
						rhs[213]--;
						}
						else if((adjacent(d,a))&&(!adjacent(d,b))&&(!adjacent(d,c))&&(!adjacent(a,b))&&(!adjacent(a,c))&&(!adjacent(b,c)))  //p33
						{
							rhs[39]+=common3[TRIPLE(x,a,d)];
						}
						else if((adjacent(d,a))&&(adjacent(d,b))&&(!adjacent(d,c))&&(!adjacent(a,b))&&(!adjacent(a,c))&&(!adjacent(b,c)))  //p42
						{
							rhs[105]+=(tri[x][a]-1+tri[x][b]-1);
							rhs[137]+=(common3[TRIPLE(x,a,d)]+common3[TRIPLE(x,b,d)]);
							rhs[164]+=(tri[a][b]-2);
							rhs[183]+=(common3[TRIPLE(x,a,b)]-1);
							rhs[223]+=tri[x][c];
							rhs[239]+=common3[TRIPLE(x,c,d)];
							rhs[268]+=(common3[TRIPLE(x,a,c)]+common3[TRIPLE(x,b,c)]);
							vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commxb)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[298]++;
							}
							if(find(commad.begin(),commad.end(),v)!=commad.end())
							{
								rhs[196]++;
							}
						}
						for(int v:commxd)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[282]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[282]++;
							}
						}
						}
						else if((!adjacent(d,a))&&(adjacent(d,b))&&(!adjacent(d,c))&&(!adjacent(a,b))&&(adjacent(a,c))&&(!adjacent(b,c)))  //p44
						{
							rhs[91]+=(deg[x]-4);
							rhs[114]+=(deg[a]-2+deg[b]-2+deg[c]-2+deg[d]-2);
							rhs[322]+=(common3[TRIPLE(x,a,c)]+common3[TRIPLE(x,b,d)]);
							vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commxb)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[338]++;
							}
							if(find(commad.begin(),commad.end(),v)!=commad.end())
							{
								rhs[338]++;
							}
						}
						for(int v:commxd)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[338]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[338]++;
							}
						}
						for(int v:commad)
						{
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[373]++;
							}
						}
						rhs[373]--;
						}
						else if((adjacent(d,a))&&(adjacent(d,b))&&(adjacent(d,c))&&(!adjacent(a,b))&&(!adjacent(a,c))&&(!adjacent(b,c)))  //p55
						{
							rhs[215]+=(tri[x][d]-3);
						}
						else if((adjacent(d,a))&&(!adjacent(d,b))&&(adjacent(d,c))&&(!adjacent(a,b))&&(adjacent(a,c))&&(!adjacent(b,c)))  //p58
						{
                            vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
							for(int v:commxd)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[211]++;
							}
						}
						}
						else if((adjacent(d,a))&&(adjacent(d,b))&&(!adjacent(d,c))&&(!adjacent(a,b))&&(adjacent(a,c))&&(!adjacent(b,c)))  //p61
						{
							rhs[377]+=common3[TRIPLE(x,b,c)];
						}
						else if((adjacent(d,a))&&(adjacent(d,b))&&(adjacent(d,c))&&(!adjacent(a,b))&&(adjacent(a,c))&&(!adjacent(b,c)))  //p67
						{
							rhs[245]+=(tri[x][d]-3);
							rhs[306]+=(common3[TRIPLE(x,a,d)]-1+common3[TRIPLE(x,c,d)]-1);
							rhs[336]+=(tri[b][d]-1);
							rhs[355]+=(common3[TRIPLE(a,c,d)]-1);
							rhs[356]+=(common3[TRIPLE(x,a,c)]-1);
							rhs[375]+=common3[TRIPLE(x,b,d)];
							rhs[384]+=(common3[TRIPLE(a,b,d)]-1+common3[TRIPLE(b,c,d)]-1);
							rhs[385]+=(common3[TRIPLE(x,a,b)]-1+common3[TRIPLE(x,b,c)]-1);
							rhs[393]+=(common3[TRIPLE(a,b,c)]-2);
							vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commxb)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[397]++;
							}
							if(find(commad.begin(),commad.end(),v)!=commad.end())
							{
								rhs[391]++;
							}
						}
						rhs[397]--;
						for(int v:commxd)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[362]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[391]++;
							}
						}
						for(int v:commad)
						{
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[396]++;
							}
						}
						rhs[396]--;
						}
						else if((adjacent(d,a))&&(adjacent(d,b))&&(!adjacent(d,c))&&(!adjacent(a,b))&&(adjacent(a,c))&&(adjacent(b,c)))  //p69
						{
							rhs[369]+=(common3[TRIPLE(x,a,b)]-2+common3[TRIPLE(x,c,d)]-2);
						}
						else if((adjacent(d,a))&&(adjacent(d,b))&&(adjacent(d,c))&&(!adjacent(a,b))&&(adjacent(a,c))&&(adjacent(b,c)))  //p71
						{
							rhs[371]+=(common3[TRIPLE(x,c,d)]-2);
							vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
							vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
							for(int v:commxb)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[403]++;
							}
							if(find(commad.begin(),commad.end(),v)!=commad.end())
							{
								rhs[403]++;
							}
						}
						rhs[403]-=2;
						}
					}
				}
			}
			for(int na1=0;na1<deg[a];na1++)
			{
				int b=adj[a][na1];
				if(b==x) continue;
				for(int na4=0;na4<deg[a];na4++)
				{
					int c=adj[a][na4];
					if(c==b||c==x) continue;
					for(int na5=0;na5<deg[a];na5++)
					{
						int d=adj[a][na5];
						if(d==c||d==b||d==x) continue;
						if((!adjacent(d,x))&&(!adjacent(d,b))&&(!adjacent(d,c))&&(!adjacent(x,c))&&(!adjacent(x,b))&&(!adjacent(b,c))) //p22
						{
							rhs[0]+=(deg[a]-4);
							rhs[4]+=(deg[x]-1);
							rhs[6]+=(tri[a][b]+tri[a][c]+tri[a][d]);
							rhs[7]+=tri[x][a];
							rhs[22]+=(tri[x][b]-1+tri[x][c]-1+tri[x][d]-1);
							rhs[25]+=(common3[TRIPLE(a,b,c)]+common3[TRIPLE(a,b,d)]+common3[TRIPLE(a,c,d)]);
							rhs[26]+=(common3[TRIPLE(x,a,b)]+common3[TRIPLE(x,a,c)]+common3[TRIPLE(x,a,d)]);
							rhs[81]+=(common3[TRIPLE(b,c,d)]-1);
							rhs[82]+=(common3[TRIPLE(x,b,c)]-1+common3[TRIPLE(x,b,d)]-1+common3[TRIPLE(x,c,d)]-1);
							vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commxb)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[86]++;
							}
							if(find(commad.begin(),commad.end(),v)!=commad.end())
							{
								rhs[86]++;
							}
						}
						for(int v:commxd)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[86]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[212]++;
							}
						}
						rhs[212]--;
						for(int v:commad)
						{
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[85]++;
							}
						}
						}
						else if((!adjacent(d,x))&&(adjacent(d,b))&&(!adjacent(d,c))&&(!adjacent(x,c))&&(!adjacent(x,b))&&(!adjacent(b,c))) //p31
						{
							rhs[37]+=common3[TRIPLE(a,b,d)];
							rhs[89]+=tri[a][c];
						}
						else if((!adjacent(d,x))&&(adjacent(d,b))&&(adjacent(d,c))&&(!adjacent(x,c))&&(!adjacent(x,b))&&(!adjacent(b,c))) //p39
						{
							rhs[43]+=(deg[d]-3);
							rhs[102]+=(tri[a][b]-1+tri[a][c]-1);
							rhs[133]+=(common3[TRIPLE(a,b,d)]+common3[TRIPLE(a,c,d)]);
							rhs[160]+=(tri[b][c]-2);
							rhs[181]+=(common3[TRIPLE(a,b,c)]-1);
							rhs[236]+=common3[TRIPLE(x,a,d)];
                            vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
						vector<int> commbd;
						for(int nb2=0,nd2=0;nb2<deg[b]&&nd2<deg[d];)
						{
							if(adj[b][nb2]==adj[d][nd2])
							{
                                commbd.push_back(adj[b][nb2]);
								nb2++;
								nd2++;
							}
							else if(adj[b][nb2]<adj[d][nd2])
							nb2++;
							else
							nd2++;
						}
						vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
							for(int v:commac)
						{
							if(find(commbd.begin(),commbd.end(),v)!=commbd.end())
							{
								rhs[193]++;
							}
						}
						for(int v:commxb)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[294]++;
							}
						}
						}
					}
					for(int nx5=0;nx5<deg[x];nx5++)
					{
						int d=adj[x][nx5];
						if(d==a) continue;
						if((!adjacent(d,a))&&(!adjacent(d,c))&&(!adjacent(d,b))&&(!adjacent(x,b))&&(!adjacent(x,c))&&(!adjacent(b,c))) //p20
						{
							rhs[13]+=(deg[c]-1);
							rhs[19]+=tri[x][a];
							rhs[31]+=tri[x][c];
							rhs[35]+=common3[TRIPLE(x,a,c)];
							rhs[55]+=(tri[a][b]+tri[a][d]);
							rhs[73]+=(common3[TRIPLE(x,a,b)]+common3[TRIPLE(x,a,d)]);
							rhs[98]+=(common3[TRIPLE(a,b,c)]+common3[TRIPLE(a,c,d)]);
							rhs[119]+=(common3[TRIPLE(x,b,c)]+common3[TRIPLE(x,c,d)]);
							rhs[142]+=(tri[b][d]-1);
							rhs[146]+=(common3[TRIPLE(x,b,d)]-1);
							rhs[151]+=common3[TRIPLE(a,b,d)];
                            vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commxb)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[125]++;
							}
							if(find(commad.begin(),commad.end(),v)!=commad.end())
							{
								rhs[155]++;
							}
						}
						for(int v:commxd)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[125]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[232]++;
							}
						}
						}
						else if((adjacent(d,a))&&(!adjacent(d,c))&&(!adjacent(d,b))&&(!adjacent(x,b))&&(!adjacent(x,c))&&(!adjacent(b,c))) //p32
						{
							rhs[38]+=(common3[TRIPLE(x,a,d)]);
							vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
						vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
							for(int v:commxb)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[237]++;
							}
						}
						}
						else if((!adjacent(d,a))&&(!adjacent(d,c))&&(adjacent(d,b))&&(!adjacent(x,b))&&(!adjacent(x,c))&&(!adjacent(b,c))) //p37
						{
							rhs[129]+=common3[TRIPLE(a,c,d)];
							rhs[185]+=common3[TRIPLE(x,c,d)];
							rhs[216]+=tri[a][b];
						}
						else if((adjacent(d,a))&&(!adjacent(d,c))&&(adjacent(d,b))&&(!adjacent(x,b))&&(!adjacent(x,c))&&(!adjacent(b,c))) //p40
						{
							rhs[44]+=(deg[d]-3);
							rhs[103]+=(tri[a][b]-1);
							rhs[134]+=common3[TRIPLE(a,b,d)];
							rhs[135]+=common3[TRIPLE(x,a,d)];
							rhs[163]+=(tri[x][b]-2);
							rhs[235]+=common3[TRIPLE(a,c,d)];
							rhs[266]+=common3[TRIPLE(x,a,c)];
							rhs[275]+=(common3[TRIPLE(x,c,d)]-1);
							vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commxb)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[295]++;
							}
							if(find(commad.begin(),commad.end(),v)!=commad.end())
							{
								rhs[194]++;
							}
						}
						for(int v:commxd)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[280]++;
							}
						}
						for(int v:commad)
						{
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[279]++;
							}
						}
						}
						else if((!adjacent(d,a))&&(adjacent(d,c))&&(adjacent(d,b))&&(!adjacent(x,b))&&(!adjacent(x,c))&&(!adjacent(b,c))) //p49
						{
							rhs[240]+=(common3[TRIPLE(a,b,d)]+common3[TRIPLE(a,c,d)]);
							rhs[241]+=common3[TRIPLE(x,a,d)];
							rhs[283]+=(tri[b][c]-2);
							rhs[284]+=(tri[x][b]-2+tri[x][c]-2);
							rhs[289]+=(common3[TRIPLE(a,b,c)]+common3[TRIPLE(b,c,d)]);
							rhs[290]+=(common3[TRIPLE(x,a,b)]+common3[TRIPLE(x,a,c)]+common3[TRIPLE(x,b,d)]+common3[TRIPLE(x,c,d)]);
							rhs[363]+=(common3[TRIPLE(x,b,c)]-2);
							vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commxb)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[364]++;
							}
							if(find(commad.begin(),commad.end(),v)!=commad.end())
							{
								rhs[300]++;
							}
						}
						for(int v:commxd)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[300]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[364]++;
							}
						}
						for(int v:commad)
						{
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[299]++;
							}
						}
						}
						else if((adjacent(d,a))&&(adjacent(d,c))&&(adjacent(d,b))&&(!adjacent(x,b))&&(!adjacent(x,c))&&(!adjacent(b,c))) //p54
						{
							rhs[214]+=(tri[a][d]-3);
						}
					}
				}
				for(int nx4=0;nx4<deg[x];nx4++)
				{
					int c=adj[x][nx4];
					if(c==a) continue;
					for(int na7=0;na7<deg[a];na7++)
					{
						int d=adj[a][na7];
						if(d==b||d==x) continue;
						if(adjacent(b,d)&&(!adjacent(c,a))&&(!adjacent(c,d))&&(!adjacent(c,b))&&(!adjacent(x,b))&&(!adjacent(x,d))) //p28
						{
							rhs[187]+=(common3[TRIPLE(x,b,d)]-1);
							rhs[200]+=common3[TRIPLE(a,b,d)];
						}
					}
					for(int nc4=0;nc4<deg[c];nc4++)
					{
						int d=adj[c][nc4];
						if(d==x) continue;
						if((!adjacent(b,x))&&(!adjacent(b,c))&&(!adjacent(b,d))&&(!adjacent(a,c))&&(!adjacent(a,d))&&(!adjacent(x,d)))  //p17
						{
							rhs[41]+=(tri[a][c]-1);
							rhs[58]+=(deg[b]-1+deg[d]-1);
							rhs[66]+=(tri[x][a]+tri[x][c]);
							rhs[76]+=common3[TRIPLE(x,a,c)];
							rhs[107]+=(tri[a][b]+tri[c][d]);
							rhs[158]+=(common3[TRIPLE(x,b,c)]+common3[TRIPLE(x,a,d)]);
							rhs[167]+=(common3[TRIPLE(x,a,b)]+common3[TRIPLE(x,c,d)]);
							rhs[258]+=common3[TRIPLE(x,b,d)];
							vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commab)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[261]++;
							}
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[172]++;
							}
						}
						for(int v:commbc)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[261]++;
							}
						}
						for(int v:commad)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[172]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[252]++;
							}
						}
						}
						else if((!adjacent(b,x))&&(!adjacent(b,c))&&(!adjacent(b,d))&&(adjacent(a,c))&&(!adjacent(a,d))&&(!adjacent(x,d)))  //p25
						{
							rhs[18]+=(deg[a]-3+deg[c]-3);
							rhs[65]+=(deg[b]-1+deg[d]-1);
							rhs[79]+=common3[TRIPLE(x,a,c)];
							vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commab)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[178]++;
							}
						}
						for(int v:commad)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[178]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[255]++;
							}
						}
						}
						else if((!adjacent(b,x))&&(!adjacent(b,c))&&(!adjacent(b,d))&&(!adjacent(a,c))&&(!adjacent(a,d))&&(adjacent(x,d)))  //p30
						{
							rhs[199]+=common3[TRIPLE(x,c,d)];
							rhs[219]+=(tri[x][b]-1);
							rhs[308]+=tri[a][b];
						}
						else if((!adjacent(b,x))&&(!adjacent(b,c))&&(!adjacent(b,d))&&(!adjacent(a,c))&&(adjacent(a,d))&&(adjacent(x,d)))  //p48
						{
							rhs[130]+=(tri[a][c]-2);
							rhs[174]+=(tri[x][c]-1);
							rhs[179]+=common3[TRIPLE(x,a,d)];
							rhs[189]+=(common3[TRIPLE(a,c,d)]-1);
							rhs[203]+=common3[TRIPLE(x,c,d)];
							rhs[264]+=(tri[x][b]-1);
							rhs[288]+=(common3[TRIPLE(x,b,d)]-1);
							rhs[312]+=tri[a][b];
							rhs[316]+=common3[TRIPLE(a,b,d)];
							rhs[317]+=common3[TRIPLE(x,a,b)];
							rhs[324]+=tri[b][c];
							rhs[327]+=common3[TRIPLE(a,b,c)];
							rhs[340]+=common3[TRIPLE(b,c,d)];
							rhs[341]+=common3[TRIPLE(x,b,c)];
							vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commab)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[319]++;
							}
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[345]++;
							}
						}
						for(int v:commbc)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[349]++;
							}
						}
						for(int v:commad)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[207]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[344]++;
							}
						}
						}
						else if((!adjacent(b,x))&&(!adjacent(b,c))&&(adjacent(b,d))&&(!adjacent(a,c))&&(!adjacent(a,d))&&(adjacent(x,d)))  //p53
						{
							rhs[100]+=(deg[d]-3);
							rhs[234]+=(tri[x][b]-2);
							rhs[254]+=tri[x][a];
							rhs[270]+=(tri[a][c]-1);
							rhs[271]+=(tri[b][c]-1);
							rhs[278]+=common3[TRIPLE(x,b,d)];
							rhs[293]+=(common3[TRIPLE(x,b,c)]-1);
							rhs[329]+=common3[TRIPLE(a,b,d)];
							rhs[333]+=common3[TRIPLE(x,c,d)];
							vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commxa;
						for(int nx2=0,na2=0;nx2<deg[x]&&na2<deg[a];)
						{
							if(adj[x][nx2]==adj[a][na2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								na2++;
							}
							else if(adj[x][nx2]<adj[a][na2])
							nx2++;
							else
							na2++;
						}
							for(int v:commbc)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[351]++;
							}
							if(find(commxa.begin(),commxa.end(),v)!=commxa.end())
							{
								rhs[381]++;
							}
						}
						}
						else if((!adjacent(b,x))&&(!adjacent(b,c))&&(!adjacent(b,d))&&(adjacent(a,c))&&(adjacent(a,d))&&(adjacent(x,d)))  //p57
						{
							vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
						for(int v:commxd)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[210]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[358]++;
							}
						}
						rhs[358]--;
						}
						else if((!adjacent(b,x))&&(!adjacent(b,c))&&(adjacent(b,d))&&(adjacent(a,c))&&(adjacent(a,d))&&(adjacent(x,d)))  //p66
						{
							rhs[244]+=(tri[a][d]-3);
							rhs[304]+=(common3[TRIPLE(a,c,d)]-1);
							rhs[305]+=(common3[TRIPLE(x,a,d)]-1);
							rhs[354]+=(common3[TRIPLE(x,a,c)]-1+common3[TRIPLE(x,c,d)]-1);
							rhs[382]+=(common3[TRIPLE(a,b,c)]-1+common3[TRIPLE(b,c,d)]-1);
							vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
						vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						for(int v:commxd)
						{
							if(find(commac.begin(),commac.end(),v)!=commac.end())
							{
								rhs[361]++;
							}
							if(find(commab.begin(),commab.end(),v)!=commab.end())
							{
								rhs[390]++;
							}
						}
						}
					}
					for(int nx5=0;nx5<deg[x];nx5++)
					{
						int d=adj[x][nx5];
						if(d==a||d==c) continue;
						if((!adjacent(b,x))&&(!adjacent(b,c))&&(!adjacent(b,d))&&(!adjacent(a,c))&&(!adjacent(a,d))&&(!adjacent(c,d)))  //p21
						{
							rhs[10]+=(deg[a]-2);
							rhs[15]+=(deg[b]-1);
							rhs[20]+=tri[x][a];
							rhs[32]+=tri[a][b];
							rhs[36]+=common3[TRIPLE(x,a,b)];
							rhs[49]+=(deg[c]-1+deg[d]-1);
							rhs[56]+=(tri[x][c]+tri[x][d]);
							rhs[62]+=(tri[a][c]-1+tri[a][d]-1);
							rhs[74]+=(common3[TRIPLE(x,a,c)]+common3[TRIPLE(x,a,d)]);
							rhs[95]+=(tri[b][c]+tri[b][d]);
							rhs[101]+=(common3[TRIPLE(x,b,c)]+common3[TRIPLE(x,b,d)]);
							rhs[121]+=(common3[TRIPLE(a,b,c)]+common3[TRIPLE(a,b,d)]);
							rhs[141]+=(tri[c][d]-1);
							rhs[145]+=(common3[TRIPLE(a,c,d)]-1);
							rhs[150]+=common3[TRIPLE(x,c,d)];
							rhs[226]+=common3[TRIPLE(b,c,d)];
							vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commab)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[127]++;
							}
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[127]++;
							}
						}
						for(int v:commbc)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[229]++;
							}
						}
						for(int v:commad)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[154]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[233]++;
							}
						}
						}
						else if((!adjacent(b,x))&&(!adjacent(b,c))&&(!adjacent(b,d))&&(!adjacent(a,c))&&(adjacent(a,d))&&(!adjacent(c,d)))  //p26
						{
							rhs[51]+=(deg[d]-2);
							rhs[80]+=common3[TRIPLE(x,a,d)];
							vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						for(int v:commab)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[256]++;
							}
						}
						}
						else if((!adjacent(b,x))&&(!adjacent(b,c))&&(!adjacent(b,d))&&(adjacent(a,c))&&(adjacent(a,d))&&(!adjacent(c,d)))  //p41
						{
							rhs[27]+=(deg[a]-4);
							rhs[45]+=(deg[x]-3);
							rhs[72]+=(deg[c]-2+deg[d]-2);
							rhs[87]+=(tri[x][a]-2);
							rhs[104]+=(tri[a][c]-1+tri[a][d]-1);
							rhs[126]+=(tri[x][c]-1+tri[x][d]-1);
							rhs[136]+=(common3[TRIPLE(x,a,c)]+common3[TRIPLE(x,a,d)]);
							rhs[149]+=(deg[b]-1);
							rhs[162]+=(tri[c][d]-2);
							rhs[182]+=(common3[TRIPLE(a,c,d)]-1);
							rhs[191]+=(common3[TRIPLE(x,c,d)]-1);
							rhs[222]+=tri[a][b];
							rhs[238]+=common3[TRIPLE(x,a,b)];
							rhs[267]+=(common3[TRIPLE(a,b,c)]+common3[TRIPLE(a,b,d)]);
							rhs[277]+=(common3[TRIPLE(x,b,c)]-1+common3[TRIPLE(x,b,d)]-1);
							rhs[291]+=(common3[TRIPLE(b,c,d)]-1);
								vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commab)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[281]++;
							}
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[281]++;
							}
						}
						for(int v:commbc)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[301]++;
							}
						}
						rhs[301]--;
						for(int v:commad)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[195]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[296]++;
							}
						}
						}
						else if((!adjacent(b,x))&&(!adjacent(b,c))&&(adjacent(b,d))&&(!adjacent(a,c))&&(adjacent(a,d))&&(!adjacent(c,d)))  //p47
						{
							rhs[132]+=(tri[x][b]-2);
							rhs[180]+=common3[TRIPLE(x,a,d)];
							rhs[192]+=(common3[TRIPLE(x,a,b)]-1+common3[TRIPLE(x,b,d)]-1);
							rhs[204]+=common3[TRIPLE(a,b,d)];
							rhs[286]+=(common3[TRIPLE(a,c,d)]-1);
							rhs[311]+=tri[x][c];
							rhs[325]+=tri[b][c];
							rhs[330]+=common3[TRIPLE(x,b,c)];
							rhs[342]+=(common3[TRIPLE(a,b,c)]+common3[TRIPLE(b,c,d)]);
							vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commab)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[208]++;
							}
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[346]++;
							}
						}
						for(int v:commbc)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[346]++;
							}
						}
						for(int v:commad)
						{
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[350]++;
							}
						}
						}
					}
				}
				for(int nb1=0;nb1<deg[b];nb1++)
				{
					int c=adj[b][nb1];
					if(c==a) continue;
					for(int nc1=0;nc1<deg[c];nc1++)
					{
						int d=adj[c][nc1];						
						if(d==b) continue;
						if((!adjacent(b,x))&&(!adjacent(c,x))&&(!adjacent(c,a))&&adjacent(d,b)&&(!adjacent(d,a))&&(!adjacent(x,d))) //p27
						{
							rhs[111]+=tri[a][b];
							rhs[184]+=(common3[TRIPLE(a,c,d)]-1);
							rhs[197]+=common3[TRIPLE(b,c,d)];
						}
						else if((!adjacent(b,x))&&(!adjacent(c,x))&&(!adjacent(c,a))&&adjacent(d,b)&&(!adjacent(d,a))&&(adjacent(x,d))) //p51
						{
							rhs[269]+=(tri[a][c]-1);
							rhs[272]+=(tri[x][c]-1);
							rhs[274]+=(common3[TRIPLE(a,b,d)]);
							rhs[276]+=(common3[TRIPLE(x,b,d)]);
							rhs[292]+=(common3[TRIPLE(x,b,c)]-1);
							rhs[314]+=tri[x][a];
							rhs[328]+=common3[TRIPLE(x,a,b)];
							rhs[331]+=common3[TRIPLE(b,c,d)];
							rhs[339]+=common3[TRIPLE(a,b,c)];
							rhs[378]+=common3[TRIPLE(x,a,c)];
							vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commcd;
						for(int nc2=0,nd2=0;nc2<deg[c]&&nd2<deg[d];)
						{
							if(adj[c][nc2]==adj[d][nd2])
							{
                                commcd.push_back(adj[c][nc2]);
								nc2++;
								nd2++;
							}
							else if(adj[c][nc2]<adj[d][nd2])
							nc2++;
							else
							nd2++;
						}
						vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
                        for(int v:commab)
						{
							if(find(commcd.begin(),commcd.end(),v)!=commcd.end())
							{
								rhs[347]++;
							}
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[380]++;
							}
						}
						for(int v:commad)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[379]++;
							}
						}
						}
						else if((!adjacent(b,x))&&(!adjacent(c,x))&&(!adjacent(c,a))&&(!adjacent(d,b))&&(adjacent(d,a))&&(adjacent(x,d))) //p52
						{
							rhs[273]+=(common3[TRIPLE(a,b,d)]+common3[TRIPLE(a,c,d)]);
							rhs[313]+=tri[b][c];
							rhs[332]+=common3[TRIPLE(x,a,d)];
						}
						else if((!adjacent(b,x))&&(!adjacent(c,x))&&(adjacent(c,a))&&(adjacent(d,b))&&(adjacent(d,a))&&(!adjacent(x,d))) //p56
						{
                            vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commcd;
						for(int nc2=0,nd2=0;nc2<deg[c]&&nd2<deg[d];)
						{
							if(adj[c][nc2]==adj[d][nd2])
							{
                                commcd.push_back(adj[c][nc2]);
								nc2++;
								nd2++;
							}
							else if(adj[c][nc2]<adj[d][nd2])
							nc2++;
							else
							nd2++;
						}
						vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
						for(int v:commcd)
						{
							if(find(commxb.begin(),commxb.end(),v)!=commxb.end())
							{
								rhs[357]++;
							}
							if(find(commab.begin(),commab.end(),v)!=commab.end())
							{
								rhs[209]++;
							}
						}
						rhs[357]--;
						}
						else if((!adjacent(b,x))&&(!adjacent(c,x))&&(adjacent(c,a))&&(adjacent(d,b))&&(adjacent(d,a))&&(adjacent(x,d)))  //p59
						{
							vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						    for(int v:commad)
						{
							if(find(commxb.begin(),commxb.end(),v)!=commxb.end())
							{
								rhs[353]++;
							}
						}
							rhs[376]+=common3[TRIPLE(x,c,d)];
						}
						else if((!adjacent(b,x))&&(!adjacent(c,x))&&(adjacent(c,a))&&(adjacent(d,b))&&adjacent(d,a)&&(adjacent(x,d))) //p65
						{
							rhs[243]+=(tri[a][d]-3);
							rhs[303]+=(common3[TRIPLE(a,b,d)]-1+common3[TRIPLE(a,c,d)]-1);
							rhs[352]+=(common3[TRIPLE(a,b,c)]-1+common3[TRIPLE(b,c,d)]-1);
							rhs[374]+=common3[TRIPLE(x,a,d)];
							rhs[392]+=(common3[TRIPLE(x,b,c)]-2);
							vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
						for(int v:commad)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[389]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[360]++;
							}
							if(find(commxb.begin(),commxb.end(),v)!=commxb.end())
							{
								rhs[389]++;
							}
						}
						}
						else if((!adjacent(b,x))&&(adjacent(c,x))&&(!adjacent(c,a))&&(adjacent(d,b))&&adjacent(d,a)&&(adjacent(x,d))) //p68
						{
							rhs[297]+=(tri[x][d]-2);
							rhs[367]+=(common3[TRIPLE(a,c,d)]-2);
							rhs[368]+=(common3[TRIPLE(x,b,d)]-2);
							rhs[383]+=(common3[TRIPLE(a,b,d)]+common3[TRIPLE(b,c,d)]);
							rhs[386]+=(common3[TRIPLE(a,b,c)]-1);
							rhs[387]+=(common3[TRIPLE(x,a,c)]-1);
							rhs[388]+=(common3[TRIPLE(x,a,b)]-1+common3[TRIPLE(x,b,c)]-1);
							vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commcd;
						for(int nc2=0,nd2=0;nc2<deg[c]&&nd2<deg[d];)
						{
							if(adj[c][nc2]==adj[d][nd2])
							{
                                commcd.push_back(adj[c][nc2]);
								nc2++;
								nd2++;
							}
							else if(adj[c][nc2]<adj[d][nd2])
							nc2++;
							else
							nd2++;
						}
						vector<int> commxa;
						for(int nx2=0,na2=0;nx2<deg[x]&&na2<deg[a];)
						{
							if(adj[x][nx2]==adj[a][na2])
							{
                                commxa.push_back(adj[x][nx2]);
								nx2++;
								na2++;
							}
							else if(adj[x][nx2]<adj[a][na2])
							nx2++;
							else
							na2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						for(int v:commcd)
						{
							if(find(commab.begin(),commab.end(),v)!=commab.end())
							{
								rhs[394]++;
							}
							if(find(commxa.begin(),commxa.end(),v)!=commxa.end())
							{
								rhs[395]++;
							}
						}
						for(int v:commbc)
						{
							if(find(commxa.begin(),commxa.end(),v)!=commxa.end())
							{
								rhs[401]++;
							}
						}
						rhs[401]--;
						}
						else if((!adjacent(b,x))&&(adjacent(c,x))&&(adjacent(c,a))&&adjacent(d,b)&&adjacent(d,a)&&(adjacent(x,d))) //p70
						{
							rhs[370]+=(common3[TRIPLE(a,c,d)]-2);
							vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commcd;
						for(int nc2=0,nd2=0;nc2<deg[c]&&nd2<deg[d];)
						{
							if(adj[c][nc2]==adj[d][nd2])
							{
                                commcd.push_back(adj[c][nc2]);
								nc2++;
								nd2++;
							}
							else if(adj[c][nc2]<adj[d][nd2])
							nc2++;
							else
							nd2++;
						}
						vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						for(int v:commab)
						{
							if(find(commcd.begin(),commcd.end(),v)!=commcd.end())
							{
								rhs[398]++;
							}
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[402]++;
							}
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[402]++;
							}
						}
						for(int v:commcd)
						{
							if(find(commxb.begin(),commxb.end(),v)!=commxb.end())
							{
								rhs[402]++;
							}
						}
						rhs[402]-=3;
						}
						else if((adjacent(b,x))&&(adjacent(c,x))&&(adjacent(c,a))&&adjacent(d,b)&&adjacent(d,a)&&(adjacent(x,d))) //p72
						{
							rhs[399]+=(common3[TRIPLE(a,b,c)]-2+common3[TRIPLE(a,b,d)]-2+common3[TRIPLE(a,c,d)]-2+common3[TRIPLE(b,c,d)]-2);
							rhs[400]+=(common3[TRIPLE(x,a,b)]-2+common3[TRIPLE(x,a,c)]-2+common3[TRIPLE(x,b,c)]-2+common3[TRIPLE(x,a,d)]-2+common3[TRIPLE(x,b,d)]-2+common3[TRIPLE(x,c,d)]-2);
						    vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						vector<int> commbd;
						for(int nb2=0,nd2=0;nb2<deg[b]&&nd2<deg[d];)
						{
							if(adj[b][nb2]==adj[d][nd2])
							{
                                commbd.push_back(adj[b][nb2]);
								nb2++;
								nd2++;
							}
							else if(adj[b][nb2]<adj[d][nd2])
							nb2++;
							else
							nd2++;
						}
						vector<int> commxa;
						for(int nx2=0,na2=0;nx2<deg[x]&&na2<deg[a];)
						{
							if(adj[x][nx2]==adj[a][na2])
							{
                                commxa.push_back(adj[x][nx2]);
								nx2++;
								na2++;
							}
							else if(adj[x][nx2]<adj[a][na2])
							nx2++;
							else
							na2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						for(int v:commad)
						{
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[404]++;
							}
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[405]++;
							}
						}
						for(int v:commxa)
						{
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[405]++;
							}
							if(find(commbd.begin(),commbd.end(),v)!=commbd.end())
							{
								rhs[405]++;
							}
						}
						for(int v:commxc)
						{
							if(find(commbd.begin(),commbd.end(),v)!=commbd.end())
							{
								rhs[405]++;
							}
						}
						rhs[404]--;
						rhs[405]-=4;
						}
						else if((!adjacent(b,x))&&(!adjacent(c,x))&&(!adjacent(c,a))&&adjacent(d,b)&&adjacent(d,a)&&(!adjacent(x,d)))   //p45
						{
                            rhs[128]+=(tri[a][c]-2);
							rhs[176]+=(common3[TRIPLE(a,b,d)]);
							rhs[188]+=(common3[TRIPLE(a,b,c)]-1+common3[TRIPLE(a,c,d)]-1);
							rhs[201]+=common3[TRIPLE(b,c,d)];
							rhs[287]+=(common3[TRIPLE(x,b,d)]-1);
							rhs[323]+=tri[x][c];
							vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commcd;
						for(int nc2=0,nd2=0;nc2<deg[c]&&nd2<deg[d];)
						{
							if(adj[c][nc2]==adj[d][nd2])
							{
                                commcd.push_back(adj[c][nc2]);
								nc2++;
								nd2++;
							}
							else if(adj[c][nc2]<adj[d][nd2])
							nc2++;
							else
							nd2++;
						}
						vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commab)
						{
							if(find(commcd.begin(),commcd.end(),v)!=commcd.end())
							{
								rhs[205]++;
							}
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[343]++;
							}
						}
						for(int v:commad)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[343]++;
							}
						}
						}
						else if((!adjacent(b,x))&&(!adjacent(c,x))&&(!adjacent(c,a))&&(!adjacent(d,b))&&(!adjacent(d,a))&&(!adjacent(x,d))){
						//p15 holo
						//do rhs for p15
						rhs[12]+=(deg[c]-2);
						rhs[40]+=(tri[a][c]-1);
						rhs[57]+=(deg[d]-1);
						rhs[59]+=(deg[x]-1);
						rhs[63]+=tri[a][b];
						rhs[64]+=tri[b][c];
						rhs[75]+=common3[TRIPLE(a,b,c)];
						rhs[106]+=tri[c][d];
						rhs[108]+=tri[x][a];
						rhs[140]+=(tri[x][b]-1);
						rhs[156]+=common3[TRIPLE(a,b,d)];
						rhs[157]+=common3[TRIPLE(x,b,c)];
						rhs[165]+=common3[TRIPLE(b,c,d)];
						rhs[166]+=common3[TRIPLE(x,a,b)];
						rhs[246]+=tri[x][d];
						rhs[247]+=common3[TRIPLE(x,a,d)];
						rhs[248]+=common3[TRIPLE(x,c,d)];
						rhs[257]+=common3[TRIPLE(x,b,d)];
						vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commcd;
						for(int nc2=0,nd2=0;nc2<deg[c]&&nd2<deg[d];)
						{
							if(adj[c][nc2]==adj[d][nd2])
							{
                                commcd.push_back(adj[c][nc2]);
								nc2++;
								nd2++;
							}
							else if(adj[c][nc2]<adj[d][nd2])
							nc2++;
							else
							nd2++;
						}
						vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commxb;
						for(int nx2=0,nb2=0;nx2<deg[x]&&nb2<deg[b];)
						{
							if(adj[x][nx2]==adj[b][nb2])
							{
                                commxb.push_back(adj[x][nx2]);
								nx2++;
								nb2++;
							}
							else if(adj[x][nx2]<adj[b][nb2])
							nx2++;
							else
							nb2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commab)
						{
							if(find(commcd.begin(),commcd.end(),v)!=commcd.end())
							{
								rhs[170]++;
							}
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[171]++;
							}
						}
						for(int v:commad)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[251]++;
							}
							if(find(commxb.begin(),commxb.end(),v)!=commxb.end())
							{
								rhs[259]++;
							}
						}
						for(int v:commcd)
						{
							if(find(commxb.begin(),commxb.end(),v)!=commxb.end())
							{
								rhs[260]++;
							}
						}
						}
					}
					for(int nx3=0;nx3<deg[x];nx3++)
					{
						int d=adj[x][nx3];
						if(d==a) continue;
						if((!adjacent(d,a))&&(!adjacent(d,b))&&(!adjacent(d,c))&&(!adjacent(a,c))&&(!adjacent(b,x))&&(!adjacent(c,x))) //p16
						{
							rhs[42]+=(tri[x][b]-1);
							rhs[67]+=tri[a][b];
							rhs[68]+=tri[x][a];
							rhs[77]+=common3[TRIPLE(x,a,b)];
							rhs[109]+=tri[x][d];
							rhs[110]+=tri[b][c];
							rhs[120]+=common3[TRIPLE(x,b,d)];
							rhs[159]+=common3[TRIPLE(x,a,c)];
							rhs[168]+=common3[TRIPLE(x,a,d)];
							rhs[169]+=common3[TRIPLE(a,b,c)];
							rhs[249]+=common3[TRIPLE(b,c,d)];
							rhs[250]+=common3[TRIPLE(x,c,d)];
							vector<int> commbd;
						for(int nb2=0,nd2=0;nb2<deg[b]&&nd2<deg[d];)
						{
							if(adj[b][nb2]==adj[d][nd2])
							{
                                commbd.push_back(adj[b][nb2]);
								nb2++;
								nd2++;
							}
							else if(adj[b][nb2]<adj[d][nd2])
							nb2++;
							else
							nd2++;
						}
						vector<int> commxa;
						for(int nx2=0,na2=0;nx2<deg[x]&&na2<deg[a];)
						{
							if(adj[x][nx2]==adj[a][na2])
							{
                                commxa.push_back(adj[x][nx2]);
								nx2++;
								na2++;
							}
							else if(adj[x][nx2]<adj[a][na2])
							nx2++;
							else
							na2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						for(int v:commxa)
						{
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[175]++;
							}
							if(find(commbd.begin(),commbd.end(),v)!=commbd.end())
							{
								rhs[173]++;
							}
						}
						for(int v:commbc)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[253]++;
							}
						}
						for(int v:commac)
						{
							if(find(commbd.begin(),commbd.end(),v)!=commbd.end())
							{
								rhs[262]++;
							}
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[263]++;
							}
						}
						}
						else if((adjacent(d,a))&&(!adjacent(d,b))&&(!adjacent(d,c))&&(!adjacent(a,c))&&(!adjacent(b,x))&&(!adjacent(c,x))) //p29
						{
							rhs[198]+=common3[TRIPLE(x,a,d)];
							rhs[217]+=(tri[a][c]-1);
							rhs[307]+=tri[b][c];
						}
						else if((!adjacent(d,a))&&(adjacent(d,b))&&(!adjacent(d,c))&&(!adjacent(a,c))&&(!adjacent(b,x))&&(!adjacent(c,x))) //p36
						{
							rhs[161]+=common3[TRIPLE(a,b,d)];
							rhs[186]+=common3[TRIPLE(x,a,d)];
							rhs[218]+=tri[b][c];
						}
						else if((adjacent(d,a))&&(!adjacent(d,b))&&(!adjacent(d,c))&&(adjacent(a,c))&&(!adjacent(b,x))&&(!adjacent(c,x))) //p43
						{
							rhs[90]+=(deg[a]-4);
							rhs[112]+=(deg[d]-2);
							rhs[113]+=(deg[b]-2+deg[c]-2);
							rhs[115]+=(deg[x]-2);
							rhs[220]+=(tri[a][d]-1);
							rhs[221]+=(tri[a][c]-1+tri[a][b]-1);
							rhs[265]+=(common3[TRIPLE(a,b,d)]+common3[TRIPLE(a,c,d)]);
							rhs[310]+=(tri[b][c]-1);
							rhs[320]+=(common3[TRIPLE(a,b,c)]);
							rhs[321]+=common3[TRIPLE(x,a,d)];
							rhs[326]+=(common3[TRIPLE(b,c,d)]-1);
							vector<int> commbd;
						for(int nb2=0,nd2=0;nb2<deg[b]&&nd2<deg[d];)
						{
							if(adj[b][nb2]==adj[d][nd2])
							{
                                commbd.push_back(adj[b][nb2]);
								nb2++;
								nd2++;
							}
							else if(adj[b][nb2]<adj[d][nd2])
							nb2++;
							else
							nd2++;
						}
						vector<int> commxa;
						for(int nx2=0,na2=0;nx2<deg[x]&&na2<deg[a];)
						{
							if(adj[x][nx2]==adj[a][na2])
							{
                                commxa.push_back(adj[x][nx2]);
								nx2++;
								na2++;
							}
							else if(adj[x][nx2]<adj[a][na2])
							nx2++;
							else
							na2++;
						}
						vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commcd;
						for(int nc2=0,nd2=0;nc2<deg[c]&&nd2<deg[d];)
						{
							if(adj[c][nc2]==adj[d][nd2])
							{
                                commcd.push_back(adj[c][nc2]);
								nc2++;
								nd2++;
							}
							else if(adj[c][nc2]<adj[d][nd2])
							nc2++;
							else
							nd2++;
						}
						vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						for(int v:commxa)
						{
							if(find(commbd.begin(),commbd.end(),v)!=commbd.end())
							{
								rhs[335]++;
							}
							if(find(commcd.begin(),commcd.end(),v)!=commcd.end())
							{
								rhs[335]++;
							}
						}
						for(int v:commab)
						{
							if(find(commcd.begin(),commcd.end(),v)!=commcd.end())
							{
								rhs[334]++;
							}
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[337]++;
							}
						}
						for(int v:commbd)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[372]++;
							}
						}
						rhs[372]--;
						}
						else if((adjacent(d,a))&&(adjacent(d,b))&&(!adjacent(d,c))&&(!adjacent(a,c))&&(!adjacent(b,x))&&(!adjacent(c,x))) //p46
						{
							rhs[131]+=(tri[x][b]-2);
							rhs[177]+=(common3[TRIPLE(a,b,d)]);
							rhs[190]+=(common3[TRIPLE(x,a,b)]-1+common3[TRIPLE(x,b,d)]-1);
							rhs[202]+=common3[TRIPLE(x,a,d)];
							rhs[285]+=(common3[TRIPLE(a,c,d)]-1);
                            rhs[309]+=tri[b][c];
							rhs[315]+=(common3[TRIPLE(a,b,c)]+common3[TRIPLE(b,c,d)]);
							vector<int> commbd;
						for(int nb2=0,nd2=0;nb2<deg[b]&&nd2<deg[d];)
						{
							if(adj[b][nb2]==adj[d][nd2])
							{
                                commbd.push_back(adj[b][nb2]);
								nb2++;
								nd2++;
							}
							else if(adj[b][nb2]<adj[d][nd2])
							nb2++;
							else
							nd2++;
						}
						vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commac;
						for(int na2=0,nc2=0;na2<deg[a]&&nc2<deg[c];)
						{
							if(adj[a][na2]==adj[c][nc2])
							{
                                commac.push_back(adj[a][na2]);
								na2++;
								nc2++;
							}
							else if(adj[a][na2]<adj[c][nc2])
							na2++;
							else
							nc2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						for(int v:commac)
						{
							if(find(commbd.begin(),commbd.end(),v)!=commbd.end())
							{
								rhs[318]++;
							}
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[348]++;
							}
						}
						for(int v:commab)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[206]++;
							}
						}
						}
						else if((!adjacent(d,a))&&(adjacent(d,b))&&(!adjacent(d,c))&&(!adjacent(a,c))&&(!adjacent(b,x))&&(adjacent(c,x))) //p50
						{
							rhs[83]+=(deg[b]-3);
							rhs[242]+=(common3[TRIPLE(x,a,b)]+common3[TRIPLE(x,b,c)]+common3[TRIPLE(x,b,d)]);
							vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commab)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[302]++;
							}
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[302]++;
							}
						}
						for(int v:commbc)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[302]++;
							}
						}
						for(int v:commad)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[366]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[365]++;
							}
						}
						}
						else if((adjacent(d,a))&&(adjacent(d,b))&&(!adjacent(d,c))&&(!adjacent(a,c))&&(!adjacent(b,x))&&(adjacent(c,x))) //p63
						{
							vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						for(int v:commab)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[359]++;
							}
						}
						}
					}
					for(int nb3=0;nb3<deg[b];nb3++)
					{
						int d=adj[b][nb3];
						if(d==a||d==c) continue;
						if((!adjacent(x,b))&&(!adjacent(x,c))&&(!adjacent(x,d))&&(!adjacent(a,d))&&(!adjacent(a,c))&&(!adjacent(c,d))) //p18
						{
							rhs[3]+=(deg[b]-3);
							rhs[14]+=(deg[x]-1);
							rhs[17]+=tri[a][b];
							rhs[23]+=(tri[x][b]-1);
							rhs[30]+=tri[x][a];
							rhs[34]+=common3[TRIPLE(x,a,b)];
							rhs[47]+=(deg[c]-1+deg[d]-1);
							rhs[53]+=(tri[b][c]+tri[b][d]);
							rhs[70]+=(common3[TRIPLE(a,b,c)]+common3[TRIPLE(a,b,d)]);
							rhs[94]+=(tri[x][c]+tri[x][d]);
							rhs[99]+=(common3[TRIPLE(x,b,c)]+common3[TRIPLE(x,b,d)]);
							rhs[118]+=(common3[TRIPLE(x,a,c)]+common3[TRIPLE(x,a,d)]);
							rhs[138]+=(tri[c][d]-1);
							rhs[143]+=(common3[TRIPLE(a,c,d)]-1);
							rhs[147]+=common3[TRIPLE(b,c,d)];
							rhs[225]+=common3[TRIPLE(x,c,d)];
                            vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commab)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[124]++;
							}
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[124]++;
							}
						}
						for(int v:commbc)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[228]++;
							}
						}
						for(int v:commad)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[231]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[152]++;
							}
						}
						}
						else if((!adjacent(x,b))&&(!adjacent(x,c))&&(!adjacent(x,d))&&(adjacent(a,d))&&(!adjacent(a,c))&&(!adjacent(c,d))) //p24
						{
							rhs[50]+=(deg[d]-2);
							rhs[78]+=common3[TRIPLE(a,b,d)];
						}
					}
					for(int na3=0;na3<deg[a];na3++)
					{
						int d=adj[a][na3];
						if(d==x||d==b) continue;
						if((!adjacent(x,d))&&(!adjacent(x,b))&&(!adjacent(x,c))&&(!adjacent(a,c))&&(!adjacent(b,d))&&(!adjacent(c,d))) //p19
						{
							rhs[2]+=(deg[a]-3);
							rhs[9]+=(deg[b]-2);
							rhs[11]+=(deg[c]-1);
							rhs[16]+=tri[a][b];
							rhs[21]+=(tri[a][c]-1);
							rhs[29]+=tri[b][c];
							rhs[33]+=common3[TRIPLE(a,b,c)];
							rhs[46]+=(deg[d]-1);
							rhs[48]+=(deg[x]-1);
							rhs[52]+=tri[a][d];
							rhs[54]+=tri[x][a];
							rhs[60]+=(tri[b][d]-1);
							rhs[61]+=(tri[x][b]-1);
							rhs[69]+=common3[TRIPLE(a,b,d)];
							rhs[71]+=common3[TRIPLE(x,a,b)];
							rhs[92]+=tri[c][d];
							rhs[93]+=tri[x][c];
							rhs[96]+=common3[TRIPLE(a,c,d)];
							rhs[97]+=common3[TRIPLE(x,a,c)];
							rhs[116]+=common3[TRIPLE(b,c,d)];
							rhs[117]+=common3[TRIPLE(x,b,c)];
							rhs[139]+=(tri[x][d]-1);
							rhs[144]+=(common3[TRIPLE(x,b,d)]-1);
							rhs[148]+=common3[TRIPLE(x,a,d)];
							rhs[224]+=common3[TRIPLE(x,c,d)];
							vector<int> commab;
						for(int na2=0,nb2=0;na2<deg[a]&&nb2<deg[b];)
						{
							if(adj[a][na2]==adj[b][nb2])
							{
                                commab.push_back(adj[a][na2]);
								na2++;
								nb2++;
							}
							else if(adj[a][na2]<adj[b][nb2])
							na2++;
							else
							nb2++;
						}
						vector<int> commxd;
						for(int nx2=0,nd2=0;nx2<deg[x]&&nd2<deg[d];)
						{
							if(adj[x][nx2]==adj[d][nd2])
							{
                                commxd.push_back(adj[x][nx2]);
								nx2++;
								nd2++;
							}
							else if(adj[x][nx2]<adj[d][nd2])
							nx2++;
							else
							nd2++;
						}
						vector<int> commxc;
						for(int nx2=0,nc2=0;nx2<deg[x]&&nc2<deg[c];)
						{
							if(adj[x][nx2]==adj[c][nc2])
							{
                                commxc.push_back(adj[x][nx2]);
								nx2++;
								nc2++;
							}
							else if(adj[x][nx2]<adj[c][nc2])
							nx2++;
							else
							nc2++;
						}
						vector<int> commbc;
						for(int nb2=0,nc2=0;nb2<deg[b]&&nc2<deg[c];)
						{
							if(adj[b][nb2]==adj[c][nc2])
							{
                                commbc.push_back(adj[b][nb2]);
								nb2++;
								nc2++;
							}
							else if(adj[b][nb2]<adj[c][nc2])
							nb2++;
							else
							nc2++;
						}
						vector<int> commad;
						for(int na2=0,nd2=0;na2<deg[a]&&nd2<deg[d];)
						{
							if(adj[a][na2]==adj[d][nd2])
							{
                                commad.push_back(adj[a][na2]);
								na2++;
								nd2++;
							}
							else if(adj[a][na2]<adj[d][nd2])
							na2++;
							else
							nd2++;
						}
						for(int v:commab)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[153]++;
							}
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[123]++;
							}
						}
						for(int v:commbc)
						{
							if(find(commxd.begin(),commxd.end(),v)!=commxd.end())
							{
								rhs[230]++;
							}
						}
						for(int v:commad)
						{
							if(find(commxc.begin(),commxc.end(),v)!=commxc.end())
							{
								rhs[227]++;
							}
							if(find(commbc.begin(),commbc.end(),v)!=commbc.end())
							{
								rhs[122]++;
							}
						}
						}
					}
				}
			}
		}
	    //solving equations
        orbits[x][479]=c479[x];
		/*orbits[x][478]=(rhs[405]-20*orbits[x][479])/2;
		orbits[x][477]=rhs[404]-5*orbits[x][479];
		orbits[x][476]=(rhs[403]-6*orbits[x][478])/4;
		orbits[x][475]=(rhs[402]-12*orbits[x][477])/2;
		orbits[x][474]=(rhs[401]-2*orbits[x][475])/4;
		orbits[x][473]=rhs[400]-6*orbits[x][478]-30*orbits[x][479];
		orbits[x][472]=rhs[399]-4*orbits[x][477]-2*orbits[x][478]-20*orbits[x][479];
		orbits[x][471]=(rhs[398]-4*orbits[x][477])/2;
		orbits[x][470]=(rhs[397]-2*orbits[x][473])/2;
		orbits[x][469]=rhs[396]-2*orbits[x][473];
		orbits[x][468]=rhs[395]-2*orbits[x][475];
		orbits[x][467]=(rhs[394]-2*orbits[x][475])/2;
		orbits[x][466]=rhs[393]-orbits[x][469]-2*orbits[x][470]-2*orbits[x][473];
		orbits[x][465]=rhs[392]-2*orbits[x][467]-3*orbits[x][471];
		orbits[x][464]=(rhs[391]-4*orbits[x][473])/2;
		orbits[x][463]=(rhs[390]-3*orbits[x][472]);
		orbits[x][462]=rhs[389]-6*orbits[x][471];
        orbits[x][461]=(rhs[388]-2*orbits[x][469]-8*orbits[x][474]-4*orbits[x][475])/2;
		orbits[x][460]=rhs[387]-orbits[x][468]-4*orbits[x][474]-2*orbits[x][475];
		or*/
		fstream file;
		file.open("coefficients.txt",ios::in);
		if(file.is_open())
		{
			string p;
			while(getline(file,p))
			{
                //extracting and storing in vector
				vector<int> num;
				stringstream ss(p);
				string tmp;
				while(getline(ss,tmp,' '))
				{
					num.push_back(stoi(tmp));
				}
                orbits[x][num[0]+73]=rhs[num[0]];
				int coeff=num[1];
				int n=num[2];
				for(int i=0;i<n;i++)
				{
					orbits[x][num[0]+73]-=(num[2*i+3]*orbits[x][num[2*i+4]]);
				}
				orbits[x][num[0]+73]/=coeff;
				//orbits[x][num[0]+73]/=6;		        
			}
		}
		cout<<x<<endl;
		for(int j=73;j<=479;j++)
		{
			if(j!=73) fout<<" ";
            fout << orbits[x][j];
		}
		fout<<endl;
	}
	cout<<"hello\n";
}

int main()
{
    fin.open("graph2.in", fstream::in);
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
		for (int i;i<m;i++) {
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
    countnode6orbits();
    return 0;
}