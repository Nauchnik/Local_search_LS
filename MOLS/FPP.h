#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "minisat/core/solver.h";
using namespace std;

class FPP {
	vector <int*> M;
public:	
	long int nVars;
	long int nClauses;
	int dimension;

	::FPP(){
		nVars=0;
		nClauses=0;
		M.clear();
	}
	vector<int> Sort(vector<int> a, int nV, int n){
		nVars=nV;		
		return HSort(a,n);
	}
	int twoceil(int k){//returns closest upside degree of 2
	int t=1;
	while (t<k){t=t*2;}
	return t;
	}
	int strtoi(string s){
		int x = atoi(s.c_str());
		return x;
	}

	string inttostr(int number)
	{
	   stringstream ss;//create a stringstream
	   ss << number;//add number to the stream
	   return ss.str();//return a string with the contents of the stream
	}
	vector <int> Row (int i);
	vector <int> Column (int j);
	vector <int> HMerge (vector<int> a, vector<int>b, int n);
	vector <int> HSort (vector<int> a, int n);
	vector <int> SHSort (vector<int> a, int lown, int n);
	vector <int> SMerge (vector<int> a, vector <int> b, int n);
	vector <int> Selectodd(vector<int> a);
	vector <int> Selecteven(vector<int> a);
	void seqcounter(vector<int> a);
	void Newvareqor(int nv, vector<int> right);
	void onlyonesimple(vector<int> t);
	void onlyonesimple_mod(vector<int> t);
	void varfollowsonlyonesimple(vector<int> t, int varid);
	void varfollowsatleastone(vector<int> t,int varid);
	void onlyonecomplex(vector<int> t);
	vector<int> order_appendix(vector<int> h, int t);
	vector<int> vecnewvareqand(const vector<int> r1, const vector<int>r2);
	void Newvareqand(int nv, vector<int> right);
	void Projectiveplane(int n);
	void Print (const char * fn);
	void Append_order(const char * fn, vector<int> h, int t);
	void Append_assumptions (const char * fn, vector<int> h);
	void Dump (char * fn);
	void LSset (int n, int r);
	void LSSetincomplete (int n, int h);
	void LSSetincomplete_improved (int n, int h);
	void LSSetincomplete_improved_all(int n, int h);
	vector<int> LSSetinc_markings(int n, int lim);
	vector<int> LSSetincomplete_improved_all_general(int n);
	void LSSetincomplete_test (int n, int h);
	vector<int> LSSetinc_markings_experimental(int n,int r);
	Minisat::Problem convert_to_problem_base();
	Minisat::Problem convert_to_problem_vector(vector<int> B);
	int LSvar(int r, int i, int j, int k, int n){
		int res=n*n*n*r+n*n*i+n*j+k+1;
		return res;
	}
};
