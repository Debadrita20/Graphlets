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
	orbits=(int **)malloc(n*sizeof(int *));
    for(int i=0;i<n;i++)
    {
        orbits[i]=(int *)malloc(407*sizeof(int));
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
			for(int na1=0;na1<deg[a];na1++)
			{
				int b=adj[a][na1];
				if(b==x) continue;
				for(int nx4=0;nx4<deg[x];nx4++)
				{
					int c=adj[x][nx4];
					if(c==a) continue;
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
						for(int nx2=0,na2=0;nx2<deg[x]&&nc2<deg[a];)
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
                                commcd.push_back(adj[b][nb2]);
								nb2++;
								nd2++;
							}
							else if(adj[b][nb2]<adj[d][nd2])
							nb2++;
							else
							nd2++;
						}
						vector<int> commxa;
						for(int nx2=0,na2=0;nx2<deg[x]&&nc2<deg[a];)
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
                                commcd.push_back(adj[b][nb2]);
								nb2++;
								nd2++;
							}
							else if(adj[b][nb2]<adj[d][nd2])
							nb2++;
							else
							nd2++;
						}
						vector<int> commxa;
						for(int nx2=0,na2=0;nx2<deg[x]&&nc2<deg[a];)
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
							if(find(commcd.begin(),commcd.end(),v)!=commcd.end())
							{
								rhs[263]++;
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
                                commcd.push_back(adj[b][nb2]);
								nb2++;
								nd2++;
							}
							else if(adj[b][nb2]<adj[d][nd2])
							nb2++;
							else
							nd2++;
						}
						vector<int> commxa;
						for(int nx2=0,na2=0;nx2<deg[x]&&nc2<deg[a];)
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
						for(int v:commcd)
						{
							if(find(commxb.begin(),commxb.end(),v)!=commxb.end())
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
                                commcd.push_back(adj[b][nb2]);
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

	}
}

