#pragma once
#include "hash.h"

using namespace std;

class DB {
private:
	// database files
	ofstream DataBase;
	fstream HashFile;
	fstream ScoreTree;

	Hash* hash;

public:
	DB(string db, string hashfile, string score);
	void InsertRecord(unsigned ID, char name[20], float score, unsigned advID);
	void Update(unsigned ID, char name[20], float score, unsigned advID);
};