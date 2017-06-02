#pragma once
#include <iostream>
#include "hash.h"
#include "B+-tree.h"

using namespace std;

struct Record {
	unsigned ID;
	char name[20];
	float score;
	unsigned advID;

	// Constructor for reading records from a binary file
	Record()
		: ID(0), score(0), advID(0) {
		strcpy_s(name, "\0");
	}

	// Constructor for reading records from a txt file
	Record(unsigned stID, string stName, float sc, unsigned aID)
		: ID(stID), score(sc), advID(aID) {
		size_t length = stName.copy(name, 20);
		if (length < 20)
			name[length] = '\0';
	}
};

class DB {
private:
	// database files
	fstream DataBase;
	fstream HashFile;
	fstream ScoreTree;

	string db;

	unsigned const bs = 4096;					// size of a block in bytes
	unsigned const bf = bs / sizeof(Record);	// blocking factor
	unsigned N;									// Number of records to insert

	Hash* hash;	// Hash function
	BpTree* indexTree;

public:
	DB(string db, string hashfile, string score, unsigned n);
	~DB();
	void InsertRecord(unsigned ID, string name, float score, unsigned advID);
	void Update(unsigned blockNum);
};