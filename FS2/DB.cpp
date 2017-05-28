#include "DB.h"

DB::DB(string db, string hashfile, string score) {
	DataBase.open(db, ios_base::binary);
	HashFile.open(hashfile, ios_base::binary);
	ScoreTree.open(score, ios_base::binary);

	hash = new Hash((unsigned)500, &HashFile); // 500 - just for test. Will change it later
}

void DB::InsertRecord(unsigned ID, char name[20], float score, unsigned advID) {
	bool update = false;	// whether need to read and insert again records from an overflowed block or not
	unsigned blockNum = hash->Hashing(ID, &update);	// insert a key value into tha hash table and return a block number

	if (update)
		Update(blockNum);

	// 혜인: Then, insert new value to the B+-tree with known block number

	// Last, insert a record into DB file
}

void DB::Update(unsigned blockNum) {
	// If splitting of a block is required
	// Read all records from the block
	// Send the key into hashing function
	// Update values in B+-tree
	// Insert in a new block
}
