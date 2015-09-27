#include <iostream>
#include <vector>
#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

using namespace std;

void insertionsort(vector<int> &v ){
	for (int i = 1; i < v.size(); i++){
		int j = i;
		while((v[j-1] > v[j]) && (j > 0)){
			int tmp = v[j];
			v[j] = v[j-1];
			v[j-1] = tmp;
			
			j--;
		}
	}
}

void mergesort(vector<int> &v, int lo, int hi){
	if (lo>=hi) return;
	
	const int me = (lo + hi)/2;
	mergesort(v, lo, me);
	mergesort(v, me + 1, hi);
	const int size = me - lo + 1;
	int ar[size];
	for(int i = 0; i < size; i++){
		ar[i] = v[lo + i];
	}
	
	int i = 0;
	int j = me + 1;
	int k = lo;
	while(i < size && j <=hi){
		if (ar[i] > v[j]){
			v[k++] = v[j++];
		}
		else {
			v[k++] = ar[i++];
		}
	}
	
	while (i < size){
		v[k++] = ar[i++];
	}
	while(j < size){
		v[k++] = v[j++];
	}
}

template<class T>
void shuffle(vector<T> &v){
	for (int i = 0; i < v.size(); i++){
		int j = rand() % (i + 1);
		T tmp = v[i];
		v[i] = v[j];
		v[j] = tmp;
	}
}

void mergesort(vector<int> &v){
	mergesort(v, 0 , v.size()-1);
}

template<class T>
void partition(vector<T> & v, int lo, int hi){
	if (lo>=hi) return;
	T p = v[lo];
	int i = lo + 1;
	int j = hi;
	while(i < j){
		if (v[i] <= p) {
			i++;
			continue;
		}
		if (v[j] > p){
			j--;
			continue;
		}
		 
		T tmp = v[i];
		v[i] = v[j];
		v[j] = tmp;
	}
	if (v[i] <=p){
		
		v[lo] = v[i];
		v[i] = p;
		partition(v, lo, i-1);
		partition(v, i+1, hi);
	}
	
	else {
		v[lo] = v[i-1];
		v[i-1] = p;
		partition(v, lo, i-2);
		partition(v, i, hi);
	}
}

template<class T>
T select(vector<T> & v, const int k,  int lo, int hi){
	if (lo>=hi) 
		if (lo == k)
		return k;

	T p = v[lo];
	int i = lo + 1;
	int j = hi;
	while(i < j){
		if (v[i] <= p) {
			i++;
			continue;
		}
		if (v[j] > p){
			j--;
			continue;
		}
		 
		T tmp = v[i];
		v[i] = v[j];
		v[j] = tmp;
	}
	if (v[i] <=p){
		
		v[lo] = v[i];
		v[i] = p;
		return i;
	}
	
	else {
		v[lo] = v[i-1];
		v[i-1] = p;
		return i -1;

	}
}

template<class T>
T select(vector<T> &v, const int k){
	int lo = 0;
	int hi = v.size() - 1;
	int i;
	while (lo < hi){
		i = select(v, k, lo, hi);
		if (k < i)
			hi = i-1;
		else if (i < k)	
			lo = i+1; 
		else return v[k];	
	}	
	return v[k];
}

template<class T>
void quickSort(vector<T> &v){
	if (v.size() == 0 || v.size() == 1) return;  
	//shuffle(v);
	partition(v, 0, v.size()-1);
	
		
}

void test(vector<int> &v){
	for (int i = 0; i < v.size(); i++){
		v[i] =0;
	}
	
}

class Point{
	int x;
	int y;
	
	public :
		Point(int xx, int yy):x(xx), y(yy){
			
		}
		bool operator>(const Point& that){
			return x>that.x;
		}
		bool operator<(const Point& that){
			return x<that.x;
		}
		bool operator>=(const Point& that){
			return x>=that.x;
		}
		bool operator<=(const Point& that){
			return x<=that.x;
		}
		friend ostream &operator<<(ostream& os,const Point& p);	
		
};

ostream &operator<<(ostream& os,const Point& p){
	os <<" (" << p.x << ", " << p.y << ") ";
	return os;
}


int main(){
	srand(time(NULL));
	vector<int> v;
	for (int i = 0; i <=9; i++){
		v.push_back(i);
	}
	
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(v.begin(), v.end(), std::default_random_engine(time(NULL)));
	
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << " " ;
	cout << endl;	
	
	for (int i = 0; i < v.size(); i++){
			cout << select(v, i) << endl;
	}

	for (int i = 0; i < v.size(); i++)
		cout << v[i] << " " ;
	cout << endl;	

/*	vector<Point> v;
	int j = 0;
	for(int i = 0; i < 10; i++){
		v.push_back(Point(j, i));
		if (i % 2 == 0) j++;
	}
	shuffle(v);
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << " " ;
	cout << endl;	
	quickSort(v);
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << " " ;
	cout << endl;	
	
*/		
	cout << endl;	
	return 0;
}
