
#include "MOLS.h";

using namespace std;

static inline double cpuTime(void)
{
	return (double)clock() / CLOCKS_PER_SEC;
}
int ijktov(int i, int j, int k, int n){
	return i*n*n + j*n + k + 1;
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
localsearch_history::localsearch_history(){
	current_index=0;
	max_radius=1;
	number_of_iterations=0;
	number_of_points_processed=0;
}

localsearch_history::localsearch_history(int radius){
	current_index=0;
	max_radius=radius;
	number_of_iterations=0;
	number_of_points_processed=0;
}

void localsearch_history::add_area(MOLS H, int best_known_value){
	localsearch_area a(H.markings,H,max_radius,best_known_value);
	areas.push_back(a);
	current_index=areas.size()-1;
	number_of_iterations++;
}
bool localsearch_history::getnextpoint(vector<int> &point, MOLS &H){
	vector<int> r;
	MOLS a;
  	bool b=areas[current_index].get_next_point(r,a);
	while ((b==false)&&(current_index>0)){
		current_index--;
		b=areas[current_index].get_next_point(r,a);
	}
	point.swap(r);
	number_of_points_processed++;
	return b;
}

localsearch_area::localsearch_area(vector<int> markings, int radius){
	markings_this.swap(markings);
	area_radius=radius;
	current_index=0;
	generate_area(radius);
}
localsearch_area::localsearch_area(vector<int> markings, MOLS H, int radius, int best_known_value){
	BKV=best_known_value;
	MOLS_this=H;
	markings_this.swap(markings);
	area_radius=radius;
	current_index=0;
	generate_area(radius);
}
vector<vector<int>> localsearch_area::generate_area_1(int sign, vector<int>markings){
	vector<vector<int>>res;
	if (sign==1){
		for(int i=0;i<markings.size();i++){
			if (markings[i]==0){
				vector<int> tmp_markings(markings);
					tmp_markings[i]=1;
					res.push_back(tmp_markings);
				}
			}
		}else {
			for(int i=0;i<markings_this.size();i++){
				if (markings[i]==1){
					vector<int> tmp_markings(markings);
					tmp_markings[i]=0;
					res.push_back(tmp_markings);
				}
			}
		}	
	return res;
}

vector<vector<int>> localsearch_area::generate_area_rad(int radius, vector<int> markings){
	if (radius==0) exit;
	
	int radius_value=abs(radius);
	int radius_sign=radius/radius_value;
	vector<vector<int>> res=generate_area_1(radius_sign,markings);
	if (radius_value==1) return res;
	vector<vector<int>> temp_res;
	
	for (int r=2;r<=radius_value;r++){
		for (int i=0;i<res.size();i++){
			vector<vector<int>> t=generate_area_1(radius_sign,res[i]);
			for (int j=0;j<t.size();j++){
				vector<int> p(t[j]);
				temp_res.push_back(p);				
			}
			t.clear();
		}
		res.clear();
		res.swap(temp_res);
	}
	return res;
}
void localsearch_area::generate_area(int radius){
	area_this.clear();
	area_radius=radius;
	int radius_value=abs(radius);
	int radius_sign=radius/radius_value;
	for (int r=1;r<=radius_value;r++){
		vector<vector<int>>temp_radius=generate_area_rad(r*radius_sign, markings_this);
		cout<<endl<<"area of radius "<<r<<" size before cleanup "<<temp_radius.size();
		cleanup(temp_radius);
		cout<<" after cleanup "<<temp_radius.size()<<endl;
		for (int i=0;i<temp_radius.size();i++){
			vector<int> t(temp_radius[i]);
			area_this.push_back(t);
		}
	}
	
}
string localsearch_area::totext (vector<int> a){
	string res;
	for (int i=0;i<a.size();i++){
		if (a[i]==0){res+='0';}else{res+='1';}
	}
	return res;
}
void localsearch_area::cleanup(vector<vector<int>>&a){
	for (int i=0;i<a.size();i++){
		for (int j=i+1;j<a.size();j++){
			if (a[i]==a[j]){a.erase(a.begin()+j);}
		}
	}
}
void localsearch_area::zip (){
	area_this.clear();	
}
void localsearch_area::unzip (){
	generate_area(area_radius);
}

bool localsearch_area::get_next_point(vector<int> & point, MOLS & H){
	if (area_this.size()==0){unzip();}
	if (current_index==area_this.size()){return false;}
	vector<int> r(area_this[current_index]);
	current_index++;
	H=MOLS_this;
	point.swap(r);
	return true;
}

LS::LS(int * b, int n){
	lselems = b;
	order = n;
	cout << endl;
//	report << endl;
	for (int i = 0; i<order; i++){
		for (int j = 0; j<order; j++){
			cout << b[i*order + j] << " ";
			//report << b[i*order + j] << " ";
		}
		cout << endl;
	//	report << endl;
	}
}
LS::LS(string s, int n){
	order = n;
	int *a = new (int[n*n*n]);
	int *b = new (int[n*n]);
	string t;
	int i1 = 0;
	int i2 = 0;
	int k = 0;
	int g = 0;
	for (int i = 0; i<s.length(); i++){
		if (s[i] == ' ')
		{
			i2 = i;
			t = s.substr(i1, i2 - i1);
			//cout <<t<<" ";
			a[k] = strtoi(t);
			if (a[k]>0) {/*cout<<k%n<< " ";*/b[g] = k%n; g++; }

			k++;
			//if ((k>0)&&((k%(n*n)==0))) cout<<endl;
			//if (k==n*n*n) cout <<endl<<endl;
			i1 = i2;
		}
	}
	lselems = b;
}
string LS::tostring(){
	string a;
	for (int i = 0; i<order; i++){
		for (int j = 0; j<order; j++){
			a = a + inttostr(item(i, j)) + ' ';
		}
		a = a + '\n';
	}
	return a;
}

int * LS::row(int i)
{
	int * a;
	a = new (int[order]);
	for (int k = 0; k<order; k++){
		a[k] = lselems[i*order + k];
	}
	return a;
}

int * LS::column(int j)
{
	int * a;
	a = new (int[order]);
	for (int k = 0; k<order; k++){
		a[k] = lselems[k*order + j];
	}
	return a;
}
int * LS::maindiag()
{
	int * a;
	a = new (int[order]);
	for (int k = 0; k<order; k++){
		a[k] = lselems[k*order + k];
	}
	return a;
}
int * LS::subdiag()
{
	int * a;
	a = new (int[order]);
	for (int k = 0; k<order; k++){
		a[k] = lselems[k*order + order - k - 1];
	}

	return a;
}

int &LS::item(int i, int j){
	return lselems[i*order + j];
}
int LS::it(int i, int j){
	int r = lselems[i*order + j];
	return r;
}

bool LS::checkonlyone(int * a){
	bool res = true;
	int *cc = new (int[order]);
	for (int k = 0; k<order; k++){
		cc[k] = 0;
	}
	for (int k = 0; k<order; k++){
		cc[a[k]]++;
		if (cc[a[k]]>1) { res = false; }
	}
	return res;
}

void LS::transprows(int i1, int i2){
	int *t = row(i1);
	for (int k = 0; k<order; k++){
		item(i1, k) = item(i2, k);
	}
	for (int k = 0; k<order; k++){
		item(i2, k) = t[k];
	}
}

void LS::transpcolumns(int j1, int j2){
	int *t = column(j1);
	for (int k = 0; k<order; k++){
		item(k, j1) = item(k, j2);
	}
	for (int k = 0; k<order; k++){
		item(k, j2) = t[k];
	}
}
bool LS::check(bool diag){

	bool a1 = true;
	for (int k = 0; k<order; k++){
		if (checkrow(k) == false){
			a1 = false;
			cout << "row " << k << " fail" << endl;
//			report << "row " << k << " fail" << endl;
		}
	}
	bool a2 = true;
	for (int k = 0; k<order; k++){
		if (checkcolumn(k) == false){
			a1 = false;
			cout << "column " << k << " fail" << endl;
//			report << "column " << k << " fail" << endl;
		}
	}
	bool a3 = true;
	if (diag == true){
		{
			if (checkmaindiag() == false){
				a3 = false;
				cout << "maindiag fail" << endl;
//				report << "maindiag fail" << endl;
			}
			if (checksubdiag() == false){
				a3 = false;
				cout << "subdiag fail" << endl;
//				report << "subdiag fail" << endl;
			}
		}
	}
	bool res = a1&&a2&&a3;
	return res;
}
void LS::normalizefirstrow(){
	int * r1 = row(1);
	for (int k = 0; k<order; k++){
		if (r1[k] != k){
			int tt1 = indexof(r1, k);
			transpcolumns(tt1, k);
			r1[tt1] = r1[k];
			r1[k] = k;
		}
	}
}
void LS::normalizefirstcolumn(){
	int * c1 = column(0);
	for (int k = 0; k<order; k++){
		if (c1[k] != k){
			int tt1 = indexof(c1, k);
			transprows(tt1, k);
			c1[tt1] = c1[k];
			c1[k] = k;
		}
	}
}
void LS::reorder(){
	int *r1 = row(0);
	int *f = new(int[order]);
	for (int k = 0; k<order; k++){
		f[r1[k]] = k;
	}
	print(r1);
	print(f);

	int *t = new (int[order*order]);
	for (int k = 0; k<order*order; k++){
		t[k] = f[lselems[k]];
		//	cout <<endl<<"("<<lselems[k]<<")->"<<f[lselems[k]]<<endl;
	}
	for (int k = 0; k<order*order; k++){
		lselems[k] = t[k];
	}
}

void MOLS::reorder(){
	for (int i = 0; i<count; i++){
		Squares[i].reorder();
	}
}
MOLS::MOLS()
{
	order = 0;
	count = 0;
}
MOLS::~MOLS(){
	//out.close();
}
void MOLS::setoutputfile(const char *fn){
	//out.open(fn,ios::app);
}
vector<int> MOLS::extractassumptions(int numofsquare, int firstrow, int lastrow, int firstcolumn, int lastcolumn){
	vector<int> a;
	for (int i = firstrow; i<lastrow; i++){
		for (int j = firstcolumn; j<lastcolumn; j++){
			int t = Squares[numofsquare].it(i, j);
			int r = numofsquare *order*order*order + i*order*order + j*order + t + 1;
			a.push_back(r);
		}
	}
	return a;
}
void MOLS::import(const char*fn, int n, int r){
	ifstream myfile(fn);
	int *b;
	order = n;
	count = r;
	b = new (int[n*n*r]);
	int sc = 0;
	if (myfile.is_open())
	{
		while (!myfile.eof())//( myfile.good() )
		{
			string line = "";
			getline(myfile, line);
			for (int i = 0; i<line.length(); i++){
				if ((line[i] >= '0') && (line[i]<'0' + 10)){
					b[sc] = line[i] - '0';
					sc++;
				}
			}
		}
		for (int k = 0; k<r; k++){
			int *z = new (int[n*n]);
			for (int t = 0; t<n*n; t++){
				z[t] = b[k*n*n + t];
			}
			LS h = LS(z, order);
			Squares.push_back(h);
		}
	}

}

struct index_int{
	int i;
	int j;
};

int MOLS::printincompleteassumption(const char * fn){

	LS a = Squares[1];
	LS b = Squares[2];
	int count = 0;
	cout << endl << "PAIRS" << endl;
	int * r = new (int[order*order]);
	vector<int> pairs;
	for (int k = 0; k<order*order; k++){ r[k] = 0; }
	for (int i = 0; i<order; i++){
		for (int j = 0; j<order; j++){
			int h = a.it(i, j)*order + b.it(i, j);
			cout << h << " ";
			pairs.push_back(h);
			r[h]++;
			if (r[h] == 1){ count++; }
		}
		cout << endl;
	}
	vector<int> indexesweneed;
	cout << endl;
	for (int i = 0; i<order; i++){
		for (int j = 0; j<order; j++){
			int h = a.it(i, j)*order + b.it(i, j);
			if (r[h] != 1) {
				pairs[i*order + j] = 0;
				indexesweneed.push_back(0);
				cout << 0 << " ";
			}
			if (r[h] == 1) {
				index_int a;
				a.i = i;
				a.j = j;
				indexesweneed.push_back(1);
				cout << 1 << " ";
				pairs[i*order + j] = 0;
			}
		}
		cout << endl;
	}
	print_assumptions_for_incidency_matrix(indexesweneed, fn);
	return count;
}
vector<int> MOLS::incompleteassumption(vector<int> current_markings){
	// returns values of markings to be put to CNF
	vector<int> result;
	for (int i = 0; i<current_markings.size(); i++){
		if (current_markings[i] == 1){
			int temp_var = 3 * order*order*order + i + 1;
			result.push_back(temp_var);
		}
	}	
	//using current_markings returns values of existing square
	for (int i = 0; i<order; i++){
		for (int j = 0; j<order; j++){
			if ((current_markings[i*order + j] == 1) && (markings[i*order + j] == 1)){
				for (int u = 0; u<count; u++){
					int temp_var = u*order*order*order + i*order*order + j*order + Squares[u].it(i, j) + 1;
					result.push_back(temp_var);
				}
			}
		}
	}
	return result;
}
void MOLS::print_incomplete_assumption_tofile(vector<int> current_markings, string filename){
	ofstream out;
	out.open(filename.c_str());
	out << "Current Markings" << endl << endl;
	for (int i = 0; i < order; i++){
		for (int j = 0; j < order; j++){
			out << current_markings[i*order + j] << " ";
		}
		out << endl;
	}
	out << endl << endl << "Squares assumptions" << endl << endl;
	for (int k = 0; k < count; k++){
		for (int i = 0; i<order; i++){
			for (int j = 0; j<order; j++){
				if ((current_markings[i*order + j] == 1) && (markings[i*order + j] == 1)){
					out << Squares[k].it(i, j) << " ";
				}
				else { out << "x "; }

			}
			out << endl;
		}
	}
	out.close();
}

vector<int> MOLS::incompleteassumptionmarkings(vector<int> current_markings){
	// returns values of markings to be put to CNF
	cout << endl;
	vector<int> result;
	for (int i = 0; i<current_markings.size(); i++){
		if (current_markings[i] == 1){
			int temp_var = 3 * order*order*order + i + 1;
			result.push_back(temp_var);
		}
	}
	return result;
}
void MOLS::print_assumptions_for_incidency_matrix(vector<int> IM, const char *fn){
	ofstream out(fn);
	//out.open(fn);
	cout << "Assumptions" << endl;
	//	out<<"Assumptions"<<endl;
	for (int i = 0; i<order; i++){
		for (int j = 0; j<order; j++){
			if (IM[i*order + j] == 1){
				for (int r = 0; r<3; r++){
					cout << r* order * order * order + ijktov(i, j, Squares[r].it(i, j), order) << " 0" << endl;
					out << r* order * order * order + ijktov(i, j, Squares[r].it(i, j), order) << " 0" << endl;
				}
			}
		}
	}
	out.close();
}
bool MOLS::checkort(int ia, int ib){
	bool f = true;
	LS a = Squares[ia];
	LS b = Squares[ib];
	int * r = new (int[order*order]);
	for (int k = 0; k<order*order; k++){ r[k] = 0; }
	for (int i = 0; i<order; i++){
		for (int j = 0; j<order; j++){
			int h = a.it(i, j)*order + b.it(i, j);
			r[h]++;
			if (r[h]>1){
				f = false;
				cout << "orthogonality check failed" << endl;
//				report << "orthogonality check failed" << endl;
			}
		}
	}
	return f;
}
int MOLS::checkortinc(int ia, int ib){
	LS a = Squares[ia];
	LS b = Squares[ib];
	int count = 0;
	int * r = new (int[order*order]);
	for (int k = 0; k<order*order; k++){ r[k] = 0; }
	for (int i = 0; i<order; i++){
		for (int j = 0; j<order; j++){
			int h = a.it(i, j)*order + b.it(i, j);
			r[h]++;
			if (r[h] == 1){ count++; }
		}
	}

	if (count<order*order){
		cout << endl << "Orthogonality Matrix for squares " << ia + 1 << " and " << ib + 1 << endl;
//		report << endl << "Orthogonality Matrix for squares " << ia + 1 << " and " << ib + 1 << endl;
		for (int i = 0; i<order; i++){
			for (int j = 0; j<order; j++){
				cout << r[i*order + j] << " ";
//				report << r[i*order + j] << " ";
			}
			cout << endl;
//			report << endl;
		}
	}
	return count;
}

int MOLS::ortogonalitycheck(){
	int res = 100;
	for (int i = 0; i<count; i++){
		for (int j = i + 1; j<count; j++){
			int r = checkortinc(i, j);
			if (r<order*order){
				cout << "Squares " << i + 1 << " and " << j + 1 << " are orthogonal in " << r << " entries" << endl;
//				report << "Squares " << i + 1 << " and " << j + 1 << " are orthogonal in " << r << " entries" << endl;

			}
			if (res>r){ res = r; }
		}
	}
	return res;
}

int MOLS::checkortinc_markings(int ia, int ib){
	LS a = Squares[ia];
	LS b = Squares[ib];
	int count = 0;
	int * r = new (int[order*order]);
	for (int k = 0; k<order*order; k++){ r[k] = 0; }
	int markings_numberofones = 0;
	for (int i = 0; i<order; i++){
		for (int j = 0; j<order; j++){
			if (markings[i*order + j] == 1){
				markings_numberofones++;
				int h = a.it(i, j)*order + b.it(i, j);
				r[h]++;
				if (r[h] == 1){ count++; }
			}
		}
	}

	//if (count<markings_numberofones){
	cout << endl << "Orthogonality Matrix for squares " << ia + 1 << " and " << ib + 1 << endl;
//	report << endl << "Orthogonality Matrix for squares " << ia + 1 << " and " << ib + 1 << endl;
	for (int i = 0; i<order; i++){
		for (int j = 0; j<order; j++){
			cout << r[i*order + j] << " ";
//			report << r[i*order + j] << " ";
		}
		cout << endl;
//		report << endl;
	}
	//}
	return count;
}
int MOLS::checkortinc_markings_tofile(int ia, int ib, string filename){
	ofstream out;
	out.open(filename.c_str(), ios::app);
	LS a = Squares[ia];
	LS b = Squares[ib];
	int count = 0;
	int * r = new (int[order*order]);
	for (int k = 0; k<order*order; k++){ r[k] = 0; }
	int markings_numberofones = 0;
	for (int i = 0; i<order; i++){
		for (int j = 0; j<order; j++){
			if (markings[i*order + j] == 1){
				markings_numberofones++;
				int h = a.it(i, j)*order + b.it(i, j);
				r[h]++;
				if (r[h] == 1){ count++; }
			}
		}
	}

	//if (count<markings_numberofones){
	cout << endl << "Orthogonality Matrix for squares " << ia + 1 << " and " << ib + 1 << endl;
//	report << endl << "Orthogonality Matrix for squares " << ia + 1 << " and " << ib + 1 << endl;
	for (int i = 0; i<order; i++){
		for (int j = 0; j<order; j++){
			out << r[i*order + j] << " ";
		}
		out << endl;
	}
	//}
	out.close();
	return count;
}
int MOLS::ortogonalitycheck_withmarkings(){
	int res = 100;
	for (int i = 0; i<count; i++){
		for (int j = i + 1; j<count; j++){
			int r = checkortinc_markings(i, j);
			if (r<order*order){
				cout << "Squares " << i + 1 << " and " << j + 1 << " are orthogonal in " << r << " entries (according to markings)" << endl;
//				report << "Squares " << i + 1 << " and " << j + 1 << " are orthogonal in " << r << " entries (according to markings)" << endl;
			}
			if (res>r){ res = r; }
		}
	}
	return res;

}


int MOLS::ortogonalitycheck_withmarkings_tofile(string filename){
	ofstream out;
	out.open(filename.c_str(), ios::app);
	for (int i = 0; i<Squares.size(); i++){
		out << Squares[i].tostring() << endl;
	}
	out << endl << "Markings" << endl;

	for (int i = 0; i<order; i++){
		for (int j = 0; j<order; j++){
			out << markings[i*order + j] << " ";
		}
		out << endl;
	}
	out << endl;
	int res = order*order;
	for (int i = 0; i<count; i++){
		for (int j = i + 1; j<count; j++){
			int r = checkortinc_markings(i, j);
			if (r<order*order){
				out << "Squares " << i + 1 << " and " << j + 1 << " are orthogonal in " << r << " entries (according to markings)" << endl;
			}
			if (res>r){ res = r; }
		}
	}

	out.close();
	return res;

}

int MOLS::check(bool diag){
	bool f = true;
	int r = 0;
	for (int i = 0; i<count; i++){
		LS a = Squares[i];
		bool r = a.check(diag);
		if (r == false){
			f = false;
			cout << "square " << i << " is wrong" << endl;
//			report << "square " << i << " is wrong" << endl;
		}
	}

	if (f == true){
		r = ortogonalitycheck();
		return r;
	}
	else {
		return 0;
	}
}
MOLS::MOLS(string s, int n, int r, bool wspaces){
	int *a = new (int[n*n*n*n*n]);
	int *b = new (int[n*n*r]);
	string t;
	int i1 = 0;
	int i2 = 0;
	int k = 0;
	int g = 0;


	if (wspaces == true){
		for (int i = 0; i<s.size(); i++){
			if (s[i] == ' ')
			{
				i2 = i;
				t = s.substr(i1, i2 - i1);
				//cout <<t<<" ";
				a[k] = strtoi(t);
				k++;
				i1 = i2;
			}
		}
	}
	else {
		for (int i = 0; i<s.length(); i++){
			//a[i]=s[i]-'0';
			if (s[i] == '0'){ a[i] = 0; }
			else{ a[i] = 1; }
		}
	}
	for (int i = 0; i<n*n*n*r; i++){
		if (a[i]>0) {
			b[g] = i%n;
			g++;
		}
	}

	cout << endl;
	order = n;
	count = r;
	for (int k = 0; k<r; k++){
		int *z = new (int[n*n]);
		for (int t = 0; t<n*n; t++){
			z[t] = b[k*n*n + t];
		}
		LS h = LS(z, order);
		Squares.push_back(h);
	}
	cout << endl << "Markings:" << endl;
	for (int i = n*n*n*r; i<n*n*n*r + n*n; i++){
		cout << (a[i]>0) << " ";
		if (a[i]>0){
			markings.push_back(1);
		}
		else{
			markings.push_back(0);
		}
		if ((i + 1) % n == 0) cout << endl;
	}

}

MOLS::MOLS(vector<int> ss, int n, int r){
	int *b = new (int[n*n*r]);
	int g = 0;

	for (int i = 0; i<n*n*n*r; i++){
		if (ss[i]>0) {
			b[g] = i%n;
			g++;
		}
	}
	cout << endl;
	order = n;
	count = r;
	for (int k = 0; k<r; k++){
		int *z = new (int[n*n]);
		for (int t = 0; t<n*n; t++){
			z[t] = b[k*n*n + t];
		}
		LS h = LS(z, order);
		Squares.push_back(h);
	}
	cout << endl << "Markings:" << endl;
	for (int i = n*n*n*r; i<n*n*n*r + n*n; i++){
		cout << (ss[i]>0) << " ";
		if (ss[i]>0){
			markings.push_back(1);
		}
		else{
			markings.push_back(0);
		}
		if ((i + 1) % n == 0) cout << endl;
	}

}
void MOLS::print(const char * fn){
	ofstream myfile(fn);
	if (myfile.is_open()){
		for (int k = 0; k<count; k++){
			myfile << Squares[k].tostring() << '\n';
			myfile << "reordered version:\n";
			Squares[k].reorder();
			myfile << Squares[k].tostring() << '\n';
		}
	}
}
void MOLS::exportLStofile(const char * fn){
	ofstream myfile(fn);
	if (myfile.is_open()){
		for (int i = 0; i<count; i++){
			LS a = Squares[i];
			for (int l = 0; l<order; l++){
				for (int j = 0; j<order; j++){
					int tmp = a.it(l, j);
					myfile << order*order*order*i + order*order*l + order*j + tmp + 1 << " 0" << endl;
				}
			}

		}
	}
	myfile.close();
}



bool eq(LS a, LS b, int order){
	bool res = true;
	for (int i = 0; i<order; i++){
		for (int j = 0; j<order; j++){
			if (a.it(i, j) != b.it(i, j)){ res = false; }
		}
	}
	return res;
}
struct ijk{
	int sq;
	int i;
	int j;
	int k;
	int phase;

};

struct less_than_ijk
{
	inline bool operator() (const ijk& st1, const ijk& st2)
	{
		bool r;
		if (st1.sq<st2.sq){ r = true; }
		else{
			if (st1.sq>st2.sq) { r = false; }
			else {
				if (st1.i<st2.i){ r = true; }
				else if (st1.i>st2.i){ r = false; }
				else {
					if (st1.j<st2.j){ r = true; }
					else if (st1.j>st2.j){ r = false; }
					else{
						if (st1.k<st2.k){ r = true; }
						else if (st1.k >= st2.k){ r = false; }
					}
				}
			}
		}
		return r;
	}
};

void copyfile(const char *from_file, const char * to_file){

	ifstream source(from_file, ios::binary);
	ofstream dest(to_file, ios::binary);
	dest << source.rdbuf();
	source.close();
	dest.close();
}
void localsearch::log(string action_description, MOLS current_MOLS, string logfile){
	ofstream out;
	out.open(logfile.c_str(), ios::app);
	out << action_description << endl << endl;

	out << "Iteration number " << iteration_number << ", Processed points " << processed_points_number << "." << endl;
	out << endl;
	out.close();
	current_MOLS.ortogonalitycheck_withmarkings_tofile(logfile);
	out << endl;
}
void localsearch::log(string action_description, string logfile){
	ofstream out;
	out.open(logfile.c_str(), ios::app);
	out << action_description << endl << endl;
	out.close();
}
void localsearch::log(string action_description, double time, string logfile){
	ofstream out;
	out.open(logfile.c_str(), ios::app);
	out << action_description << " " << time << " seconds. " << endl << endl;
	out.close();
}
vector<vector<int>> localsearch::generate_areas(vector<vector<int>> input, int increment){
	int sign;
	if (increment > 0){ sign = 1; }
	else {
		if (increment < 0) { sign = -1; }
		else { sign = 0; }
	}
	int incval = abs(increment);
	vector<vector<int>> result;
	for (int i = 0; i < input.size(); i++){
		if (incval>1){
			vector<vector<int>> temp;
			vector<int> markings_temp = input[i];
			temp.push_back(markings_temp);
			vector<vector<int>> res_temp;
			res_temp = generate_areas(temp, sign*(incval - 1));
			/*			for (int j = 0; j < res_temp.size(); j++){
			result.push_back(res_temp[j]);
			}*/
			vector<vector<int>> part_result = generate_areas(res_temp, sign);
			for (int j = 0; j < part_result.size(); j++){
				result.push_back(part_result[j]);
			}
		}
		else{
			vector<int> cv = input[i];
			//
			if (sign>0){
				for (int j = 0; j<cv.size(); j++){
					if (cv[j] == 0){
						vector<int> temp;
						for (int l = 0; l < cv.size(); l++){
							if (l == j){
								{ temp.push_back(1); }
							}
							else { temp.push_back(cv[l]); }
						}
						result.push_back(temp);
					}
				}
			}
			else
			{
				for (int j = 0; j<cv.size(); j++){
					if (cv[j] == 1){
						vector<int> temp;
						for (int l = 0; l<cv.size(); l++){
							if (l == j){
								{ temp.push_back(0); }
							}
							else { temp.push_back(cv[l]); }
						}
						result.push_back(temp);
					}
				}
			}
			//
		}

	}

	return result;
}
void localsearch::print_area(vector<vector<int>> area, string filename){
	ofstream out;
	out.open(filename.c_str());
	for (int i = 0; i < area.size(); i++){
		vector<int> tmp = area[i];
		for (int j = 0; j < tmp.size(); j++){
			out << tmp[j];
			if ((j + 1) % order_of_squares == 0) out << endl;
		}
		out << endl;
	}
	out.close();
}
void localsearch::generate_area(vector<int> markings, int increment){
	current_area.clear();
	vector<vector<int>> inp;
	inp.push_back(markings);
	vector<vector<int>>result;
	result = generate_areas(inp, increment);
	current_area = result;
	
}

bool localsearch::get_next_point()
{
	if (current_area.size() == 0){
		if (current_radius == maximum_radius){ return false; }
		else{
			current_radius++;
			log("Generating wider area with radius " + inttostr(current_radius), logfile_name);
			generate_area(record_point.markings, -current_radius);
			string area_file = current_path + "\\area_" + inttostr(iteration_number) + ".area";
			print_area(current_area, area_file);
			current_markings.clear();
			current_markings = current_area.back();
			current_area.pop_back();
			return true;
		}
	}
	current_markings.clear();
	current_markings = current_area.back();
	current_area.pop_back();
	return true;
}
bool localsearch::extract_solution(string filename, int n, int r, int &number, MOLS & m){

	ifstream myfile(filename.c_str());
	bool UNSAT = true;
	while (myfile.good()){
		string line = "";
		getline(myfile, line);
		if (line.size()>5){
			UNSAT = false;
			m = MOLS(line, n, r, true);
			number = m.ortogonalitycheck_withmarkings();
			cout << endl << "Amount of orthogonal pairs with regard to markings is : " << number << endl;
		}
	}
	myfile.close();
	return !UNSAT;
}
void localsearch::initialize(int n, int r, int start_from, string path){
	current_path = path;
	order_of_squares = n;
	number_of_squares = r;
	maximum_radius = 2;
	processed_points_number = 0;
	int cur_rad = 1;
	iteration_number = 0;
	number_of_processed_point = 0;
	cnfinput = init.LSSetinc_markings_experimental(n, r);
	base_cnf = path + "\\base.cnf";
	string init_cnf = path + "\\initialization.cnf";
	string init_cnf_out = path + "\\initialization.out";
	logfile_name = path + "\\log.txt";
	init.Print(base_cnf.c_str());
	copyfile(base_cnf.c_str(), init_cnf.c_str());
	init.Append_order(init_cnf.c_str(), cnfinput, start_from);
	string cmd_string;
	cmd_string = "D:\\Tests\\cryptominisat32.exe " + init_cnf + " " + init_cnf_out;

	double init_time_0 = cpuTime();
	system(cmd_string.c_str());
	double init_time_1 = cpuTime();
	log("Initialization MOLS found", init_time_1 - init_time_0, logfile_name);

	MOLS init_mols;
	int init_num;
	if (extract_solution(init_cnf_out, n, r, init_num, init_mols) == true){
		cout << "Initialization MOLS found" << endl;
	}
	else { exit; }

	iteration_number++;
	number_of_processed_point++;

	log("Initialization MOLS found", init_mols, logfile_name);

	current_record = init_num;
	initial_point = init_mols;
	record_point = init_mols;
	generate_area(init_mols.markings, -cur_rad);
	string area_file = path + "\\area_" + inttostr(iteration_number) + ".area";
	print_area(current_area, area_file);
	get_next_point();

}
void thread_test(string cmdstring){
	system(cmdstring.c_str());
}

void localsearch::search(int n, int r, int start_from, int end_value, string path){

	initialize(n, r, start_from, path);
	string point_cnf = path + "\\point";
	string cmd_string;
	string current_cnf = point_cnf + "_" + inttostr(iteration_number) + "_" + inttostr(processed_points_number) + ".cnf";
	string current_cnf_out = point_cnf + "_" + inttostr(iteration_number) + "_" + inttostr(processed_points_number) + ".out";
	string current_assumptions_file = point_cnf + "_" + inttostr(iteration_number) + "_" + inttostr(processed_points_number) + "assumptions.txt";
	int cur_num = 0;
	while (current_record<end_value){
		processed_points_number++;
		current_cnf = point_cnf + "_" + inttostr(iteration_number) + "_" + inttostr(processed_points_number) + ".cnf";
		current_cnf_out = point_cnf + "_" + inttostr(iteration_number) + "_" + inttostr(processed_points_number) + ".out";
		current_assumptions_file = point_cnf + "_" + inttostr(iteration_number) + "_" + inttostr(processed_points_number) + "assumptions.txt";

		copyfile(base_cnf.c_str(), current_cnf.c_str());
		init.Append_order(current_cnf.c_str(), cnfinput, current_record + 1);

		init.Append_assumptions(current_cnf.c_str(), record_point.incompleteassumption(current_markings));

		string test_current_cnf = point_cnf + "_" + inttostr(iteration_number) + "_" + inttostr(processed_points_number) + "_test.cnf";
		string test_current_cnf_out = point_cnf + "_" + inttostr(iteration_number) + "_" + inttostr(processed_points_number) + "_test.out";
		copyfile(current_cnf.c_str(), test_current_cnf.c_str());


		record_point.print_incomplete_assumption_tofile(current_markings, current_assumptions_file);

		cmd_string = "D:\\Tests\\cryptominisat32.exe " + current_cnf + " " + current_cnf_out;

		double cur_time_0 = cpuTime();
		system(cmd_string.c_str());
		double cur_time_1 = cpuTime();

		MOLS cur_mols;
		bool unsat = !extract_solution(current_cnf_out, n, r, cur_num, cur_mols);
		if (unsat){
			log("UNSAT iteration " + inttostr(iteration_number) + " point " + inttostr(processed_points_number), cur_time_1 - cur_time_0, logfile_name);
			if (get_next_point() == false){
				current_radius++;
				log("Generating wider area with radius " + inttostr(current_radius), logfile_name);
				generate_area(record_point.markings, -current_radius);
				string area_file = path + "\\area_" + inttostr(iteration_number) + ".area";
				print_area(current_area, area_file);
				get_next_point();
			}
		}
		else{
			log("New record MOLS found", cur_time_1 - cur_time_0, logfile_name);
			log("Squares", cur_mols, logfile_name);
			iteration_number++;
			cout << "New record " << cur_num << endl;
			current_record = cur_num;
			record_point = cur_mols;
			current_radius = 1;
			generate_area(cur_mols.markings, -current_radius);
			get_next_point();
		}
	}
}

localsearch_minisat :: localsearch_minisat() :
	solve_count      ( 0 ),
	core_len         ( 0 ),
	unsat_count      ( 0 ), 
	sat_count        ( 0 ),
	min_unsat_time   ( 1 << 20 ), 
	min_sat_time     ( 1 << 20 ),
	max_unsat_time   ( 0 ), 
	max_sat_time     ( 0 ),
	med_unsat_time   ( 0 ), 
	med_sat_time     ( 0 ),
	total_unsat_time ( 0 ), 
	total_sat_time   ( 0 ),
	max_solving_time ( 0 )
{}

void localsearch_minisat :: printSolving()
{
	cout << "sat_count "      << sat_count      << endl;
	cout << "min_sat_time "   << min_sat_time   << endl;
	cout << "max_sat_time "   << max_sat_time   << endl;
	cout << "med_sat_time "   << med_sat_time   << endl;
	cout << "***" << endl;
	cout << "unsat_count "    << unsat_count    << endl;
	cout << "min_unsat_time " << min_unsat_time << endl;
	cout << "max_unsat_time " << max_unsat_time << endl;
	cout << "med_unsat_time " << med_unsat_time << endl;
}

void localsearch::search_multithread(int num_of_threads, int n, int r, int start_from, int end_value, string path){
	initialize(n, r, start_from, path);
	string point_cnf = path + "\\point";
	string cmd_string;
	string current_cnf;
	string current_cnf_out;
	string current_assumptions_file;
	int cur_num = 0;

	while (current_record < end_value){
		vector<string> list_of_cnf_names;
		vector<string> list_of_cnf_outputs;
		vector<string> list_of_assumption_files;
		vector<vector<int>> list_of_markings_processing;
		vector<string> list_of_cmdstrings;
		processed_points_number++;
		list_of_cnf_names.clear();
		list_of_cnf_outputs.clear();

		for (int i = 0; i < num_of_threads; i++){
			string temp_name = point_cnf + "_" + inttostr(iteration_number) + "_" + inttostr(processed_points_number) + "_thread_" + inttostr(i) + ".cnf";
			string temp_out = point_cnf + "_" + inttostr(iteration_number) + "_" + inttostr(processed_points_number) + "_thread_" + inttostr(i) + ".out";
			string temp_assumption = point_cnf + "_" + inttostr(iteration_number) + "_" + inttostr(processed_points_number) + "assumptions.txt";
			processed_points_number++;
			list_of_cnf_names.push_back(temp_name);
			list_of_cnf_outputs.push_back(temp_out);
			list_of_assumption_files.push_back(temp_assumption);
		}
		for (int i = 0; i < num_of_threads; i++){
			list_of_markings_processing.push_back(current_markings);
			get_next_point();
		}
		for (int i = 0; i < num_of_threads; i++){
			copyfile(base_cnf.c_str(), list_of_cnf_names[i].c_str());
			init.Append_order(list_of_cnf_names[i].c_str(), cnfinput, current_record + 1);
			init.Append_assumptions(list_of_cnf_names[i].c_str(), record_point.incompleteassumption(list_of_markings_processing[i]));
			record_point.print_incomplete_assumption_tofile(current_markings, list_of_assumption_files[i].c_str());
		}

		for (int i = 0; i < num_of_threads; i++){
			string tempcmdstring = "D:\\Tests\\cryptominisat32.exe " + list_of_cnf_names[i] + " " + list_of_cnf_outputs[i];
			list_of_cmdstrings.push_back(tempcmdstring);
		}
		vector<thread> threads;
		for (int i = 0; i < num_of_threads; i++){
			threads.push_back(thread(thread_test, list_of_cmdstrings[i]));
		}

		for (int i = 0; i < num_of_threads; i++){
			threads[i].join();
		}
		vector<MOLS> MOLS_results;
		vector<bool> UNSAT;
		vector<int> Records_threads;
		for (int i = 0; i < num_of_threads; i++){
			MOLS a;
			int cur_num = 0;
			bool tmp_unsat = !extract_solution(list_of_cnf_outputs[i].c_str(), n, r, cur_num, a);
			MOLS_results.push_back(a);
			UNSAT.push_back(tmp_unsat);
			Records_threads.push_back(cur_num);
		}
		int index_of_best = 0;
		for (int i = 0; i < num_of_threads; i++){
			if (Records_threads[i]>Records_threads[index_of_best]){ index_of_best = i; }
		}

		for (int i = 0; i < num_of_threads; i++){
			if (UNSAT[i] == true){
				log("UNSAT iteration " + inttostr(iteration_number) + " point " + inttostr(processed_points_number - i), logfile_name);
			}
			else{
				log("New record MOLS found", logfile_name);

				log("Squares", MOLS_results[index_of_best], logfile_name);
				iteration_number++;
				cout << "New record " << cur_num << endl;
				current_record = Records_threads[index_of_best];
				record_point = MOLS_results[index_of_best];
				current_radius = 1;
				generate_area(record_point.markings, -current_radius);
				get_next_point();
			}


		}
	}
}
int tvi(int t, int i, int j, int mod, int n){

	return n*n*(3 * t + mod) + i*n + j + 1;
}

//localsearch minisat


void localsearch_minisat::log(string action_description, MOLS current_MOLS, string logfile){
	ofstream out;
	out.open(logfile.c_str(), ios::app);
	out << action_description << endl << endl;

	out << "Iteration number " << iteration_number << ", Processed points " << processed_points_number << "." << endl;
	out << endl;
	out.close();
	current_MOLS.ortogonalitycheck_withmarkings_tofile(logfile);
	out << endl;
}
void localsearch_minisat::log(string action_description, string logfile){
	ofstream out;
	out.open(logfile.c_str(), ios::app);
	out << action_description << endl << endl;
	out.close();
}
void localsearch_minisat::log(string action_description, double time, string logfile){
	ofstream out;
	out.open(logfile.c_str(), ios::app);
	out << action_description << " " << time << " seconds. " << endl << endl;
	out.close();
}
vector<vector<int>> localsearch_minisat::generate_areas(vector<vector<int>> input, int increment){
	int sign;
	if (increment > 0){ sign = 1; }
	else {
		if (increment < 0) { sign = -1; }
		else { sign = 0; }
	}
	int incval = abs(increment);
	vector<vector<int>> result;
	for (int i = 0; i < input.size(); i++){
		if (incval>1){
			vector<vector<int>> temp;
			vector<int> markings_temp = input[i];
			temp.push_back(markings_temp);
			vector<vector<int>> res_temp;
			res_temp = generate_areas(temp, sign*(incval - 1));
			/*			for (int j = 0; j < res_temp.size(); j++){
			result.push_back(res_temp[j]);
			}*/
			vector<vector<int>> part_result = generate_areas(res_temp, sign);
			for (int j = 0; j < part_result.size(); j++){
				result.push_back(part_result[j]);
			}
		}
		else{
			vector<int> cv = input[i];
			//
			if (sign>0){
				for (int j = 0; j<cv.size(); j++){
					if (cv[j] == 0){
						vector<int> temp;
						for (int l = 0; l < cv.size(); l++){
							if (l == j){
								{ temp.push_back(1); }
							}
							else { temp.push_back(cv[l]); }
						}
						result.push_back(temp);
					}
				}
			}
			else
			{
				for (int j = 0; j<cv.size(); j++){
					if (cv[j] == 1){
						vector<int> temp;
						for (int l = 0; l<cv.size(); l++){
							if (l == j){
								{ temp.push_back(0); }
							}
							else { temp.push_back(cv[l]); }
						}
						result.push_back(temp);
					}
				}
			}
			//
		}

	}

	return result;
}
void localsearch_minisat::print_area(vector<vector<int>> area, string filename){
	ofstream out;
	out.open(filename.c_str());
	for (int i = 0; i < area.size(); i++){
		vector<int> tmp = area[i];
		for (int j = 0; j < tmp.size(); j++){
			out << tmp[j];
			if ((j + 1) % order_of_squares == 0) out << endl;
		}
		out << endl;
	}
	out.close();
}
void localsearch_minisat::generate_area(vector<int> markings, int increment){
	current_area.clear();
	vector<vector<int>> inp;
	inp.push_back(markings);
	vector<vector<int>>result;
	result = generate_areas(inp, increment);
	current_area = result;

}

bool localsearch_minisat::get_next_point()
{
	if (current_area.size() == 0){
		if (current_radius == maximum_radius){ return false; }
		else{
			current_radius++;
			log("Generating wider area with radius " + inttostr(current_radius), logfile_name);
			generate_area(record_point.markings, -current_radius);
			string area_file = current_path + "\\area_" + inttostr(iteration_number) + ".area";
			print_area(current_area, area_file);
			current_markings.clear();
			current_markings = current_area.back();
			current_area.pop_back();
			return true;
		}
	}
	current_markings.clear();
	current_markings = current_area.back();
	current_area.pop_back();
	return true;
}
bool localsearch_minisat::extract_solution(vector<int> ss, int n, int r, int &number, MOLS & m){
		m = MOLS(ss, n, r);
		number = m.ortogonalitycheck_withmarkings();
		cout << endl << "Amount of orthogonal pairs with regard to markings is : " << number << endl;	
		return true;
}

class Minisat_solver_wrapper{
private:
	unsigned core_len;
	vector<int> sat_solution;
	string result;	
	int verb; //Verbosity level (0=silent, 1=some, 2=more).
	bool pre; //"Completely turn on/off any preprocessing.", true;
	bool solve; //"Completely turn on/off solving after preprocessing.", true);
	string dimacs; //"If given, stop after preprocessing and write the result to this file.");
	int cpu_lim;//"Limit on CPU time allowed in seconds.\n", 0, IntRange(0, INT32_MAX));
	int mem_lim;// "Limit on memory usage in megabytes.\n", 0, IntRange(0, INT32_MAX));
	bool strictp;// "Validate DIMACS header during parsing.", false;
	// added
	bool print_learnts;//"print_learnts", "Printing all learnt clauses.", false);
	bool print_decisions; // "print_decisions", "Printing all decisions.", false);
	
public:
	void initialize();
	void initialize(int verbosity, bool preprocessing, bool solving, string dimacs_string, int cpu_limit, int memory_limit, bool strict_parsing, bool print_learnts_flag, bool print_decisions_flag);
	bool solve_this(Minisat::Problem &Base, Minisat::Problem order, Minisat::Problem assumptions, int num_of_vars, unsigned core_len, double max_solving_time, unsigned &solve_count, double &solving_time );
	vector<int> GetSatSolution(){ return sat_solution; }
	
};

void Minisat_solver_wrapper::initialize(){
	verb = 2;
	pre = false;
	solve = true;
	dimacs = "";
	cpu_lim = 0;
	mem_lim = 0;
	strictp = false; 
	print_learnts = false;
	print_decisions = false; 
{ }
}

void Minisat_solver_wrapper::initialize(int verbosity, bool preprocessing, bool solving, string dimacs_string, int cpu_limit, int memory_limit, bool strict_parsing, bool print_learnts_flag, bool print_decisions_flag){
	verb = verbosity;
	pre = preprocessing;
	solve = solving;
	dimacs = dimacs_string;
	cpu_lim = cpu_limit;
	mem_lim = memory_limit;
	strictp = strict_parsing;
	print_learnts = print_learnts_flag;
	print_decisions = print_decisions_flag;
}
/*
IntOption    verb   ("MAIN", "verb",   "Verbosity level (0=silent, 1=some, 2=more).", 1, IntRange(0, 2));
BoolOption   pre    ("MAIN", "pre",    "Completely turn on/off any preprocessing.", true);
BoolOption   solve  ("MAIN", "solve",  "Completely turn on/off solving after preprocessing.", true);
StringOption dimacs ("MAIN", "dimacs", "If given, stop after preprocessing and write the result to this file.");
IntOption    cpu_lim("MAIN", "cpu-lim","Limit on CPU time allowed in seconds.\n", 0, IntRange(0, INT32_MAX));
IntOption    mem_lim("MAIN", "mem-lim","Limit on memory usage in megabytes.\n", 0, IntRange(0, INT32_MAX));
BoolOption   strictp("MAIN", "strict", "Validate DIMACS header during parsing.", false);
// added
BoolOption   print_learnts("MAIN", "print_learnts",    "Printing all learnt clauses.", false);
BoolOption   print_decisions("MAIN", "print_decisions",  "Printing all decisions.", false);

*/

bool Minisat_solver_wrapper::solve_this( Minisat::Problem &Base, Minisat::Problem order, Minisat::Problem assumptions, int num_of_vars, 
										 unsigned core_len, double max_solving_time, unsigned &solve_count, double &solving_time )
{
	//Minisat::SimpSolver S;
	//Minisat::Solver* solver;
	Minisat::Solver S;
	double initial_time = cpuTime();
	S.verbosity = verb;
	S.addProblem_modified(Base, num_of_vars);
	//S.addProblem(Base); // CNF goes to solver
	S.addProblem_modified(order,0);
	if (assumptions.size() != 0)
		S.addProblem_modified(assumptions,0);
	cout << endl;

	if (S.verbosity > 0){
		cout<<"============================[ Problem Statistics ]=============================\n";
		cout << "|                                                                             |\n";
		cout << "|  Number of variables: " << S.nVars() << "|\n";
		cout << "|  Number of clauses: " << S.nClauses()<<"|\n";
	}

	double parsed_time = cpuTime();
	if (S.verbosity > 0)
		cout << "|  Parse time: " << parsed_time - initial_time << "|\n";

	if (!S.okay()){
		cout << "UNSAT" << endl;
		if (S.verbosity > 0){
			cout<<("===============================================================================\n");
			cout<<("Solved by simplification\n");
			S.printStats();
			cout << endl;
		}
		cout << "UNSATISFIABLE" << endl;
	//	exit(20);
		//change from 25_02
	}

	if ( solve_count++ == 0 ) {
		S.core_len       = core_len;
		S.start_activity = 2.67054e-306;
	}
	S.max_solving_time = max_solving_time;

	solving_time = cpuTime();
	bool ret = S.solve();
	solving_time = cpuTime() - solving_time;

	if (S.verbosity > 0){
		cout << "===============================================================================" << endl;
		S.printStats();
		printf("\n");
	}
	
	if ( ret ) {
		cout << "SAT" << endl;
		for (int i = 0; i < S.nVars(); ++i)
			sat_solution.push_back( (S.model[i] == Minisat::l_True) ? 1 : 0 );
	} 
	else cout << "UNSAT" << endl;
	
	if (ret){ return true; }
	else { return false; }
}



void localsearch_minisat::initialize(int n, int r, int start_from, string path){
	current_path = path;
	order_of_squares = n;
	number_of_squares = r;
	maximum_radius = -2;
	processed_points_number = 0;
	int cur_rad = 1;
	iteration_number = 0;
	number_of_processed_point = 0;
	points_history.initialize(maximum_radius);
	cnfinput = init.LSSetinc_markings_experimental(n, r);
	Base = init.convert_to_problem_base();
	number_of_vars_cnf = init.nVars;
	string base_cnf = path + "\\base.cnf";

	logfile_name = path + "\\log.txt";
	
	string init_cnf = path + "\\init.cnf";
	
	init.Print(base_cnf.c_str());
	Minisat::Problem order_app = init.convert_to_problem_vector(init.order_appendix(cnfinput, start_from));
	init.Print(init_cnf.c_str());
	init.Append_order(init_cnf.c_str(),cnfinput,start_from);

	Minisat_solver_wrapper temp;
	temp.initialize();
	Minisat::Problem assumptions;
	assumptions.clear();

	double init_time_0 = cpuTime();
	double solving_time;

	bool init_found = temp.solve_this(Base, order_app, assumptions, number_of_vars_cnf, core_len, max_solving_time, solve_count, solving_time );
	
	double init_time_1 = cpuTime();
	
	if (init_found == true) { log("Initialization MOLS found", init_time_1 - init_time_0, logfile_name); }
	else { log("Initialization MOLS not found", init_time_1 - init_time_0, logfile_name); }

	MOLS init_mols;
	int init_num;
	if (extract_solution(temp.GetSatSolution(), n, r, init_num, init_mols) == true){
		cout << "Initialization MOLS found" << endl;
		points_history.add_area(init_mols,init_num);
	}
	else { exit; }

	iteration_number++;
	number_of_processed_point++;

	log("Initialization MOLS found", init_mols, logfile_name);

	current_record = init_num;
	initial_point = init_mols;
	record_point = init_mols;
	//generate_area(init_mols.markings, -cur_rad);
	//string area_file = path + "\\area_" + inttostr(iteration_number) + ".area";
	//print_area(current_area, area_file);
	//get_next_point();
	bool beta=points_history.getnextpoint(current_markings,record_point);
}


void localsearch_minisat::search(int n, int r, int start_from, int end_value, string path ){

	initialize(n, r, start_from, path);

	string current_assumptions_file;
	
	double solving_time;
	int cur_num = 0;
	
	while (current_record<end_value){
		processed_points_number++;

		string point_cnf = path + "\\point";
		current_assumptions_file = point_cnf + "_" + inttostr(iteration_number) + "_" + inttostr(processed_points_number) + "assumptions.txt";
		
		string base_cnf = path + "\\base.cnf";
		string current_cnf = point_cnf + "_" + inttostr(iteration_number) + "_" + inttostr(processed_points_number) + ".cnf";
		//copyfile(base_cnf.c_str(), current_cnf.c_str());
		//init.Append_order(current_cnf.c_str(), cnfinput, current_record + 1);
		//init.Append_assumptions(current_cnf.c_str(), record_point.incompleteassumption(current_markings));

		Minisat::Problem current_order;
		current_order.clear();
		Minisat::Problem current_assumptions;
		current_assumptions.clear();
		
		current_order = init.convert_to_problem_vector(init.order_appendix(cnfinput, current_record + 1));
		current_assumptions = init.convert_to_problem_vector(record_point.incompleteassumption(current_markings));
		
		//high verbosity, debug only
	//	init.Print(current_cnf.c_str());
	//	init.Append_order(current_cnf.c_str(), cnfinput, current_record + 1);
	//	init.Append_assumptions(current_cnf.c_str(), record_point.incompleteassumption(current_markings));
		
		//~high verbosity, debug only

		//record_point.print_incomplete_assumption_tofile(current_markings, current_assumptions_file);
		Minisat_solver_wrapper temp;
		temp.initialize();
		
		double cur_time_0 = cpuTime();
		bool unsat = !temp.solve_this(Base, current_order, current_assumptions, number_of_vars_cnf, core_len, max_solving_time, solve_count, solving_time );
		//cout << "solve_count " << solve_count << endl;
		double cur_time_1 = cpuTime();

		if (unsat){
			unsat_count++;
			total_unsat_time += solving_time;
			min_unsat_time = ( solving_time < min_unsat_time ) ? solving_time : min_unsat_time;
			max_unsat_time = ( solving_time > max_unsat_time ) ? solving_time:  max_unsat_time;
			med_unsat_time = total_unsat_time / unsat_count;
			printSolving();
			log("UNSAT iteration " + inttostr(iteration_number) + " point " + inttostr(processed_points_number), cur_time_1 - cur_time_0, logfile_name);
			bool beta=points_history.getnextpoint(current_markings,record_point);
			if (beta==false){break;}
		}
		else{
			sat_count++;
			total_sat_time += solving_time;
			min_sat_time = ( solving_time < min_sat_time ) ? solving_time : min_sat_time;
			max_sat_time = ( solving_time > max_sat_time ) ? solving_time : max_sat_time;
			med_sat_time = total_sat_time / sat_count;
			printSolving();
			MOLS cur_mols;
			extract_solution(temp.GetSatSolution(), n, r, cur_num, cur_mols);
			points_history.add_area(cur_mols,cur_num);
			cout << "extract_solution done" << endl;
			log("New record MOLS found", cur_time_1 - cur_time_0, logfile_name);
			log("Squares", cur_mols, logfile_name);
			iteration_number++;
			cout << "New record " << cur_num << endl;
			current_record = cur_num;
			record_point = cur_mols;
			current_radius = 1;
			generate_area(cur_mols.markings, -current_radius);
			bool beta=points_history.getnextpoint(current_markings,record_point);
		}
	}
}
