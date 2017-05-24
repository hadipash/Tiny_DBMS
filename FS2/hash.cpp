#include "hash.h"

Hash::Hash(unsigned N) {
	i = 1;
	numberOfBlocks = 0;
	table = new HashEntry[N];		// create a hash table
}

Hash::~Hash() {
	delete table;
}

unsigned Hash::Hashing(unsigned key) {
	unsigned pref = (key << (sizeof(unsigned) - i)) >> (sizeof(unsigned) - i);	// extract prefix

	if (!table[pref].isAllocated)
		table[pref].SetValue(++numberOfBlocks);
	else
		if (!isAvailable(table[pref].GetBlNum))
			Split(table[pref].GetBlNum);


	return table[pref].GetBlNum;
}

bool Hash::isAvailable(unsigned block)
{
	return false;
}

void Hash::Split(unsigned block) {

}