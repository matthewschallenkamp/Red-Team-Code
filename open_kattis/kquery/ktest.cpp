#include <iostream>

using namespace std;

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	cout << 30000 << endl;
	for(int i = 0; i < 30000; i++)
		cout << 1 << " ";
	cout << endl;
	cout << 200000 << endl;
	for(int i = 0; i < 200000; i++)
		cout << 7500 << " " << 22500 << " " << 2 << endl;
	return 0;
}
