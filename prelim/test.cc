#include <iostream>

using namespace std;

int main()
{
	int i = 7;
	int *ipointer;
	int &ireference=i; 

	ipointer = &i;
	//ireference = *i;

	cout << *ipointer << endl;
	cout << ireference << endl;
}