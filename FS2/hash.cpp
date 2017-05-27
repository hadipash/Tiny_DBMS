#include "hash.h"

Hash::Hash(unsigned N, fstream* hash) {
	i = 1;							// set initial prefix to 1
	minDepth = 1;					// initial local minimal depth equals to 1
	numberOfBlocks = 2;				// initially 2 blocks are allocated
	table = new HashEntry[N];		// create a hash table
	table[0].SetAlloc(true);
	table[1].SetAlloc(true);
	hashFile = hash;
}

Hash::~Hash() {
	delete table;
}

unsigned Hash::Hashing(unsigned key) {
	unsigned pref = 0;

	for (unsigned d = minDepth; d <= i; d++) {
		pref = (key << (sizeof(unsigned) - d)) >> (sizeof(unsigned) - d);	// extract prefix

		if (table[pref].isAllocated() && table[pref].GetDepth() == d)		// whether a block is allocated and have the same depth
			if (table[pref].isFull())
				Split(pref, d);
			else {
				table[pref].IncreaseRecNum();
				return table[pref].GetBlNum();
			}
	}
}

void Hash::Split(unsigned prefix, unsigned depth) {
	// allocate a new block
	unsigned newBlock = 1 << (depth + 1) + prefix;
	table[newBlock].SetValue(++numberOfBlocks);

	// increase local depths
	table[prefix].SetDepth(depth + 1);
	table[newBlock].SetDepth(depth +1);

	// if local depth of the original block equals to global depth
	if (i == depth)
		i++;

	for (unsigned k = 0; k < maxRecNum; k++) {
		// read from file and insert to new locations
	}
}

void Hash::FindMinDepth() {
	minDepth = table[0].GetDepth();

	for (unsigned k = 1; k < 500; k++)
		if (table[k].isAllocated() && table[k].GetDepth() < minDepth)
			minDepth = table[k].GetDepth();
}