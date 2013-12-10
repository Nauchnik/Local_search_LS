#include "FPP.h"

vector <int> FPP::Row (int i){
	vector<int> res;
	for (int k=0;k<dimension;k++){
		res.push_back(dimension*i+k+1);
	}
	return res;
}
vector <int> FPP::Column (int j){
	vector<int> res;
	for (int k=0;k<dimension;k++){
		res.push_back(j+dimension*k+1);
	}
	return res;
}
void FPP::Newvareqor(int nv, vector<int>right){
	int len=right.size();
	int *a;
	a=new(int[len+2]);
	a[0]=-nv;
	for (int i=0;i<len;i++){
		a[i+1]=right[i];
	}
	a[len+1]=0;
	M.push_back(a);
	nClauses++;
	for (int i=0;i<len;i++){
		a=new (int[3]);
		a[0]=nv;
		a[1]=-right[i];
		a[2]=0;
		M.push_back(a);
		nClauses++;
	}
}
void FPP::Newvareqand(int nv, vector<int>right){
	int len=right.size();
	int *a;
	a=new(int[len+2]);
	a[0]=nv;
	for (int i=0;i<len;i++){
		a[i+1]=-right[i];
	}
	a[len+1]=0;
	M.push_back(a);
	nClauses++;
	for (int i=0;i<len;i++){
		a=new (int[3]);
		a[0]=-nv;
		a[1]=right[i];
		a[2]=0;
		M.push_back(a);
		nClauses++;
	}
}
vector<int> FPP::vecnewvareqand (const vector<int> r1, const vector<int> r2){
	vector<int> nv;
	if (r1.size()!=r2.size()) exit;
	for (int i=0;i<r1.size();i++){
		nv.push_back(++nVars);
	}	
	for (int i=0;i<nv.size();i++){
		vector<int> tmp;
		tmp.push_back(r1[i]);
		tmp.push_back(r2[i]);
		Newvareqand(nv[i],tmp);
	}
	return nv;
}
void FPP::onlyonesimple(vector<int> t){
	int len=t.size();
	int *lcl;
	lcl=new(int[len+1]);
	for (int i=0;i<len;i++){
		lcl[i]=t[i];
		for (int j=i+1;j<len;j++){
			int *a;
			a=new(int[3]);
			a[0]=-t[i];
			a[1]=-t[j];
			a[2]=0;
			M.push_back(a);
			nClauses++;
		}
	}
	lcl[len]=0;
	M.push_back(lcl);
	nClauses++;
}
void FPP::onlyonesimple_mod(vector<int> t){
	int len=t.size();
	for (int i=0;i<len;i++){
		for (int j=i+1;j<len;j++){
			int *a;
			a=new(int[3]);
			a[0]=-t[i];
			a[1]=-t[j];
			a[2]=0;
			M.push_back(a);
			nClauses++;
		}
	}	
}
void FPP::varfollowsatleastone(vector<int> t,int varid){
	int len=t.size();
	int *lcl;
	lcl=new(int[len+2]);
	for (int i=0;i<len;i++){
		lcl[i]=t[i];		
	}
	lcl[len]=-varid;
	lcl[len+1]=0;
	M.push_back(lcl);
	nClauses++;
}
void FPP::varfollowsonlyonesimple(vector<int> t,int varid){
	int len=t.size();
	int *lcl;
	lcl=new(int[len+2]);
	for (int i=0;i<len;i++){
		lcl[i]=t[i];
		for (int j=i+1;j<len;j++){
			int *a;
			a=new(int[4]);
			a[0]=-t[i];
			a[1]=-t[j];
			a[2]=-varid;
			a[3]=0;
			M.push_back(a);
			nClauses++;
		}
	}
	lcl[len]=-varid;
	lcl[len+1]=0;
	M.push_back(lcl);
	nClauses++;
}
void FPP::onlyonecomplex(vector<int> t){
	int len=t.size();
	vector<int>nv;
	for(int i=0;i<len;i++){
		vector<int>r;
		for (int j=0;j<len;j++){
			if (j!=i) r.push_back(-t[j]);
		}
		nv.push_back(++nVars);
		Newvareqand(nVars,r);//+m+2 дизъюнкта
		r.clear();
	}
	//в итоге m*(m+2) дизъюнктов
	for (int i=0;i<len;i++){
		int * a;
		a=new(int[3]);
		a[0]=-t[i];
		a[1]=nv[i];
		a[2]=0;
		M.push_back(a);
		nClauses++;
	}
}
void FPP::Projectiveplane(int n){
	vector <int> unitlits;
	int m=n*n+n+1;
	nVars=m*m;
	dimension = m;
	int n2=0;
	int m2=0;
	int	k=2;
	while (m>(k/2)){		
		if (((n+1)<=k)&&((n+1)>k/2)) {n2=k;}
		if ((m<=k)&&(m>k/2)) {m2=k;}
		k=k*2;
	}
	vector <int> zerovals;
	if (m<m2){//we need to fill some zeros to the variable list
		for (int i=m+1;i<=m2;i++){
			zerovals.push_back(++nVars);
			unitlits.push_back(-nVars);
		}
	}	

	//cardinality
	//rows
	for (int i=0;i<m;i++){
		vector<int> a = Row(i);
		if (m<m2){
			for (int j=0;j<zerovals.size();j++){
				a.push_back(zerovals[j]);
			}
		}
		vector<int> r=SHSort(a,n2,m2);
		for (int i=0;i<n+1;i++){
			unitlits.push_back(r[i]);
		}
		unitlits.push_back(-r[n+1]);
	}
	
	//columns
	for (int i=0;i<m;i++){
		vector<int> a = Column(i);
		if (m<m2){
			for (int j=0;j<zerovals.size();j++){
				a.push_back(zerovals[j]);
			}
		}
		vector<int> r=SHSort(a,n2,m2);
		for (int i=0;i<n+1;i++){
			unitlits.push_back(r[i]);
		}
		unitlits.push_back(-r[n+1]);
	}
//	Dump("C:\\Lam\\cardinality.cnf");
	//condition of "Only 1 intersection" or the kind of 
	//rows
	for (int r1=0;r1<m;r1++){
		vector<int> t1=Row(r1);
		for (int r2=r1+1;r2<m;r2++){
			vector<int> t2=Row(r2);
			vector<int> tt=vecnewvareqand(t1,t2);
			seqcounter(tt);
		//	onlyonesimple(tt);
	//		onlyonecomplex(tt);
			t2.clear();
			
	//		Dump("C:\\Lam\\onlyonecomplexdump.cnf");
	//		onlyonesimple(tt);
//			Dump("C:\\Lam\\onlyonesimpledump.cnf");
			tt.clear();
		}
		t1.clear();
		cout<<endl<<r1<<" rows processed, nVars = "<<nVars<<" M size "<< M.size()<< " Max size " <<M.max_size();
	}
	
	//columns
		for (int c1=0;c1<m;c1++){
		for (int c2=c1+1;c2<m;c2++){
			vector<int> t1=Column(c1);
			vector<int> t2=Column(c2);
			vector<int> tt=vecnewvareqand(t1,t2);
			//onlyonesimple(tt);
		//	onlyonecomplex(tt);
		//	Dump("C:\\Lam\\test10.cnf");
			seqcounter(tt);
		}
		cout<<endl<<c1<<" columns processed, nVars = "<<nVars<<" M size "<< M.size()<< " Max size " <<M.max_size();
	}
	//Dump("C:\\Lam\\test10.cnf");

	for (int i=0;i<unitlits.size();i++){
		int *t;
		t=new (int[2]);
		t[0]=unitlits[i];
		t[1]=0;
		M.push_back(t);
	}
	//Dump("C:\\Lam\\test10.cnf");

}
void FPP::LSset (int n, int r){
	nVars=n*n*n*r;
	for (int t=0;t<r;t++){
		//squares first
		//elements
		for (int i=0;i<n;i++){
			for (int j=0;j<n;j++){
				vector <int> e;
				for (int k=0;k<n;k++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				onlyonesimple(e);
			}
		}
		//columns
		for (int k=0;k<n;k++){
			for (int i=0;i<n;i++){
				vector <int> e;
				for (int j=0;j<n;j++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				onlyonesimple(e);
			}
		}
		//rows
		for (int k=0;k<n;k++){
			for (int j=0;j<n;j++){
				vector <int> e;
				for (int i=0;i<n;i++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				onlyonesimple(e);
			}
		}
	}
	//orthogonality
	for (int t1=0;t1<r;t1++){
		for (int t2=t1+1;t2<r;t2++){
				for(int k1=0;k1<n;k1++){
					for (int k2=0;k2<n;k2++){
						vector<int> lf;
						vector<int> rf;
						for (int i=0;i<n;i++){
							for(int j=0;j<n;j++){
										lf.push_back(LSvar(t1,i,j,k1,n));
										rf.push_back(LSvar(t2,i,j,k2,n));
							}
						}
						vector<int> re=vecnewvareqand(lf,rf);
						onlyonesimple(re);
					}
				}
			}	
		}
	
}
void FPP::LSSetincomplete (int n, int h){
	nVars=n*n*n*3;
	for (int t=0;t<3;t++){
		//squares first
		//elements
		for (int i=0;i<n;i++){
			for (int j=0;j<n;j++){
				vector <int> e;
				for (int k=0;k<n;k++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				//seqcounter(e);
				onlyonesimple(e);
			}
		}
		//columns
		for (int k=0;k<n;k++){
			for (int i=0;i<n;i++){
				vector <int> e;
				for (int j=0;j<n;j++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				//seqcounter(e);
				onlyonesimple(e);
			}
		}
		//rows
		for (int k=0;k<n;k++){
			for (int j=0;j<n;j++){
				vector <int> e;
				for (int i=0;i<n;i++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				//seqcounter(e);
				onlyonesimple(e);
			}
		}
	}
	//orthogonality 
	// normal for 1-2 and 1-3 and incomplete for 2-3
		
	for (int t2=1;t2<3;t2++){
			for(int k1=0;k1<n;k1++){
				for (int k2=0;k2<n;k2++){
					vector<int> lf;
					vector<int> rf;
					for (int i=0;i<n;i++){
						for(int j=0;j<n;j++){
									lf.push_back(LSvar(0,i,j,k1,n));
									rf.push_back(LSvar(t2,i,j,k2,n));
						}
					}
					vector<int> re=vecnewvareqand(lf,rf);
					//seqcounter(re);
					onlyonesimple(re);
				}
			}
		}	
	
	// now incomplete one
	// here we need exactly h pairs and not more, so... 
	// okay let me do it like i feel it will be proper
	if (h>0){
		int hcounter;
		hcounter=0;
		for(int k1=0;k1<n;k1++){
			for (int k2=0;k2<n;k2++){
				vector<int> lf;
				vector<int> rf;
				for (int i=0;i<n;i++){
					for(int j=0;j<n;j++){
								lf.push_back(LSvar(1,i,j,k1,n));
								rf.push_back(LSvar(2,i,j,k2,n));
					}
				}
				hcounter++;
				vector<int> re=vecnewvareqand(lf,rf);
				//seqcounter(re);
				onlyonesimple(re);
				if (hcounter>=h) break;
			}
			if (hcounter>=h) break;
		}
	}
}
void FPP::LSSetincomplete_improved (int n, int h){
	nVars=n*n*n*3;
	for (int t=0;t<3;t++){
		//squares first
		//elements
		for (int i=0;i<n;i++){
			for (int j=0;j<n;j++){
				vector <int> e;
				for (int k=0;k<n;k++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				//seqcounter(e);
				onlyonesimple(e);
			}
		}
		//columns
		for (int k=0;k<n;k++){
			for (int i=0;i<n;i++){
				vector <int> e;
				for (int j=0;j<n;j++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				//seqcounter(e);
				onlyonesimple(e);
			}
		}
		//rows
		for (int k=0;k<n;k++){
			for (int j=0;j<n;j++){
				vector <int> e;
				for (int i=0;i<n;i++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				//seqcounter(e);
				onlyonesimple(e);
			}
		}
	}
	//orthogonality 
	// normal for 1-2 and 1-3 and incomplete for 2-3
		
	for (int t2=1;t2<3;t2++){
			for(int k1=0;k1<n;k1++){
				for (int k2=0;k2<n;k2++){
					vector<int> lf;
					vector<int> rf;
					for (int i=0;i<n;i++){
						for(int j=0;j<n;j++){
									lf.push_back(LSvar(0,i,j,k1,n));
									rf.push_back(LSvar(t2,i,j,k2,n));
						}
					}
					vector<int> re=vecnewvareqand(lf,rf);
					//seqcounter(re);
					onlyonesimple(re);
				}
			}
		}	
	
	// now incomplete one
	// here we need exactly h pairs and not more, so... 
	// okay let me do it like i feel it will be proper
	vector<int> countervars;
	cout<<endl<<"Countervars :"<<endl;
	for (int i=0;i<n*n;i++){
		countervars.push_back(++nVars);
		cout<<nVars<<" ";
	}
	int hcnt=0;
	for(int k1=0;k1<n;k1++){
			for (int k2=0;k2<n;k2++){
				vector<int> lf;
				vector<int> rf;
				for (int i=0;i<n;i++){
					for(int j=0;j<n;j++){
								lf.push_back(LSvar(1,i,j,k1,n));
								rf.push_back(LSvar(2,i,j,k2,n));
					}
				}				
				vector<int> re=vecnewvareqand(lf,rf);
				//seqcounter(re);
			//	varfollowsonlyonesimple(re,countervars[hcnt]);
				varfollowsatleastone(re,countervars[hcnt]);
				hcnt++;				
			}			
		}
	int *tmp;
	
	for (int i=n*n;i<twoceil(n*n);i++){
		tmp=new(int[2]);
		tmp[0]=-(++nVars);
		tmp[1]=0;
		M.push_back(tmp);
		nClauses++;
		countervars.push_back(-tmp[0]);
	}

	cout<<endl<<"Countervars :"<<endl;
	for (int i=0;i<countervars.size();i++){
		cout<<countervars[i]<<" ";
	}
	cout<<endl<<"Sorted:"<<endl;
	vector<int> sorted=HSort(countervars,twoceil(n*n));
	for (int l=0;l<sorted.size();l++){
		cout<<sorted[l]<<" ";
	}
	tmp=new(int[2]);
	if (h>0){
		tmp[0]=sorted[h];
		tmp[1]=0;
		M.push_back(tmp);
		nClauses++;
	}
	tmp=new(int[n*n+1]);
	for (int i=0;i<n*n;i++){
		tmp[i]=sorted[i];
	}
	tmp[n*n]=0;
	M.push_back(tmp);
	nClauses++;
	//initialize first row
	

}

void FPP::LSSetincomplete_improved_all (int n, int h){
	nVars=n*n*n*3;
	for (int t=0;t<3;t++){
		//squares first
		//elements
		for (int i=0;i<n;i++){
			for (int j=0;j<n;j++){
				vector <int> e;
				for (int k=0;k<n;k++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				//seqcounter(e);
				onlyonesimple(e);
			}
		}
		//columns
		for (int k=0;k<n;k++){
			for (int i=0;i<n;i++){
				vector <int> e;
				for (int j=0;j<n;j++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				//seqcounter(e);
				onlyonesimple(e);
			}
		}
		//rows
		for (int k=0;k<n;k++){
			for (int j=0;j<n;j++){
				vector <int> e;
				for (int i=0;i<n;i++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				//seqcounter(e);
				onlyonesimple(e);
			}
		}
	}
	//orthogonality 
	// normal for 1-2 and 1-3 and incomplete for 2-3

	vector<int> countervars;
	cout<<endl<<"Countervars :"<<endl;
	for (int i=0;i<n*n;i++){
		countervars.push_back(++nVars);
		cout<<nVars<<" ";
	}
	

	for (int t1=0;t1<3;t1++){		
		for (int t2=t1+1;t2<3;t2++){
			int hcnt=0;
			for(int k1=0;k1<n;k1++){
				for (int k2=0;k2<n;k2++){
					vector<int> lf;
					vector<int> rf;
					for (int i=0;i<n;i++){
						for(int j=0;j<n;j++){
									lf.push_back(LSvar(t1,i,j,k1,n));
									rf.push_back(LSvar(t2,i,j,k2,n));
						}
					}
					vector<int> re=vecnewvareqand(lf,rf);
					//seqcounter(re);
					//varfollowsonlyonesimple(re,countervars[hcnt]);
					varfollowsatleastone(re,countervars[hcnt]);
					hcnt++;				
				}
			}
		}	
	}
		
	int *tmp;
	tmp=new(int[2]);
	tmp[0]=-(++nVars);
	tmp[1]=0;
	M.push_back(tmp);
	nClauses++;
	for (int i=n*n;i<twoceil(n*n);i++){
		countervars.push_back(-tmp[0]);
	}
	//cout<<endl<<"Sorted:"<<endl;
	vector<int> sorted=HSort(countervars,twoceil(n*n));
	for (int l=0;l<sorted.size();l++){
		cout<<sorted[l]<<" ";
	}
	tmp=new(int[2]);
	if (h>0){
		tmp[0]=sorted[h];
		tmp[1]=0;
		M.push_back(tmp);
		nClauses++;
	}
	tmp=new(int[n*n+1]);
	for (int i=0;i<n*n;i++){
		tmp[i]=sorted[i];
	}
	tmp[n*n]=0;
	M.push_back(tmp);
	nClauses++;
	//initialize first row
	
}
vector<int> FPP::LSSetincomplete_improved_all_general (int n){
	nVars=n*n*n*3;
	for (int t=0;t<3;t++){
		//squares first
		//elements
		for (int i=0;i<n;i++){
			for (int j=0;j<n;j++){
				vector <int> e;
				for (int k=0;k<n;k++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				//seqcounter(e);
				onlyonesimple(e);
			}
		}
		//columns
		for (int k=0;k<n;k++){
			for (int i=0;i<n;i++){
				vector <int> e;
				for (int j=0;j<n;j++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				//seqcounter(e);
				onlyonesimple(e);
			}
		}
		//rows
		for (int k=0;k<n;k++){
			for (int j=0;j<n;j++){
				vector <int> e;
				for (int i=0;i<n;i++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				//seqcounter(e);
				onlyonesimple(e);
			}
		}
	}
	//orthogonality 
	// normal for 1-2 and 1-3 and incomplete for 2-3

	vector<int> countervars;
	cout<<endl<<"Countervars :"<<endl;
	for (int i=0;i<n*n;i++){
		countervars.push_back(++nVars);
		cout<<nVars<<" ";
	}
	

	for (int t1=0;t1<3;t1++){		
		for (int t2=t1+1;t2<3;t2++){
			int hcnt=0;
			for(int k1=0;k1<n;k1++){
				for (int k2=0;k2<n;k2++){
					vector<int> lf;
					vector<int> rf;
					for (int i=0;i<n;i++){
						for(int j=0;j<n;j++){
									lf.push_back(LSvar(t1,i,j,k1,n));
									rf.push_back(LSvar(t2,i,j,k2,n));
						}
					}
					vector<int> re=vecnewvareqand(lf,rf);
					//seqcounter(re);
					//varfollowsonlyonesimple(re,countervars[hcnt]);
					varfollowsatleastone(re,countervars[hcnt]);
					hcnt++;				
				}
			}
		}	
	}
		
	int *tmp;
	tmp=new(int[2]);
	tmp[0]=-(++nVars);
	tmp[1]=0;
	M.push_back(tmp);
	nClauses++;
	for (int i=n*n;i<twoceil(n*n);i++){
		countervars.push_back(-tmp[0]);
	}
	//cout<<endl<<"Sorted:"<<endl;
	vector<int> sorted=HSort(countervars,twoceil(n*n));
	for (int l=0;l<sorted.size();l++){
		cout<<sorted[l]<<" ";
	}
	tmp=new(int[n*n+1]);
	for (int i=0;i<n*n;i++){
		tmp[i]=sorted[i];
	}
	tmp[n*n]=0;
	M.push_back(tmp);
	nClauses++;
	return sorted;
}

void FPP::LSSetincomplete_test (int n, int h){
	nVars=n*n;
	vector<int> countervars;
	cout<<endl<<"Countervars :"<<endl;
	for (int i=0;i<n;i++){
		countervars.push_back(++nVars);
		cout<<nVars<<" ";
	}
	int hcnt=0;
	for (int i=0;i<n;i++){
		vector<int> oo_temp;
		for (int j=0;j<n;j++){
			oo_temp.push_back(i*n+j+1);
		}
		varfollowsonlyonesimple(oo_temp,countervars[i]);
	}
	int *tmp;
	for (int i=n;i<twoceil(n);i++){
		tmp=new(int[2]);
		tmp[0]=-(++nVars);
		tmp[1]=0;
		M.push_back(tmp);
		nClauses++;
		countervars.push_back(-tmp[0]);
	}
	cout<<endl<<"Sorted:"<<endl;
	vector<int> sorted=HSort(countervars,twoceil(n));
	for (int l=0;l<sorted.size();l++){
		cout<<sorted[l]<<" ";
	}
	tmp=new(int[2]);
	if (h>0){
		tmp[0]=sorted[h];
		tmp[1]=0;
		M.push_back(tmp);
		nClauses++;
	}
	tmp=new(int[n+1]);
	for (int i=0;i<n;i++){
		tmp[i]=sorted[i];
	}
	tmp[n]=0;
	M.push_back(tmp);
	nClauses++;
	//initialize first row
	
}
void FPP::Print(const char *fn){
	ofstream out;
	out.open(fn);
	out<<"p cnf "<<nVars<<" "<<nClauses<< endl;
	for (int i=0;i<M.size();i++){
		int *t=M[i];
		int k=0;
		while (t[k]!=0){
			out<<t[k]<<" ";
			k++;
		}
		out<<"0\n";
	}
	cout<<endl<<"Output finished"<<endl;
	out.close();
}
void FPP::Dump(char *fn){
	ofstream out;
	out.open(fn,ios::app);
	for (int i=0;i<M.size();i++){
		int *t=M[i];
		int k=0;
		while (t[k]!=0){
			out<<t[k]<<" ";
			k++;
		}
		out<<"0\n";
	}
	M.clear();
	cout<<endl<<"Dump finished"<<endl;
	out.close();
}
void FPP::seqcounter( vector<int> a)
{
	int lenk=a.size();
	//nVars=lenk;
	vector<int> s;
	int *lcl;
	lcl=new(int [lenk+1]);
	for (int i=0;i<lenk-1;i++){
		lcl[i]=a[i];
		s.push_back(++nVars);
	}
	lcl[lenk-1]=a[lenk-1];
	lcl[lenk]=0;
	M.push_back(lcl);
	nClauses++;
	int * t;
	t=new(int[3]);
	t[0]=-a[1-1];
	t[1]=s[1-1];
	t[2]=0;
	M.push_back(t);
	nClauses++;
	t=new(int[3]);
	t[0]=-a[lenk-1];
	t[1]=-s[lenk-1-1];
	t[2]=0;
	M.push_back(t);
	nClauses++;
	for (int i=2;i<lenk;i++){
		t=new(int[3]);
		t[0]=-a[i-1];
		t[1]=s[i-1];
		t[2]=0;
		M.push_back(t);
		nClauses++;
		t=new(int[3]);
		t[0]=-s[i-1-1];
		t[1]=s[i-1];
		t[2]=0;
		M.push_back(t);
		nClauses++;
		t=new(int[3]);
		t[0]=-a[i-1];
		t[1]=-s[i-1-1];
		t[2]=0;
		M.push_back(t);
		nClauses++;
	}
}
vector <int> FPP::Selectodd(vector<int> a){
	vector <int> res;
	for (int i=0;i<a.size();i=i+2){
		int t=a[i];
		res.push_back(t);
	}
	return res;
}
vector <int> FPP::Selecteven(vector<int> a){
	vector <int> res;
	for (int i=1;i<a.size();i=i+2){
		int t=a[i];
		res.push_back(t);
	}
	return res;
}
vector <int> FPP::HMerge(vector<int> a, vector<int>b, int n){
	vector<int> res;
	for (int i=0;i<2*n;i++){int t=++nVars;res.push_back(t);}
	if (n==1){
		int *cl1=new(int[4]);
		int *cl2=new(int[3]);
		int *cl3=new(int[3]);
		int *cl4=new(int[4]);
		int *cl5=new(int[3]);
		int *cl6=new(int[3]);
		
		cl1[0]=-a[0]; cl1[1]=-b[0]; cl1[2]=res[1]; cl1[3]=0;
		cl2[0]=-a[0]; cl2[1]=res[0]; cl2[2]=0;
		cl3[0]=-b[0]; cl3[1]=res[0]; cl3[2]=0;

		cl4[0]=a[0]; cl4[1]=b[0]; cl4[2]=-res[0]; cl4[3]=0;
		cl5[0]=a[0]; cl5[1]=-res[1]; cl5[2]=0;
		cl6[0]=b[0]; cl6[1]=-res[1]; cl6[2]=0;
		M.push_back(cl1);
		M.push_back(cl2);
		M.push_back(cl3);
		M.push_back(cl4);
		M.push_back(cl5);
		M.push_back(cl6);
		nClauses+=6;
	}
	else
	{
		vector<int> aeven=Selecteven(a);
		vector<int> aodd=Selectodd(a);
		vector<int> beven=Selecteven(b);
		vector<int> bodd=Selectodd(b);
		
		vector<int> D=HMerge(aodd,bodd,aodd.size());
		vector<int> E=HMerge(aeven,beven,aeven.size());
		

		for (int i=1;i<n;i++){
			int *cl1=new(int[4]);
			int *cl2=new(int[3]);
			int *cl3=new(int[3]);
			int *cl4=new(int[4]);
			int *cl5=new(int[3]);
			int *cl6=new(int[3]);
			res[0]=D[0];
			res[2*n-1]=E[n-1];
			cl1[0]=-D[i+1-1]; cl1[1]=-E[i-1]; cl1[2]=res[2*i+1-1]; cl1[3]=0;
			cl2[0]=-D[i+1-1]; cl2[1]=res[2*i-1]; cl2[2]=0;
			cl3[0]=-E[i-1];cl3[1]=res[2*i-1]; cl3[2]=0;

			cl4[0]=D[i+1-1];cl4[1]=E[i-1]; cl4[2]=-res[2*i-1]; cl4[3]=0;
			cl5[0]=D[i+1-1];cl5[1]=-res[2*i+1-1]; cl5[2]=0;
			cl6[0]=E[i-1]; cl6[1]=-res[2*i+1-1]; cl6[2]=0;
			M.push_back(cl1);
			M.push_back(cl2);
			M.push_back(cl3);
			M.push_back(cl4);
			M.push_back(cl5);
			M.push_back(cl6);
			nClauses+=6;
		}			

	}

	return res;
}
vector <int> FPP::SMerge (vector<int> a, vector <int> b, int n){
	vector<int> res;
	if (n==1){ int t=++nVars; res.push_back(t);
			   int r=++nVars; res.push_back(r);}
	else {
		res.push_back(0);
		for (int i=2;i<=n+1;i++){int t=++nVars;res.push_back(t);}
		}	
	if (n==1){
		int *cl1=new(int[4]);
		int *cl2=new(int[3]);
		int *cl3=new(int[3]);
		int *cl4=new(int[4]);
		int *cl5=new(int[3]);
		int *cl6=new(int[3]);
		
		cl1[0]=-a[0]; cl1[1]=-b[0]; cl1[2]=res[1]; cl1[3]=0;
		cl2[0]=-a[0]; cl2[1]=res[0]; cl2[2]=0;
		cl3[0]=-b[0]; cl3[1]=res[0]; cl3[2]=0;

		cl4[0]=a[0]; cl4[1]=b[0]; cl4[2]=-res[0]; cl4[3]=0;
		cl5[0]=a[0]; cl5[1]=-res[1]; cl5[2]=0;
		cl6[0]=b[0]; cl6[1]=-res[1]; cl6[2]=0;
		M.push_back(cl1);
		M.push_back(cl2);
		M.push_back(cl3);
		M.push_back(cl4);
		M.push_back(cl5);
		M.push_back(cl6);
		nClauses+=6;
	}
	else
	{
		vector<int> aeven=Selecteven(a);
		vector<int> aodd=Selectodd(a);
		vector<int> beven=Selecteven(b);
		vector<int> bodd=Selectodd(b);
		
		vector<int> D=SMerge(aodd,bodd,aodd.size());
		vector<int> E=SMerge(aeven,beven,aeven.size());
		res[0]=D[0];
		for (int i=1;i<=n/2;i++){
			int *cl1=new(int[4]);
			int *cl2=new(int[3]);
			int *cl3=new(int[3]);
			int *cl4=new(int[4]);
			int *cl5=new(int[3]);
			int *cl6=new(int[3]);
			

			cl1[0]=-D[i+1-1]; cl1[1]=-E[i-1]; cl1[2]=res[2*i+1-1]; cl1[3]=0;
			cl2[0]=-D[i+1-1]; cl2[1]=res[2*i-1]; cl2[2]=0;
			cl3[0]=-E[i-1];cl3[1]=res[2*i-1]; cl3[2]=0;

			cl4[0]=D[i+1-1];cl4[1]=E[i-1]; cl4[2]=-res[2*i-1]; cl4[3]=0;
			cl5[0]=D[i+1-1];cl5[1]=-res[2*i+1-1]; cl5[2]=0;
			cl6[0]=E[i-1]; cl6[1]=-res[2*i+1-1]; cl6[2]=0;
			M.push_back(cl1);
			M.push_back(cl2);
			M.push_back(cl3);
			M.push_back(cl4);
			M.push_back(cl5);
			M.push_back(cl6);
			nClauses+=6;
		}			

	}

	return res;

}
vector<int> FPP::HSort(vector<int> a, int n){
	vector<int>res;
	if (n/2>1){
		vector<int>l;
		vector<int>r;
		for (int i=0;i<n/2;i++){
			l.push_back(a[i]);
			r.push_back(a[n/2+i]);
		}
		vector<int> lsort=HSort(l,n/2);
		vector<int> rsort=HSort(r,n/2);
		res=HMerge(lsort,rsort,n/2);

	}
	else{
		vector <int> l;
		vector <int> r;
		l.push_back(a[0]);
		r.push_back(a[1]);
		res=HMerge(l,r,1);
	}
	return res;
}
vector<int> FPP::SHSort(vector<int> a, int lown, int n){
	vector<int>res;
	vector<vector<int>> ntuples;
	for (int i=0;i<a.size()/lown;i++){
		vector<int> t;
		for (int j=0;j<lown;j++){
			t.push_back(a[i*lown+j]);
		}
		ntuples.push_back(t);
	}
	vector<int> leftres;
	for (int k=1;k<ntuples.size();k++){
		if (k==1){
			vector<int> sortedleft=HSort(ntuples[0],lown);
			vector<int> sortedright=HSort(ntuples[1],lown);
			leftres=SMerge(sortedleft,sortedright,lown);
		}
		else
		{
			vector<int> rightres;
			vector<int> sortedright=HSort(ntuples[k],lown);
			leftres.erase(leftres.end()-1);
			rightres=SMerge(leftres,sortedright,lown);
			leftres=rightres;
		}
	}
	return leftres;
}
vector<int> FPP::order_appendix(vector<int> h, int t){
	vector<int> result;
	int tmp = h[t];
	result.push_back(tmp);
	return result;	
}
void FPP::Append_order(const char * fn, vector<int> h, int t)
{
	ofstream out;
	out.open(fn,ios::app);
	int tmp=h[t];
	out<<tmp<< " 0"<<endl;
	out.close();
}

void FPP::Append_assumptions (const char *fn, vector<int> h)
{
	ofstream out;
	out.open(fn,ios::app);
	for (int i=0;i<h.size();i++){
		out<<h[i]<< " 0"<<endl;
	}
	out.close();
}
vector<int> FPP::LSSetinc_markings(int n, int lim){

	nVars=n*n*n*3;
	for (int t=0;t<3;t++){
		//squares first
		//elements
		for (int i=0;i<n;i++){
			for (int j=0;j<n;j++){
				vector <int> e;
				for (int k=0;k<n;k++){
					e.push_back(LSvar(t,i,j,k,n));
				}			
				onlyonesimple(e);
			}
		}
		//columns
		for (int k=0;k<n;k++){
			for (int i=0;i<n;i++){
				vector <int> e;
				for (int j=0;j<n;j++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				//seqcounter(e);
				onlyonesimple(e);
			}
		}
		//rows
		for (int k=0;k<n;k++){
			for (int j=0;j<n;j++){
				vector <int> e;
				for (int i=0;i<n;i++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				//seqcounter(e);
				onlyonesimple(e);
			}
		}
	}
	//orthogonality 
	// normal for 1-2 and 1-3 and incomplete for 2-3

	vector<vector<int>> сvars; 
	cout<<endl<<"Markings vector:"<<endl;
	vector<int> markings;
	for (int i=0;i<n*n;i++){
			markings.push_back(++nVars);
			cout<<nVars<<" ";
		}	
	cout <<endl;
	
	for (int t1=0;t1<3;t1++){		
		for (int t2=t1+1;t2<3;t2++){
			int hcnt=0;
			vector<vector<int>> tempvars; 
			
			vector<int> countervars;
			cout<<endl<<"Countervars for squares "<<t1<<" and "<<t2<<":"<<endl;
			for (int i=0;i<n*n;i++){
				countervars.push_back(++nVars);
				cout<<nVars<<" ";
			}	
			сvars.push_back(countervars);
			//пусть o=tempvars[h] - вектор
			// o содержит номера переменных, соответствующие паре k1,k2: k1*n+k2=h; 
			// o[v] - номер переменной, которая=1 если пара k1,k2 стоит в позиции (i,j):i*n+j=v
			// дальше мы вводим новый вектор переменных маркеров
			// переменная в h позиции означает что в (i,j):i*n+j=h стоит наша пара
			// как мы кодируем соответствующее условие
			for(int k1=0;k1<n;k1++){
				for (int k2=0;k2<n;k2++){
					vector<int> lf;
					vector<int> rf;
					for (int i=0;i<n;i++){
						for(int j=0;j<n;j++){
									lf.push_back(LSvar(t1,i,j,k1,n));
									rf.push_back(LSvar(t2,i,j,k2,n));
						}
					}
					vector<int> re=vecnewvareqand(lf,rf);
					tempvars.push_back(re);
					//seqcounter(re);
					varfollowsatleastone(re,countervars[hcnt]);
					hcnt++;				
				}
			}

			for (int i=0;i<n*n;i++){
				vector<int> lf_t;
				for (int j=0;j<n*n;j++){
					lf_t.push_back(tempvars[i][j]);
				}
				vector<int> re_t=vecnewvareqand(countervars,lf_t);
				varfollowsatleastone(re_t,markings[i]);
			}

			int *tmp;
			tmp=new(int[2]);
			tmp[0]=-(++nVars);
			tmp[1]=0;
			M.push_back(tmp);
			nClauses++;	
			for (int i=n*n;i<twoceil(n*n);i++){
				countervars.push_back(-tmp[0]);
			}
			//cout<<endl<<"Sorted:"<<endl;
			vector<int> sorted=HSort(countervars,twoceil(n*n));
			for (int l=0;l<sorted.size();l++){
				cout<<sorted[l]<<" ";
			}
			tmp=new(int[n*n+1]);
			for (int i=0;i<n*n;i++){
				tmp[i]=sorted[i];
			}
			tmp[n*n]=0;
			M.push_back(tmp);
			nClauses++;
			tmp=new(int[2]);
			tmp[0]=sorted[lim];
			tmp[1]=0;
			M.push_back(tmp);
			nClauses++;
		}	
	}
	int *tmp;
	tmp=new(int[2]);
	tmp[0]=-(++nVars);
	tmp[1]=0;
	M.push_back(tmp);
	nClauses++;	
	for (int i=n*n;i<twoceil(n*n);i++){
		markings.push_back(-tmp[0]);
	}
	//cout<<endl<<"Sorted:"<<endl;
	vector<int> sorted=HSort(markings,twoceil(n*n));
	for (int l=0;l<sorted.size();l++){
		cout<<sorted[l]<<" ";
	}
	tmp=new(int[n*n+1]);
	for (int i=0;i<n*n;i++){
		tmp[i]=sorted[i];
	}
	tmp[n*n]=0;
	M.push_back(tmp);
	nClauses++;
	tmp=new(int[2]);
	tmp[0]=sorted[lim];
	tmp[1]=0;
	M.push_back(tmp);
	nClauses++;		
	return sorted;
}
vector<int> FPP::LSSetinc_markings_experimental(int n, int r ){

	vector<int>markings;
	nVars=n*n*n*r;
	for (int t=0;t<r;t++){
		//squares first
		//elements
		for (int i=0;i<n;i++){
			for (int j=0;j<n;j++){
				vector <int> e;
				for (int k=0;k<n;k++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				onlyonesimple(e);
			}
		}
		//columns
		for (int k=0;k<n;k++){
			for (int i=0;i<n;i++){
				vector <int> e;
				for (int j=0;j<n;j++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				onlyonesimple(e);
			}
		}
		//rows
		for (int k=0;k<n;k++){
			for (int j=0;j<n;j++){
				vector <int> e;
				for (int i=0;i<n;i++){
					e.push_back(LSvar(t,i,j,k,n));
				}
				onlyonesimple(e);
			}
		}
	}
	//orthogonality

	for (int i=0;i<n*n;i++){
		markings.push_back(++nVars);
	}
	for (int t1=0;t1<3;t1++){
		for (int t2=t1+1;t2<r;t2++){
				for(int k1=0;k1<n;k1++){
					for (int k2=0;k2<n;k2++){
						vector<int> lf;
						vector<int> rf;
						for (int i=0;i<n;i++){
							for(int j=0;j<n;j++){
										lf.push_back(LSvar(t1,i,j,k1,n));
										rf.push_back(LSvar(t2,i,j,k2,n));
							}
						}
						vector<int> re=vecnewvareqand(lf,rf);
						vector<int> re2=vecnewvareqand(re,markings);
						onlyonesimple_mod(re2);
					}
				}
			}	
		}
	int *tmp;
	tmp=new(int[2]);
	tmp[0]=-(++nVars);
	tmp[1]=0;
	M.push_back(tmp);
	nClauses++;	
	for (int i=n*n;i<twoceil(n*n);i++){
		markings.push_back(-tmp[0]);
	}
	//cout<<endl<<"Sorted:"<<endl;
	vector<int> sorted=HSort(markings,twoceil(n*n));
	for (int l=0;l<sorted.size();l++){
		cout<<sorted[l]<<" ";
	}
	tmp=new(int[n*n+1]);
	for (int i=0;i<n*n;i++){
		tmp[i]=sorted[i];
	}
	tmp[n*n]=0;
	M.push_back(tmp);
	nClauses++;
	
	return sorted;
}
void printProblem(const Minisat::Problem& p)
{
	for (size_t i = 0; i < p.size(); i++) {
		for (int j = 0; j < p[i]->size(); j++) {
			Minisat::Lit& lit = (*p[i])[j];
			cout << (sign(lit) ? "-" : "") << var(lit) + 1 << (j + 1 == p[i]->size() ? " 0\n" : " ");
		}
	}
}


Minisat::Problem FPP::convert_to_problem_base(){
	Minisat::Problem cnf;
	Minisat::Disjunct *lits;
	for (int i = 0; i < M.size(); i++){
		int * tmp;
		tmp = M[i];
		lits = new Minisat::Disjunct;
		int k = 0;
		for (;;){
			if (tmp[k] == 0) break;
			int var = abs(tmp[k]) - 1;
			lits->push((tmp[k]>0) ? Minisat::mkLit(var) : ~Minisat::mkLit(var));
			k++;
		}
		cnf.push_back(lits);
	}
	return cnf;
}


Minisat::Problem FPP::convert_to_problem_vector(vector<int> B){
	Minisat::Problem addition;
	Minisat::Disjunct * lits = 0;
	for (int i = 0; i < B.size(); i++){
		lits = new Minisat::Disjunct;
		int var = abs(B[i]) - 1;
		lits->push((B[i]>0) ? Minisat::mkLit(var) : ~Minisat::mkLit(var));
		addition.push_back(lits);
	}
	return addition;
}
