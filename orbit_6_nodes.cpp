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

void countP15()
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
									rhs[159]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[v2][t1])!=comm1.end())
								{
									rhs[165]++;
									rhs[166]++;
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
									rhs[168]++;
									rhs[169]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[v3][t1])!=comm1.end())
								{
									rhs[156]++;
									rhs[157]++;
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
									comm5.push_back(adj[x][t1]);
								}
							}
							for(int v:comm)
							{
								if(find(comm3.begin(),comm3.end(),v)!=comm3.end())
								{
									rhs[170]++;
									rhs[171]++;
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
									rhs[173]++;
									rhs[175]++;
								}
							}
							for(int v:comm3)
							{
								if(find(comm2.begin(),comm2.end(),v)!=comm2.end())
								{
									rhs[251]+=2;
								}
							}
							for(int t1=0;t1<deg[v1];t1++)
							{
								if(find(comm5.begin(),comm5.end(),adj[v1][t1])!=comm5.end())
								{
									rhs[259]++;
									rhs[260]++;
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
									rhs[77]+=2;
								}
								if(find(comm5.begin(),comm5.end(),adj[y][t1])!=comm5.end())
								{
									rhs[259]++;
									rhs[260]++;
								}
								if(find(comm2.begin(),comm2.end(),adj[y][t1])!=comm2.end())
								{
									rhs[120]++;
								}
							}
							rhs[40]+=2*(countcac-1);
							rhs[42]+=2*(countcac-1);
							rhs[63]+=countab;
							rhs[64]+=countab;
							rhs[67]+=countab;
							rhs[68]+=countab;
							rhs[106]+=countccd;
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
									rhs[159]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[v1][t1])!=comm1.end())
								{
									rhs[165]++;
									rhs[166]++;
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
									rhs[168]++;
									rhs[169]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[v3][t1])!=comm1.end())
								{
									rhs[156]++;
									rhs[157]++;
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
									comm5.push_back(adj[y][t1]);
								}
							}
							for(int t1=0;t1<deg[v2];t1++)
							{
								if(find(comm5.begin(),comm5.end(),adj[v2][t1])!=comm5.end())
								{
									rhs[259]++;
									rhs[260]++;
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
									rhs[77]+=2;
								}
								if(find(comm5.begin(),comm5.end(),adj[x][t1])!=comm5.end())
								{
									rhs[259]++;
									rhs[260]++;
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
									rhs[173]++;
									rhs[175]++;
								}
							}
							for(int v:comm3)
							{
								if(find(comm2.begin(),comm2.end(),v)!=comm2.end())
								{
									rhs[251]+=2;
								}
							}
							rhs[63]+=countab;
							rhs[64]+=countab;
							rhs[67]+=countab;
							rhs[68]+=countab;
							countab=0;
							rhs[40]+=2*(countcac-1);
							rhs[42]+=2*(countcac-1);
							countcac=0;
							rhs[106]+=countccd;
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
									rhs[159]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[y][t1])!=comm1.end())
								{
									rhs[165]++;
									rhs[166]++;
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
									rhs[168]++;
									rhs[169]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[v3][t1])!=comm1.end())
								{
									rhs[156]++;
									rhs[157]++;
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
									comm5.push_back(adj[v1][t1]);
								}
							}
							for(int t1=0;t1<deg[v2];t1++)
							{
								if(find(comm5.begin(),comm5.end(),adj[v2][t1])!=comm5.end())
								{
                                    rhs[259]++;
									rhs[260]++;
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
									rhs[77]+=2;
								}
								if(find(comm5.begin(),comm5.end(),adj[x][t1])!=comm5.end())
								{
                                    rhs[259]++;
									rhs[260]++;
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
									rhs[173]++;
									rhs[175]++;
								}
							}
							for(int v:comm3)
							{
								if(find(comm2.begin(),comm2.end(),v)!=comm2.end())
								{
									rhs[251]+=2;
								}
							}
							rhs[63]+=countab;
							rhs[64]+=countab;
							rhs[67]+=countab;
							rhs[68]+=countab;
							countab=0;
							rhs[40]+=2*(countcac-1);
							rhs[42]+=2*(countcac-1);
							countcac=0;
							rhs[106]+=countccd;
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
									rhs[159]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[v2][t1])!=comm1.end())
								{
									rhs[165]++;
									rhs[166]++;
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
									rhs[168]++;
									rhs[169]++;
								}
								if(find(comm1.begin(),comm1.end(),adj[v3][t1])!=comm1.end())
								{
									rhs[156]++;
									rhs[157]++;
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
									comm5.push_back(adj[v1][t1]);
								}
							}
							for(int t1=0;t1<deg[v2];t1++)
							{
								if(find(comm5.begin(),comm5.end(),adj[v2][t1])!=comm5.end())
								{
                                    rhs[259]++;
									rhs[260]++;
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
									rhs[77]+=2;
								}
								if(find(comm5.begin(),comm5.end(),adj[y][t1])!=comm5.end())
								{
                                    rhs[259]++;
									rhs[260]++;
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
									rhs[173]++;
									rhs[175]++;
								}
							}
							for(int v:comm3)
							{
								if(find(comm2.begin(),comm2.end(),v)!=comm2.end())
								{
									rhs[251]+=2;
								}
							}
							rhs[63]+=countab;
							rhs[64]+=countab;
							rhs[67]+=countab;
							rhs[68]+=countab;
							countab=0;
							rhs[40]+=2*(countcac-1);
							rhs[42]+=2*(countcac-1);
							countcac=0;
							rhs[106]+=countccd;
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
    rhs[12]=count;
	rhs[57]=countcd;
	rhs[59]=countcd;
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
    countP15();
    return 0;
}