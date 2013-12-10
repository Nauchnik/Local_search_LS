#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <thread>
#include "FPP.h";
#include "minisat/simp/SimpSolver.h";
#include "minisat/utils/System.h";

using namespace std;
class LS{
private:

	int indexof(int *a, int c){
		int res = -1;
		for (int k = 0; k<order; k++){
			if (a[k] == c){ res = k; break; }
		}
		return res;
	}
	int* lselems;
	int order;
	int &item(int i, int j);
	int * row(int i);
	int * column(int j);
	int * maindiag();
	int * subdiag();
	bool checkonlyone(int * a);
	void print(int *a){
		cout << endl;
		for (int i = 0; i<order; i++){
			cout << a[i] << " ";
		}
		cout << endl;
	}
	bool checkrow(int i)
	{
		int * t = row(i);
		return checkonlyone(t);
	}
	bool checkcolumn(int j){
		int * t = column(j);
		return checkonlyone(t);
	}
	bool checkmaindiag(){
		int * t = maindiag();
		return checkonlyone(t);
	}
	bool checksubdiag(){
		int * t = subdiag();

		return checkonlyone(t);
	}

	void transprows(int i1, int i2);
	void transpcolumns(int j1, int j2);
public:

	LS(string s, int n);
	LS(int *ss, int n);
	int it(int i, int j);
	bool parse(int * ss);
	bool check(bool diag);
	void normalizefirstrow();
	void normalizefirstcolumn();
	void reorder();
	string tostring();


};


class MOLS {
private:
	int order;
	int count;
	bool checkort(int ia, int ib);
	int checkortinc(int ia, int ib);
	int checkortinc_markings(int ia, int ib);
	int checkortinc_markings_tofile(int ia, int ib, string filename);

public:

	vector<int>markings;
	int printincompleteassumption(const char * fn);
	void print_assumptions_for_incidency_matrix(vector<int> IM, const char *fn);
	vector<int> extractassumptions(int numofsquare, int firstrow, int lastrow, int firstcolumn, int lastcolumn);
	vector<LS> Squares;
	MOLS();
	~MOLS();
	vector<int> incompleteassumptionmarkings(vector<int> current_markings);
	vector<int> incompleteassumption(vector<int> current_markings);

	void print_incomplete_assumption_tofile(vector<int> current_markings, string filename);
	void exportLStofile(const char * fn);
	MOLS(string s, int n, int r, bool wspaces);
	MOLS(vector<int> ss, int n, int r);
	void import(const char *fn, int n, int r);
	void setoutputfile(const char *fn);
	int ortogonalitycheck();
	int ortogonalitycheck_withmarkings();
	int ortogonalitycheck_withmarkings_tofile(string filename);
	int check(bool diag);
	void reorder();
	void print(const char * fn);
};

class localsearch{
private:
	
	int order_of_squares;
	int number_of_squares;
	int current_record;
	MOLS initial_point;
	MOLS record_point;
	vector<vector<int>> generate_areas(vector<vector<int>> input, int increment);
	vector<vector<int>> current_area;
	vector<int> current_markings; // points to a markings in current_area
	int iteration_number;
	int processed_points_number;
	string current_path;
	string base_cnf;
	string logfile_name;
	int current_radius;
	int maximum_radius;
	int number_of_processed_point;
	FPP init;
	vector<int> cnfinput;
public:
	void search_multithread(int num_of_threads, int n, int r, int start_from, int end_value, string path);
	void log(string action_description, MOLS current_MOLS, string logfile);
	void log(string action_description, string logfile);
	void log(string action_description, double time, string logfile);
	void print_area(vector<vector<int>> area, string filename);
	void generate_area(vector<int> markings, int increment);
	void initialize(int n, int r, int start_from, string path);
	bool get_next_point();

	void search(int n, int r, int start_from, int end_value, string path);
	bool extract_solution(string file, int n, int r, int & number, MOLS & m);
};

class localsearch_minisat{
private:
	unsigned unsat_count;
	unsigned sat_count;
	double min_unsat_time; 
	double min_sat_time;
	double max_unsat_time;
	double max_sat_time;
	double med_unsat_time; 
	double med_sat_time;
	double total_unsat_time; 
	double total_sat_time;
	unsigned solve_count;
	int number_of_vars_cnf;
	int order_of_squares;
	int number_of_squares;
	int current_record;
	MOLS initial_point;
	MOLS record_point;
	vector<vector<int>> generate_areas(vector<vector<int>> input, int increment);
	vector<vector<int>> current_area;
	vector<int> current_markings; // points to a markings in current_area
	int iteration_number;
	int processed_points_number;
	string current_path;
	Minisat::Problem Base;
	string logfile_name;
	int current_radius;
	int maximum_radius;
	int number_of_processed_point;
	FPP init;
	vector<int> cnfinput;
public:
	localsearch_minisat();
	unsigned core_len;
	double max_solving_time;
	void printSolving();
	void search_multithread(int num_of_threads, int n, int r, int start_from, int end_value, string path);
	void log(string action_description, MOLS current_MOLS, string logfile);
	void log(string action_description, string logfile);
	void log(string action_description, double time, string logfile);
	void print_area(vector<vector<int>> area, string filename);
	void generate_area(vector<int> markings, int increment);
	void initialize(int n, int r, int start_from, string path);
	bool get_next_point();
	void search(int n, int r, int start_from, int end_value, string path );
	bool extract_solution(vector<int> ss, int n, int r, int & number, MOLS & m);
};