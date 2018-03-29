//parallel prime numbet sieve
//I made a mistake by using the memset function;
//for detail look at the blog: https://www.cnblogs.com/anthow/p/3392233.html
#include <iostream>
#include <cmath>
#include <cstring>
using namespace std;

int main() {
	int k = 5;
	int num = 0;
	double MAX1 = pow(2, k) * 1000;
	int MAX = (int)MAX1;
	int loc = 0, count = 0;
	int* prime = new int[MAX];
	int* arr1 = new int[3432];
	int* arr2 = new int[3432];
	int index = 0;
	memset(prime, 1, sizeof(int) * MAX);
	count = 0;
	for (int i = 2; i * i <= MAX; i++) {
	  if (prime[i]) {
		  loc = i + i;
		  while (loc <= MAX) {
			  prime[loc] = false;
			  loc = loc + i;
		  }
	  }
	}
	for (int i = 2; i <= MAX; i++) {
		if (prime[i]) {
			count++;
			arr1[index++] = i;
		}
	}
	int n = MAX;
	cout << "k is " << k;
	cout << " n is " << n;
	cout << " count is " << count << endl;
	return 0;
}
