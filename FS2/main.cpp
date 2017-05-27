#include <iostream>
#include <sstream>
#include "DB.h"

using namespace std;


void main() {
	int recNum;	// number of records
	string DataBase = "Students.DB";
	string HashFile = "Students.hash";
	string ScoreFile = "Students_score.idx";
	string Data = "sampleData.csv";
	DB* db = new DB(DataBase, HashFile, ScoreFile);

	ifstream DataFile(Data, ios_base::in); //open file for reading
	if (DataFile.is_open()) {
		string line, temp; //strings for storing temporary values
		unsigned studID, advID;
		char studName[20];
		float score;

		getline(DataFile, line);
		recNum = stoi(line);

		getline(DataFile, line);
		stringstream ss(line);

		//reading values line by line and insert them into DB
		for (int i = 0; i < recNum; i++) {
			ss >> studName >> temp >> studID >> temp >> score >> temp >> advID;
			db->InsertRecord(studID, studName, score, advID);
			getline(DataFile, line);
			ss.str(string());
			ss.clear();
			ss << line;
		}

		DataFile.close();
	}

	delete db;
}