#include "DB.h"

DB::DB(string db, string hashfile, string score) {
	DataBase.open(db, ios_base::out | ios_base::binary);
	HashFile.open(hashfile, ios_base::binary);
	ScoreTree.open(score, ios_base::binary);

	hash = new Hash((unsigned)500, &HashFile);
}

void DB::InsertRecord(unsigned ID, char name[20], float score, unsigned advID) {
	unsigned blockNum = hash->Hashing(ID);	// insert a key value into tha hash table and return a block number

	// 혜인: Then, insert new value to the B+-tree with known block number

	// Last, insert a record into DB file
}

void DB::Update(unsigned ID, char name[20], float score, unsigned advID) {
	// Same as InsertRecord(), however instead of inserting a new node into B+-tree
	// updates already existing one. Later will merge it with InsertRecord().

	unsigned blockNum = hash->Hashing(ID);

	// 혜인: update information in B+-tree

	// insert a record into DB file
}