/*void countP15_16_17()
{
    int count=0,countcd=0,countcac=0,countab=0,countccd=0,countcxb=0;
    set<PAIR> alreadydone;
    for(int i=0;i<m;i++)
	{
        int x=edges[i].a, y=edges[i].b;
    //current edge is middle edge
			for (int xi=0; xi<deg[x]; xi++) {
				int v1=adj[x][xi];
				if(v1==y) continue;
				for(int yi=0; yi<deg[y];yi++){
				int v2=adj[y][yi];
				if(v2==x) continue;
			if (!adjacent(v1,v2)&&!adjacent(v1,y)&&!adjacent(v2,x)&&(alreadydone.find(PAIR(x,v1))==alreadydone.end())&&(alreadydone.find(PAIR(y,v2))==alreadydone.end())) 
			{ 
					for(int ii=0;ii<deg[v1];ii++)
					{
						int v3=adj[v1][ii];
						if(v3==x||(alreadydone.find(PAIR(v3,v1))!=alreadydone.end())) continue;
						if(!adjacent(v3,x)&&!adjacent(v3,y)&&!adjacent(v3,v2))
						{
                            //printf("v1 %d deg=%d y %d deg=%d\n",v1,deg[v1],y,deg[y]);
							count+=(deg[v1]+deg[y]-4); alreadydone.insert(edges[i]);
							countcd+=(deg[v3]+deg[v2]-2);
							for(int t1=0,t2=0;t1<deg[v1]&&t2<deg[y];)
							{
								if(adj[v1][t1]==adj[y][t2])
								{
									countcac++;
									t1++; t2++;
								}
								else if(adj[v1][t1]>adj[y][t2]) t2++;
								else t1++;
							}
							vector<int> comm4;
							for(int t1=0,t2=0;t1<deg[v2]&&t2<deg[v3];)
							{
								if(adj[v2][t1]==adj[v3][t2])
								{
									rhs[246]+=2;
									comm4.push_back(adj[v2][t1]);
									t1++; t2++;
								}
								else if(adj[v2][t1]>adj[v3][t2]) t2++;
								else t1++;
							}
							vector<int> comm2,comm3;
							for(int t1=0,t2=0;t1<deg[v1]&&t2<deg[v3];)
							{
								if(adj[v1][t1]==adj[v3][t2])
								{
									countccd++;
									t1++; t2++;
									comm2.push_back(adj[v1][t1]);
								}
								else if(adj[v1][t1]>adj[v3][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[v2]&&t2<deg[y];)
							{
								if(adj[v2][t1]==adj[y][t2])
								{
									countccd++;
									t1++; t2++;
									comm3.push_back(adj[v2][t1]);
								}
								else if(adj[v2][t1]>adj[y][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[v2]&&t2<deg[x];)
							{
								if(adj[v2][t1]==adj[x][t2])
								{
									countcxb++;
									t1++; t2++;
								}
								else if(adj[v2][t1]>adj[x][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[v3]&&t2<deg[x];)
							{
								if(adj[v3][t1]==adj[x][t2])
								{
									countcxb++;
									t1++; t2++;
								}
								else if(adj[v3][t1]>adj[x][t2]) t2++;
								else t1++;
							}
		                    vector<int> comm,comm1;
							for(int t1=0,t2=0;t1<deg[v1]&&t2<deg[x];)
							{
								if(adj[v1][t1]==adj[x][t2])
								{
									countab++;
									t1++; t2++;
									comm.push_back(adj[v1][t1]);
								}
								else if(adj[v1][t1]>adj[x][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[y]&&t2<deg[x];)
							{
								if(adj[y][t1]==adj[x][t2])
								{
									countab++;
									t1++; t2++;
									comm1.push_back(adj[y][t1]);
								}
								else if(adj[y][t1]>adj[x][t2]) t2++;
								else t1++;
							}
							for(int t1=0;t1<deg[v2];t1++)
							{
								if(find(comm.begin(),comm.end(),adj[v2][t1])!=comm.end())
								{
									rhs[156]++;
									rhs[157]++;
									rhs[158]+=2;
									rhs[159]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[v2][t1])!=comm1.end())
								{
									rhs[165]++;
									rhs[166]++;
									rhs[167]+=2;
									rhs[168]++;
									rhs[169]++;
								}
								if(find(comm2.begin(),comm2.end(),adj[v2][t1])!=comm2.end())
								{
									rhs[247]++;
									rhs[248]++;
									rhs[249]++;
									rhs[250]++;
								}
							}
							for(int t1=0;t1<deg[v3];t1++)
							{
								if(find(comm.begin(),comm.end(),adj[v3][t1])!=comm.end())
								{
									rhs[165]++;
									rhs[166]++;
									rhs[167]+=2;
									rhs[168]++;
									rhs[169]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[v3][t1])!=comm1.end())
								{
									rhs[156]++;
									rhs[157]++;
									rhs[158]+=2;
									rhs[159]++;
								}
								if(find(comm3.begin(),comm3.end(),adj[v3][t1])!=comm3.end())
								{
									rhs[247]++;
									rhs[248]++;
									rhs[249]++;
									rhs[250]++;
								}
							}
							vector<int> comm5;
							for(int t1=0;t1<deg[x];t1++)
							{
								if(find(comm4.begin(),comm4.end(),adj[x][t1])!=comm4.end())
								{
									rhs[257]+=2;
									rhs[258]+=2;
									comm5.push_back(adj[x][t1]);
								}
							}
							for(int v:comm)
							{
								if(find(comm3.begin(),comm3.end(),v)!=comm3.end())
								{
									rhs[170]++;
									rhs[171]++;
									rhs[172]+=2;
									rhs[173]++;
									rhs[175]++;
								}
							}
							for(int v:comm1)
							{
								if(find(comm2.begin(),comm2.end(),v)!=comm2.end())
								{
									rhs[170]++;
									rhs[171]++;
									rhs[172]+=2;
									rhs[173]++;
									rhs[175]++;
								}
							}
							for(int v:comm3)
							{
								if(find(comm2.begin(),comm2.end(),v)!=comm2.end())
								{
									rhs[251]+=2;
									rhs[252]+=2;
									rhs[253]+=2;
								}
							}
							for(int t1=0;t1<deg[v1];t1++)
							{
								if(find(comm5.begin(),comm5.end(),adj[v1][t1])!=comm5.end())
								{
									rhs[259]++;
									rhs[260]++;
									rhs[261]+=2;
									rhs[262]++;
									rhs[263]++;
								}
								if(find(comm3.begin(),comm3.end(),adj[v1][t1])!=comm3.end())
								{
									rhs[120]++;
								}
							}
                            for(int t1=0;t1<deg[y];t1++)
							{
								if(find(comm.begin(),comm.end(),adj[y][t1])!=comm.end())
								{
									rhs[75]+=2;
									rhs[76]+=2;
									rhs[77]+=2;
								}
								if(find(comm5.begin(),comm5.end(),adj[y][t1])!=comm5.end())
								{
									rhs[259]++;
									rhs[260]++;
									rhs[261]+=2;
									rhs[262]++;
									rhs[263]++;
								}
								if(find(comm2.begin(),comm2.end(),adj[y][t1])!=comm2.end())
								{
									rhs[120]++;
								}
							}
							rhs[40]+=2*(countcac-1);
							rhs[41]+=2*(countcac-1);
							rhs[42]+=2*(countcac-1);
							rhs[63]+=countab;
							rhs[64]+=countab;
							rhs[66]+=(2*countab);
							rhs[67]+=countab;
							rhs[68]+=countab;
							rhs[106]+=countccd;
							rhs[107]+=(2*countccd);
							rhs[108]+=countccd;
							rhs[109]+=countccd;
							rhs[110]+=countccd;
							rhs[140]+=(countcxb-2);
							countcxb=0;
							countccd=0;
							countab=0;
							countcac=0;
						}
					}
					for(int ii=0;ii<deg[v2];ii++)
					{
						int v3=adj[v2][ii];
						if(v2==y||(alreadydone.find(PAIR(v3,v2))!=alreadydone.end())) continue;
						if(!adjacent(v3,x)&&!adjacent(v3,y)&&!adjacent(v3,v1))
						{
                            //printf("v2 %d deg=%d x %d deg=%d\n",v2,deg[v2],x,deg[x]);
                            //printf("%d %d %d %d %d\n",v3,v2,y,x,v1);
                            //printf("fhghjllu222\n");
							count+=(deg[x]+deg[v2]-4); alreadydone.insert(edges[i]);
							countcd+=(deg[v1]+deg[v3]-2);
							for(int t1=0,t2=0;t1<deg[v2]&&t2<deg[x];)
							{
								if(adj[v2][t1]==adj[x][t2])
								{
									countcac++;
									t1++; t2++;
								}
								else if(adj[v2][t1]>adj[x][t2]) t2++;
								else t1++;
							}
							vector<int> comm4;
							for(int t1=0,t2=0;t1<deg[v1]&&t2<deg[v3];)
							{
								if(adj[v1][t1]==adj[v3][t2])
								{
									rhs[246]+=2;
									comm4.push_back(adj[v1][t1]);
									t1++; t2++;
								}
								else if(adj[v1][t1]>adj[v3][t2]) t2++;
								else t1++;
							}
							vector<int> comm2,comm3;
							for(int t1=0,t2=0;t1<deg[v2]&&t2<deg[v3];)
							{
								if(adj[v2][t1]==adj[v3][t2])
								{
									countccd++;
									t1++; t2++;
									comm2.push_back(adj[v2][t1]);
								}
								else if(adj[v2][t1]>adj[v3][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[v1]&&t2<deg[x];)
							{
								if(adj[v1][t1]==adj[x][t2])
								{
									countccd++;
									t1++; t2++;
									comm3.push_back(adj[v1][t1]);
								}
								else if(adj[v1][t1]>adj[x][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[v1]&&t2<deg[y];)
							{
								if(adj[v1][t1]==adj[y][t2])
								{
									countcxb++;
									t1++; t2++;
								}
								else if(adj[v1][t1]>adj[y][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[v3]&&t2<deg[y];)
							{
								if(adj[v3][t1]==adj[y][t2])
								{
									countcxb++;
									t1++; t2++;
								}
								else if(adj[v3][t1]>adj[y][t2]) t2++;
								else t1++;
							}
                            vector<int> comm,comm1;
							for(int t1=0,t2=0;t1<deg[v2]&&t2<deg[y];)
							{
								if(adj[v2][t1]==adj[y][t2])
								{
									countab++;
									t1++; t2++;
									comm.push_back(adj[v2][t1]);
								}
								else if(adj[v2][t1]>adj[y][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[y]&&t2<deg[x];)
							{
								if(adj[y][t1]==adj[x][t2])
								{
									countab++;
									t1++; t2++;
									comm1.push_back(adj[y][t1]);
								}
								else if(adj[y][t1]>adj[x][t2]) t2++;
								else t1++;
							}
							for(int t1=0;t1<deg[v1];t1++)
							{
								if(find(comm.begin(),comm.end(),adj[v1][t1])!=comm.end())
								{
									rhs[156]++;
									rhs[157]++;
									rhs[158]+=2;
									rhs[159]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[v1][t1])!=comm1.end())
								{
									rhs[165]++;
									rhs[166]++;
									rhs[167]+=2;
									rhs[168]++;
									rhs[169]++;
								}
								if(find(comm2.begin(),comm2.end(),adj[v1][t1])!=comm2.end())
								{
									rhs[247]++;
									rhs[248]++;
									rhs[249]++;
									rhs[250]++;
								}
							}
							for(int t1=0;t1<deg[v3];t1++)
							{
								if(find(comm.begin(),comm.end(),adj[v3][t1])!=comm.end())
								{
									rhs[165]++;
									rhs[166]++;
									rhs[167]+=2;
									rhs[168]++;
									rhs[169]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[v3][t1])!=comm1.end())
								{
									rhs[156]++;
									rhs[157]++;
									rhs[158]+=2;
									rhs[159]++;
								}
								if(find(comm3.begin(),comm3.end(),adj[v3][t1])!=comm3.end())
								{
									rhs[247]++;
									rhs[248]++;
									rhs[249]++;
									rhs[250]++;
								}
							}
							vector<int> comm5;
							for(int t1=0;t1<deg[y];t1++)
							{
								if(find(comm4.begin(),comm4.end(),adj[y][t1])!=comm4.end())
								{
									rhs[257]+=2;
									rhs[258]+=2;
									comm5.push_back(adj[y][t1]);
								}
							}
							for(int t1=0;t1<deg[v2];t1++)
							{
								if(find(comm5.begin(),comm5.end(),adj[v2][t1])!=comm5.end())
								{
									rhs[259]++;
									rhs[260]++;
									rhs[261]+=2;
									rhs[262]++;
									rhs[263]++;
								}
								if(find(comm3.begin(),comm3.end(),adj[v2][t1])!=comm3.end())
								{
									rhs[120]++;
								}
							}
							for(int t1=0;t1<deg[x];t1++)
							{
								if(find(comm.begin(),comm.end(),adj[x][t1])!=comm.end())
								{
									rhs[75]+=2;
									rhs[76]+=2;
									rhs[77]+=2;
								}
								if(find(comm5.begin(),comm5.end(),adj[x][t1])!=comm5.end())
								{
									rhs[259]++;
									rhs[260]++;
									rhs[261]+=2;
									rhs[262]++;
									rhs[263]++;
								}
								if(find(comm2.begin(),comm2.end(),adj[x][t1])!=comm2.end())
								{
									rhs[120]++;
								}
							}
							for(int v:comm)
							{
								if(find(comm3.begin(),comm3.end(),v)!=comm3.end())
								{
									rhs[170]++;
									rhs[171]++;
									rhs[172]+=2;
									rhs[173]++;
									rhs[175]++;
								}
							}
							for(int v:comm1)
							{
								if(find(comm2.begin(),comm2.end(),v)!=comm2.end())
								{
									rhs[170]++;
									rhs[171]++;
									rhs[172]+=2;
									rhs[173]++;
									rhs[175]++;
								}
							}
							for(int v:comm3)
							{
								if(find(comm2.begin(),comm2.end(),v)!=comm2.end())
								{
									rhs[251]+=2;
									rhs[252]+=2;
									rhs[253]+=2;
								}
							}
							rhs[63]+=countab;
							rhs[64]+=countab;
							rhs[66]+=(2*countab);
							rhs[67]+=countab;
							rhs[68]+=countab;
							countab=0;
							rhs[40]+=2*(countcac-1);
							rhs[41]+=2*(countcac-1);
							rhs[42]+=2*(countcac-1);
							countcac=0;
							rhs[106]+=countccd;
							rhs[107]+=(2*countccd);
							rhs[108]+=countccd;
							rhs[109]+=countccd;
							rhs[110]+=countccd;
							countccd=0;
							rhs[140]+=(countcxb-2);
							countcxb=0;
						}
					}
			}
			
		    }
			}
			//current edge is side edge
			for(int xi=0;xi<deg[x];xi++)
			{     
				int v1=adj[x][xi];
				if(v1==y||adjacent(v1,y)||(alreadydone.find(PAIR(x,v1))!=alreadydone.end())) continue;
				for(int v1i=0;v1i<deg[v1];v1i++)
				{
					int v2=adj[v1][v1i];
					if(v2==x) continue;
					if(!adjacent(v2,x)&&!adjacent(v2,y)&&(alreadydone.find(PAIR(v2,v1))==alreadydone.end())) 
					{ 
					    for(int ii=0;ii<deg[v2];ii++)
					   {
						int v3=adj[v2][ii];
						if(v3==v1||(alreadydone.find(PAIR(v3,v2))!=alreadydone.end())) continue;
						if(!adjacent(v3,x)&&!adjacent(v3,y)&&!adjacent(v3,v1))
						{
                            //printf("v2 %d deg=%d x %d deg=%d\n",v2,deg[v2],x,deg[x]);
                            //printf("%d %d %d %d %d\n",v3,v2,v1,x,y);
                            //printf("wertyu44");
							count+=(deg[v2]+deg[x]-4); alreadydone.insert(edges[i]);
							countcd+=(deg[v3]+deg[y]-2);
							for(int t1=0,t2=0;t1<deg[v2]&&t2<deg[x];)
							{
								if(adj[v2][t1]==adj[x][t2])
								{
									countcac++;
									t1++; t2++;
								}
								else if(adj[v2][t1]>adj[x][t2]) t2++;
								else t1++;
							}
							vector<int> comm4;
							for(int t1=0,t2=0;t1<deg[y]&&t2<deg[v3];)
							{
								if(adj[y][t1]==adj[v3][t2])
								{
									rhs[246]+=2;
									comm4.push_back(adj[y][t1]);
									t1++; t2++;
								}
								else if(adj[y][t1]>adj[v3][t2]) t2++;
								else t1++;
							}
							vector<int> comm2,comm3;
							for(int t1=0,t2=0;t1<deg[v2]&&t2<deg[v3];)
							{
								if(adj[v2][t1]==adj[v3][t2])
								{
									countccd++;
									t1++; t2++;
									comm2.push_back(adj[v2][t1]);
								}
								else if(adj[v2][t1]>adj[v3][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[x]&&t2<deg[y];)
							{
								if(adj[x][t1]==adj[y][t2])
								{
									countccd++;
									t1++; t2++;
									comm3.push_back(adj[x][t1]);
								}
								else if(adj[x][t1]>adj[y][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[v1]&&t2<deg[y];)
							{
								if(adj[v1][t1]==adj[y][t2])
								{
									countcxb++;
									t1++; t2++;
								}
								else if(adj[v1][t1]>adj[y][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[v3]&&t2<deg[y];)
							{
								if(adj[v3][t1]==adj[y][t2])
								{
									countcxb++;
									t1++; t2++;
								}
								else if(adj[v3][t1]>adj[y][t2]) t2++;
								else t1++;
							}
                            vector<int> comm,comm1;
							for(int t1=0,t2=0;t1<deg[v2]&&t2<deg[v1];)
							{
								if(adj[v2][t1]==adj[v1][t2])
								{
									countab++;
									t1++; t2++;
									comm.push_back(adj[v2][t1]);
								}
								else if(adj[v2][t1]>adj[v1][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[v1]&&t2<deg[x];)
							{
								if(adj[v1][t1]==adj[x][t2])
								{
									countab++;
									t1++; t2++;
									comm1.push_back(adj[v1][t1]);
								}
								else if(adj[v1][t1]>adj[x][t2]) t2++;
								else t1++;
							}
							for(int t1=0;t1<deg[y];t1++)
							{
								if(find(comm.begin(),comm.end(),adj[y][t1])!=comm.end())
								{
									rhs[156]++;
									rhs[157]++;
									rhs[158]+=2;
									rhs[159]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[y][t1])!=comm1.end())
								{
									rhs[165]++;
									rhs[166]++;
									rhs[167]+=2;
									rhs[168]++;
									rhs[169]++;
								}
								if(find(comm2.begin(),comm2.end(),adj[y][t1])!=comm2.end())
								{
									rhs[247]++;
									rhs[248]++;
									rhs[249]++;
									rhs[250]++;
								}
							}
							for(int t1=0;t1<deg[v3];t1++)
							{
								if(find(comm.begin(),comm.end(),adj[v3][t1])!=comm.end())
								{
									rhs[165]++;
									rhs[166]++;
									rhs[167]+=2;
									rhs[168]++;
									rhs[169]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[v3][t1])!=comm1.end())
								{
									rhs[156]++;
									rhs[157]++;
									rhs[158]+=2;
									rhs[159]++;
								}
								if(find(comm3.begin(),comm3.end(),adj[v3][t1])!=comm3.end())
								{
									rhs[247]++;
									rhs[248]++;
									rhs[249]++;
									rhs[250]++;
								}
							}
							vector<int> comm5;
							for(int t1=0;t1<deg[v1];t1++)
							{
								if(find(comm4.begin(),comm4.end(),adj[v1][t1])!=comm4.end())
								{
									rhs[257]+=2;
									rhs[258]+=2;
									comm5.push_back(adj[v1][t1]);
								}
							}
							for(int t1=0;t1<deg[v2];t1++)
							{
								if(find(comm5.begin(),comm5.end(),adj[v2][t1])!=comm5.end())
								{
                                    rhs[259]++;
									rhs[260]++;
									rhs[261]+=2;
									rhs[262]++;
									rhs[263]++;
								}
								if(find(comm3.begin(),comm3.end(),adj[v2][t1])!=comm3.end())
								{
									rhs[120]++;
								}
							}
							for(int t1=0;t1<deg[x];t1++)
							{
								if(find(comm.begin(),comm.end(),adj[x][t1])!=comm.end())
								{
									rhs[75]+=2;
									rhs[76]+=2;
									rhs[77]+=2;
								}
								if(find(comm5.begin(),comm5.end(),adj[x][t1])!=comm5.end())
								{
                                    rhs[259]++;
									rhs[260]++;
									rhs[261]+=2;
									rhs[262]++;
									rhs[263]++;
								}
								if(find(comm2.begin(),comm2.end(),adj[x][t1])!=comm2.end())
								{
									rhs[120]++;
								}
							}
							for(int v:comm)
							{
								if(find(comm3.begin(),comm3.end(),v)!=comm3.end())
								{
									rhs[170]++;
									rhs[171]++;
									rhs[172]+=2;
									rhs[173]++;
									rhs[175]++;
								}
							}
							for(int v:comm1)
							{
								if(find(comm2.begin(),comm2.end(),v)!=comm2.end())
								{
									rhs[170]++;
									rhs[171]++;
									rhs[172]+=2;
									rhs[173]++;
									rhs[175]++;
								}
							}
							for(int v:comm3)
							{
								if(find(comm2.begin(),comm2.end(),v)!=comm2.end())
								{
									rhs[251]+=2;
									rhs[252]+=2;
									rhs[253]+=2;
								}
							}
							rhs[63]+=countab;
							rhs[64]+=countab;
							rhs[66]+=(2*countab);
							rhs[67]+=countab;
							rhs[68]+=countab;
							countab=0;
							rhs[40]+=2*(countcac-1);
							rhs[41]+=2*(countcac-1);
							rhs[42]+=2*(countcac-1);
							countcac=0;
							rhs[106]+=countccd;
							rhs[107]+=(2*countccd);
							rhs[108]+=countccd;
							rhs[109]+=countccd;
							rhs[110]+=countccd;
							countccd=0;
							rhs[140]+=(countcxb-2);
							countcxb=0;
						}
					    }
					}
				}
			}
			for(int yi=0;yi<deg[y];yi++)
			{
				int v1=adj[y][yi];
				if(v1==x||adjacent(v1,x)||(alreadydone.find(PAIR(y,v1))!=alreadydone.end())) continue;
				for(int v1i=0;v1i<deg[v1];v1i++)
				{
					int v2=adj[v1][v1i];
					if(v2==x||v2==y) continue;
					if(!adjacent(v2,x)&&!adjacent(v2,y)&&(alreadydone.find(PAIR(v2,v1))==alreadydone.end())) 
					{ 
					    for(int ii=0;ii<deg[v2];ii++)
					   {
						int v3=adj[v2][ii];
						if(v3==v1||(alreadydone.find(PAIR(v3,v2))!=alreadydone.end())) continue;
						if(!adjacent(v3,x)&&!adjacent(v3,y)&&!adjacent(v3,v1))
						{
                            //printf("v2 %d deg=%d y %d deg=%d\n",v2,deg[v2],y,deg[y]);
                            //printf("%d %d %d %d %d\n",v3,v2,v1,y,x);
                            //printf("asdfggd55");
							count+=(deg[y]+deg[v2]-4); alreadydone.insert(edges[i]);
							countcd+=(deg[v3]+deg[x]-2);
							for(int t1=0,t2=0;t1<deg[v2]&&t2<deg[y];)
							{
								if(adj[v2][t1]==adj[y][t2])
								{
									countcac++;
									t1++; t2++;
								}
								else if(adj[v2][t1]>adj[y][t2]) t2++;
								else t1++;
							}
							vector<int> comm4;
							for(int t1=0,t2=0;t1<deg[x]&&t2<deg[v3];)
							{
								if(adj[x][t1]==adj[v3][t2])
								{
									rhs[246]+=2;
									comm4.push_back(adj[x][t1]);
									t1++; t2++;
								}
								else if(adj[x][t1]>adj[v3][t2]) t2++;
								else t1++;
							}
							vector<int> comm2,comm3;
							for(int t1=0,t2=0;t1<deg[v2]&&t2<deg[v3];)
							{
								if(adj[v2][t1]==adj[v3][t2])
								{
									countccd++;
									t1++; t2++;
									comm2.push_back(adj[v2][t1]);
								}
								else if(adj[v2][t1]>adj[v3][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[x]&&t2<deg[y];)
							{
								if(adj[x][t1]==adj[y][t2])
								{
									countccd++;
									t1++; t2++;
									comm3.push_back(adj[x][t1]);
								}
								else if(adj[x][t1]>adj[y][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[v1]&&t2<deg[x];)
							{
								if(adj[v1][t1]==adj[x][t2])
								{
									countcxb++;
									t1++; t2++;
								}
								else if(adj[v1][t1]>adj[x][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[v3]&&t2<deg[v1];)
							{
								if(adj[v3][t1]==adj[v1][t2])
								{
									countcxb++;
									t1++; t2++;
								}
								else if(adj[v3][t1]>adj[v1][t2]) t2++;
								else t1++;
							}
                            vector<int> comm,comm1;
							for(int t1=0,t2=0;t1<deg[v2]&&t2<deg[v1];)
							{
								if(adj[v2][t1]==adj[v1][t2])
								{
									countab++;
									t1++; t2++;
									comm.push_back(adj[v2][t1]);
								}
								else if(adj[v2][t1]>adj[v1][t2]) t2++;
								else t1++;
							}
							for(int t1=0,t2=0;t1<deg[y]&&t2<deg[v1];)
							{
								if(adj[y][t1]==adj[v1][t2])
								{
									countab++;
									t1++; t2++;
									comm1.push_back(adj[y][t1]);
								}
								else if(adj[y][t1]>adj[v1][t2]) t2++;
								else t1++;
							}
							for(int t1=0;t1<deg[x];t1++)
							{
								if(find(comm.begin(),comm.end(),adj[v2][t1])!=comm.end())
								{
									rhs[156]++;
									rhs[157]++;
									rhs[158]+=2;
									rhs[159]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[v2][t1])!=comm1.end())
								{
									rhs[165]++;
									rhs[166]++;
									rhs[167]+=2;
									rhs[168]++;
									rhs[169]++;
								}
								if(find(comm2.begin(),comm2.end(),adj[x][t1])!=comm2.end())
								{
									rhs[247]++;
									rhs[248]++;
									rhs[249]++;
									rhs[250]++;
								}
							}
							for(int t1=0;t1<deg[v3];t1++)
							{
								if(find(comm.begin(),comm.end(),adj[v3][t1])!=comm.end())
								{
									rhs[165]++;
									rhs[166]++;
									rhs[167]+=2;
									rhs[168]++;
									rhs[169]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[v3][t1])!=comm1.end())
								{
									rhs[156]++;
									rhs[157]++;
									rhs[158]+=2;
									rhs[159]++;
								}
								if(find(comm3.begin(),comm3.end(),adj[v3][t1])!=comm3.end())
								{
									rhs[247]++;
									rhs[248]++;
									rhs[249]++;
									rhs[250]++;
								}
							}
							vector<int> comm5;
							for(int t1=0;t1<deg[v1];t1++)
							{
								if(find(comm4.begin(),comm4.end(),adj[v1][t1])!=comm4.end())
								{
									rhs[257]+=2;
									rhs[258]+=2;
									comm5.push_back(adj[v1][t1]);
								}
							}
							for(int t1=0;t1<deg[v2];t1++)
							{
								if(find(comm5.begin(),comm5.end(),adj[v2][t1])!=comm5.end())
								{
                                    rhs[259]++;
									rhs[260]++;
									rhs[261]+=2;
									rhs[262]++;
									rhs[263]++;
								}
								if(find(comm3.begin(),comm3.end(),adj[v2][t1])!=comm3.end())
								{
									rhs[120]++;
								}
							}
							for(int t1=0;t1<deg[y];t1++)
							{
								if(find(comm.begin(),comm.end(),adj[y][t1])!=comm.end())
								{
									rhs[75]+=2;
									rhs[76]+=2;
									rhs[77]+=2;
								}
								if(find(comm5.begin(),comm5.end(),adj[y][t1])!=comm5.end())
								{
                                    rhs[259]++;
									rhs[260]++;
									rhs[261]+=2;
									rhs[262]++;
									rhs[263]++;
								}
								if(find(comm2.begin(),comm2.end(),adj[y][t1])!=comm2.end())
								{
									rhs[120]++;
								}
							}
							for(int v:comm)
							{
								if(find(comm3.begin(),comm3.end(),v)!=comm3.end())
								{
									rhs[170]++;
									rhs[171]++;
									rhs[172]+=2;
									rhs[173]++;
									rhs[175]++;
								}
							}
							for(int v:comm1)
							{
								if(find(comm2.begin(),comm2.end(),v)!=comm2.end())
								{
									rhs[170]++;
									rhs[171]++;
									rhs[172]+=2;
									rhs[173]++;
									rhs[175]++;
								}
							}
							for(int v:comm3)
							{
								if(find(comm2.begin(),comm2.end(),v)!=comm2.end())
								{
									rhs[251]+=2;
									rhs[252]+=2;
									rhs[253]+=2;
								}
							}
							rhs[12]=count;
	                        rhs[57]=countcd;
	                        rhs[58]=2*countcd;
	                        rhs[59]=countcd;
							count=0;
							countcd=0;
							rhs[63]+=countab;
							rhs[64]+=countab;
							rhs[66]+=(2*countab);
							rhs[67]+=countab;
							rhs[68]+=countab;
							countab=0;
							rhs[40]+=2*(countcac-1);
							rhs[41]+=2*(countcac-1);
							rhs[42]+=2*(countcac-1);
							countcac=0;
							rhs[106]+=countccd;
							rhs[107]+=(2*countccd);
							rhs[108]+=countccd;
							rhs[109]+=countccd;
							rhs[110]+=countccd;
							countccd=0;
							rhs[140]+=(countcxb-2);
							countcxb=0;
						}
					    }
					}
				}
			}
    }
}*/

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
    countP15_16_17();
    return 0;
}