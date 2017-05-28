#include "hash.h"

Hash::Hash(unsigned N, fstream* hash) {
	i = 1;							// set initial prefix to 1
	minDepth = 1;					// initial local minimal depth equals to 1
	numberOfBlocks = 1;				// initially 2 blocks are allocated (start from 0)
	maxNumberOfBlocks = N;

	table = new HashEntry[maxNumberOfBlocks];		// create a hash table
	table[0].SetValue(0);			// initialize first two blocks
	table[1].SetValue(1);

	hashFile = hash;
}

Hash::~Hash() {
	delete table;
}

unsigned Hash::Hashing(unsigned key, bool* update, unsigned* pos) {
	for (unsigned d = minDepth; d <= i; d++) {
		unsigned pref = (key << (sizeof(unsigned) * 8 - d)) >> (sizeof(unsigned) * 8 - d);	// extract prefix

		if (table[pref].isAllocated() && table[pref].GetDepth() == d)	// whether a block is allocated and have the same depth
			if (table[pref].isFull()) {									// if block is full - split it
				*update = true;
				Split(pref, d);
				return table[pref].GetBlNum();
			}
			else {
				*pos = table[pref].GetRecNum();
				return table[pref].GetBlNum();
			}
	}
}

void Hash::Split(unsigned prefix, unsigned depth) {
	// allocate a new block
	unsigned newBlock = (1 << depth) + prefix;
	table[newBlock].SetValue(++numberOfBlocks);

	// increase local depths
	table[prefix].SetDepth(depth + 1);
	table[newBlock].SetDepth(depth +1);

	// Reset a number of records in blocks to zero
	table[prefix].ResetRecNum();
	table[newBlock].ResetRecNum();

	// if local depth of the original block equals to global depth
	if (i == depth)
		i++;

	// Find new minimum local depth
	FindMinDepth();
}

void Hash::FindMinDepth() {
	minDepth = table[0].GetDepth();

	for (unsigned k = 1; k < maxNumberOfBlocks; k++)
		if (table[k].isAllocated() && table[k].GetDepth() < minDepth)
			minDepth = table[k].GetDepth();
}