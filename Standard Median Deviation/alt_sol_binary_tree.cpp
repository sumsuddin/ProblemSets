#include <vector>
#include <map>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

int Val[200009];
int Cnt[200009];
int TCnt[200009];
long long Sum[200009];
int L[200009];
int R[200009];
int N = 0;
int _root = -1;

void insert(int root, int tp)
{
	if(_root==-1)
	{
		Val[0] = tp;
		Cnt[0] = 1;
		TCnt[0] = 1;
		Sum[0] = tp;
		L[0] = -1;
		R[0] = -1;
		N = 1;
		_root = 0;
	}
	else
	{
		if(Val[root] == tp)
			Cnt[root]++;
		else if(Val[root]>tp)
		{
			if(L[root]==-1)
			{
				Val[N] = tp;
				Sum[N] = tp;
				Cnt[N] = 1;
				TCnt[N] = 1;
				L[N] = -1;
				R[N] = -1;
				L[root] = N;
				N++;
			}
			else
				insert(L[root], tp);
		}
		else
		{
			if(R[root]==-1)
			{
				Val[N] = tp;
				Sum[N] = tp;
				Cnt[N] = 1;
				TCnt[N] = 1;
				L[N] = -1;
				R[N] = -1;
				R[root] = N;
				N++;
			}
			else
				insert(R[root], tp);
		}
		
		TCnt[root] = Cnt[root];
		
		if(L[root]!=-1)
			TCnt[root] += TCnt[L[root]];
		if(R[root]!=-1)
			TCnt[root] += TCnt[R[root]];
			
		long long temp = Val[root];
		Sum[root] = temp * Cnt[root];
		
		if(L[root]!=-1)
			Sum[root] += Sum[L[root]];
		if(R[root]!=-1)
			Sum[root] += Sum[R[root]];
	}
}

bool remove(int root, int tp)
{
	if(root < 0)
		return false;
		
	int res = false;
	
	if(Val[root] == tp)
	{
		if(Cnt[root] > 0)
		{
			Cnt[root]--;
			res = true;
		}
	}
	else
	{
		if(Val[root]>tp)
		{
			if(L[root]!=-1)
				res = remove(L[root], tp);
		}
		else
		{
			if(R[root]!=-1)
				res = remove(R[root], tp);
		}
	}
	
	TCnt[root] = Cnt[root];
		
	if(L[root]!=-1)
		TCnt[root] += TCnt[L[root]];
	if(R[root]!=-1)
		TCnt[root] += TCnt[R[root]];
		
	long long temp = Val[root];
	Sum[root] = temp * Cnt[root];
	
	if(L[root]!=-1)
		Sum[root] += Sum[L[root]];
	if(R[root]!=-1)
		Sum[root] += Sum[R[root]];
		
	return res;
}

long long Lcal(int root, int tp, long long leftSum, int leftCount)
{
	if(root < 0)
		return 0;
		
	int lcount = 0;
	int rcount = 0;
	
	if(L[root]!=-1)
		lcount = TCnt[L[root]];
	if(R[root]!=-1)
		rcount = TCnt[R[root]];
	
	if(lcount>=tp)
		return Lcal(L[root], tp, leftSum, leftCount);
	else if(lcount+Cnt[root]>=tp)
	{
		if(L[root]!=-1)
		{
			leftSum += Sum[L[root]];
			leftCount += TCnt[L[root]];
		}
		
		long long temp = leftCount;
		temp *= Val[root];
		return temp - leftSum;
	}
	else
	{
		long long temp = Cnt[root];
		temp *= Val[root];
		
		if(L[root]!=-1)
			temp += Sum[L[root]];
		
		return Lcal(R[root], tp-(lcount+Cnt[root]), leftSum +  temp, leftCount + lcount + Cnt[root]);
	}
	
}

long long Rcal(int root, int tp, long long rightSum, int rightCount)
{
	if(root < 0)
		return 0;
		
	int lcount = 0;
	int rcount = 0;
	
	if(L[root]!=-1)
		lcount = TCnt[L[root]];
	if(R[root]!=-1)
		rcount = TCnt[R[root]];
	
	if(rcount>=tp)
		return Rcal(R[root], tp, rightSum, rightCount);
	else if(rcount+Cnt[root]>=tp)
	{
		if(R[root]!=-1)
		{
			rightSum += Sum[R[root]];
			rightCount += TCnt[R[root]];
		}
		
		long long temp = rightCount;
		temp *= Val[root];
		return rightSum - temp;
	}
	else
	{
		long long temp = Cnt[root];
		temp *= Val[root];
		
		if(R[root]!=-1)
			temp += Sum[R[root]];
		
		return Rcal(L[root], tp-(rcount+Cnt[root]), rightSum +  temp, rightCount + rcount + Cnt[root]);
	}
	
}

int main()
{
	freopen("in.txt","r",stdin);
    freopen("out_alt.txt","w",stdout);

	int cas;
	
	scanf("%d",&cas);

	assert(cas <= 100000);
	
	for(int cas1=1;cas1<=cas;cas1++)
	{
		int t, a, b;
		scanf("%d %d",&t ,&a);
		
		assert(t >= 1 && t <= 3);
		assert(a >= 1 && a <= 1000000000);
		
		if(t==1)
		{
			insert(_root, a);
		}
		else if(t==2)
		{
			remove(_root, a);
		}
		else if(t==3)
		{
			scanf("%d",&b);
			
			assert(b >= 1 && b <= 1000000000);
			
			if(remove(_root, a))
				insert(_root, b);
		}
		
		long long total = 0;
		
		if(TCnt[_root] > 0)
		{
			int mid = TCnt[_root] / 2;
			
			if(TCnt[_root]%2!=0)
				mid++;
			
			total += Lcal(_root, mid, 0, 0);
			total += Rcal(_root, mid, 0, 0);
		}
	
		printf("%lld\n", total);
	}
	
	//printf("%d\n",N);

	return 0;
}

