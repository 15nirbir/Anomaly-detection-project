#include <iostream>
#include "anomaly_detection_util.h"
#include <cmath>
using namespace std;

	
	float avg(float *x , int size) {
		float xCounter = 0;
		for(int i = 0; i < size; i++){
			xCounter += *(x+i);
		}
		return xCounter / size;
	}
	//calculate the variance with the formula ((1/n) *sum of i=0 to no of xi^2) * ((1/n) *
	//(sum of i=0 to no of xi^2)
	float var(float* x, int size) {
		//size = n
		float divider = 1.0/size;
		float sumOfSquares = 0;
		float sum = 0;
		//clalulate the sum of i=0 to no of xi^2.
		for (int i = 0;i < size; i++) {
			sumOfSquares+=( *(x+i))*( *(x+i));
			sum += *(x+i);       
		}
		return (divider * sumOfSquares) - (divider * sum)* (divider * sum); 
			
		}
	float cov(float* x, float* y, int size) {
		float xAverage = avg(x , size);
		float yAverage = avg(y ,size);
		float sum = 0;
		for(int i = 0;i < size; i++) {
			sum =sum + ((*(x + i))- xAverage) * ((*(y + i)) - yAverage);
		}
		
		return sum / size;
	}

	float standardDeviation(float* x,int size) {
		return sqrt(var(x,size));
	}
	float pearson(float* x, float* y, int size) {
		return cov(x , y ,size)/(standardDeviation(x , size)* standardDeviation(y,size));
	}
	Line linear_reg(Point** points, int size) {
		float x[size];
		float y[size];
		for(int i = 0; i < size;i++) {
			*(x + i) = (*(points + i))->x;
			*(y + i) = (*(points + i))->y;
			// std :: cout <<  (*(points + i))->x << std :: endl;
			// std :: cout <<  (*(points + i))->y << std :: endl;
		}
		float a = cov(x , y , size) / var(x , size);
		float b = avg(y , size) - a * avg(x , size);
		// std :: cout << "a is " << a << std :: endl;
		// std :: cout << "b is " << b << std :: endl;
		// std :: cout << cov(x , y , size) << std :: endl;
		// std :: cout << var(x , size) << std :: endl;
		return Line(a,b);

	}
	// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size) {
	Line line = linear_reg(points , size);
	float yVal = line.f(p.x);
	if(p.y - yVal >= 0) {
		return p.y - yVal;
	}
	else {
		return yVal - p.y;
	}
}
// returns the deviation between point p and the line
float dev(Point p,Line l) {
	float yVal = l.f(p.x);
	if(p.y - yVal >= 0){
		return p.y - yVal;
	}
	else{
		return yVal - p.y;
	}
}
	
bool wrong(float val, float expected){
	return val<expected-0.001 || val>expected+0.001;
}

// // this is a simple test to put you on the right track
// int main(){
// 	const int N=10;
// 	float x[]={1,2,3,4,5,6,7,8,9,10};
// 	float y[]={2.1,4.2,6.1,8.1,10.3,12.2,14.4,16.1,18.2,20.3};

// 	Point* ps[N];
// 	for(int i=0;i<N;i++)
// 		ps[i]=new Point(x[i],y[i]);

// 	Line l=linear_reg(ps,N);
// 	Point p(4,8);

// 	float v[]={var(x,N),cov(x,y,N),pearson(x,y,N),l.a,l.b,l.f(4),dev(p,l)};
// 	float e[]={8.25,16.63,0.999,2.015,0.113,8.176,0.176};


// 	for(int i=0;i<7;i++)
// 		if(wrong(v[i],e[i]))
// 			cout<<"error for check "<<i<<" (-14)"<<endl;


// 	for(int i=0;i<N;i++)
// 		delete ps[i];

// 	cout<<"done"<<endl;
// 	return 0;
// }

		

