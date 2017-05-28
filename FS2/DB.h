#pragma once
#include "hash.h"

using namespace std;

// Size of Record is 32 bytes and block size is 4096 bytes =>
// Blocking factor = 4096 / 32 = 128 records/block
struct Record {
	unsigned ID;
	char name[20];
	float score;
	unsigned advID;

	Record(unsigned stID, char* stName, float sc, unsigned aID)
		: ID(stID), score(sc), advID(aID) {
		strcpy_s(name, stName);
	}
};

class DB {
private:
	// database files
	fstream DataBase;
	fstream HashFile;
	fstream ScoreTree;

	Hash* hash;

public:
	DB(string db, string hashfile, string score);
	void InsertRecord(unsigned ID, char name[20], float score, unsigned advID);
	void Update(unsigned blockNum);
};