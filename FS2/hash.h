#pragma once
#include <fstream>

//using namespace std;

const unsigned block = 4096;			// size of the disk block in bytes

class HashEntry {
private:
	unsigned bNum;						// block number
	bool alloc;							// check whether a block for this entry is allocated or not
public:
	HashEntry() { alloc = false; }
	void SetValue(unsigned b) { bNum = b; alloc = true; }
	bool isAllocated() { return alloc; }
	unsigned GetBlNum() { return bNum; }
};

class Hash {
private:
	unsigned i;							// number of digits in a hash prefix
	unsigned numberOfBlocks;			// number of used blocks
	HashEntry *table;					// a hash table
	std::ofstream hashFile;				// the hash table file
	bool isAvailable(unsigned block);	// check whether a block is avalaible for insertion or splitting is needed (overflow)
	void Split(unsigned block);			// split blocks in the case of overflow
public:
	Hash(unsigned N);
	~Hash();
	unsigned Hashing(unsigned key);		// inset a record into the hash table and return a block number
};