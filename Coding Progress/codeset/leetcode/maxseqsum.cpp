#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<memory.h>
#define k 0x3f3f3f3f
using namespace std;

extern int ii;

int main(){
	int ii=100;
    int n;
    long long int temp;
    long long int sum=0;
    long long int Max = -k;
	
	int a[] = {1,-5,-3,2,4};
	
	sum = 0;
	Max = -k;
	for(int i=0; i<sizeof(a)/sizeof(int); i++){
		temp = a[i];
		sum = max(sum+temp, temp);
		cout << "sum = " << sum <<"  ";
		Max = max(Max, sum);
		cout << "Max = " << Max <<endl;
	}
	cout<<Max<<endl;
	
	cout<<ii<<endl;
}
