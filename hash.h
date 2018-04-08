#pragma once
#include <fstream>

using namespace std;

const unsigned maxRecNum = 4096 / 32;	// Assume, that block size is 4096 bytes and record size is 32 bytes

class HashEntry {
private:
	unsigned bNum;							// block number
	unsigned depth;							// depth of a bucket (how many keys pointing to a bucket)
	unsigned recNumber;						// number of records in a bucket
	bool alloc;								// whether a block for this entry is allocated or not

public:
	HashEntry()					{ alloc = false; recNumber = 0; depth = 1; }
	void SetValue(unsigned b)	{ bNum = b; alloc = true; }
	void SetDepth(unsigned d)	{ depth = d; }
	void ResetRecNum()			{ recNumber = 0; }
	unsigned GetRecNum()		{ return recNumber++; }
	bool isAllocated()			{ return alloc; }
	bool isFull()				{ return (recNumber == maxRecNum);  }
	unsigned GetBlNum()			{ return bNum;	}
	unsigned GetDepth()			{ return depth; }
};

class Hash {
private:
	unsigned i;								// number of digits in a hash prefix (global depth)
	unsigned numberOfBlocks;				// number of used blocks
	unsigned maxNumberOfBlocks;				// maximum number of blocks
	unsigned minDepth;						// minimum local depth
	HashEntry* table;						// hash table
	ofstream hashFile;						// the hash table file
	void FindMinDepth();					// find minimum local depth
	void Split(unsigned prefix, unsigned depth);	// split blocks in the case of overflow
	void SaveIntoFile();
	
public:
	Hash(unsigned N, string hash);
	~Hash();
	unsigned Hashing(unsigned key, bool* update, unsigned* pos);	// inset a record into the hash table and return a block number
	void PrintTable();
};