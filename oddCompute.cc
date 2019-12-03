#include <iostream>
#include <vector>
#include <cmath>
#include <string.h>

using namespace std;

#define SPINE 2
#define LEAF 3
#define TOR 4
#define DEFAULT_ROLE 0
#define ROLE_TYPE int 

vector<int> nPrime(int n) {
	int k = 2;
	vector<int> v;
	while (k <= n) {
		bool isPrime = true;
		int t = sqrt(k);
		for (; t > 1; t--) {
			if (k%t == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime)
			v.push_back(k);
		k++;
	}
	return v;
}

int dPrime(int n, int m) {
	int pow = 0;
	while (n >= m) {
		int temp = n / m;
		pow += temp;
		n = temp;
	}
	return pow;
}

int C(int n, int m) {
	long long ans = 1;
	vector<int> v = nPrime(n);
	for (int i = 0; i < v.size(); i++) {
		int k = v.at(i),pow;
		pow = dPrime(n, k) - dPrime(m, k) - dPrime(n - m, k);
		for (int j = 0; j < pow; j++) {
			ans *= k;
			ans %= (int)(1e9 + 7);
		}
	}
	return (int)ans;
}


float oddsCompute( int nMasters, int nSpines, int nLeafs, int nTors, int nPods, float p_linkDown, float p_NMlinkDown, ROLE_TYPE roleType, ROLE_TYPE conRole = DEFAULT_ROLE) 
{
	/***
	  brief:
	  		Calculate the odds of losing connection between Master and Node
	  para：
   		    nMasters: number of master
   		      nSpine: number of spine
   		      nLeafs: number of leaf
   		       nTors: number of tor
   		       nPods: number of pod
   		  p_linkDown: the odds of losing connection between Node and Node
   		p_NMlinkDown: the odds of losing connection between Node and Master
			roleType: role type: SPINE, LEAF or TOR
	***/
	int neighborNumArr[4] = { nTors, nPods-1, nSpines/nLeafs, nLeafs };   
	if(conRole == DEFAULT_ROLE)
	{
		conRole = roleType;
		
	}
	else if(conRole == SPINE)
	{
		neighborNumArr[1] = nPods;
	} 
	float odds = 0;
	float oddsNeighbor = 0;
	float oddsTemp_1 = 0;
	float oddsTemp_2 = 0;

	oddsTemp_1 = pow(p_NMlinkDown, nMasters);
	if(roleType == 0)
	{
		odds = pow(p_NMlinkDown, nMasters);
		return odds;
	}
	else
	{
		roleType--;
		for (int nWorkLinks = 0; nWorkLinks < neighborNumArr[roleType]; nWorkLinks++)
		{
			float oddsTemp = oddsCompute( nMasters, nSpines, nLeafs, nTors, nPods, p_linkDown, p_NMlinkDown, roleType, conRole);
			oddsTemp_2 = oddsTemp_2 + C(neighborNumArr[roleType], nWorkLinks) * pow(p_linkDown, neighborNumArr[roleType] - nWorkLinks) * pow((1- p_linkDown), nWorkLinks) * pow(oddsTemp, nWorkLinks+1);
		}			
	}

	odds = oddsTemp_1 * oddsTemp_2;

	return odds;
}


int main() {

	int nMasters = 4;
	int nSpines = 16;
	int nLeafs = 4;
	int nTors = 4;
	int nPods = 2;
	float p_linkDown = 0.5;
	float p_NMlinkDown = 0.5;
	ROLE_TYPE roleType = SPINE;

	float odds = oddsCompute( nMasters, nSpines, nLeafs, nTors, nPods, p_linkDown, p_NMlinkDown, roleType);

	cout<<"The odds of loss of connection is: "<<odds<<endl;

	return 0;
}
