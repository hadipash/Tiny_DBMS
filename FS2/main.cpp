#include <iostream>
#include <bitset>

using namespace std;

void main() {
	unsigned i = 15151515;
	cout << std::bitset<32>(i) << endl;
	i <<= 32;
	i >>= 32;
	cout << std::bitset<32>(i) << endl;
}