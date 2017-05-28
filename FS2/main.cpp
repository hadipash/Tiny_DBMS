#include <sstream>
#include "DB.h"

void main() {
	unsigned recNum;	// number of records

	string DataBase = "Students.DB";
	string HashFile = "Students.hash";
	string ScoreFile = "Students_score.idx";
	string Data = "sampleData.csv";

	ifstream DataFile(Data, ios_base::in); //open file for reading
	if (DataFile.is_open()) {
		string line, temp; //strings for storing temporary values
		unsigned studID, advID;
		char studName[20];
		float score;
		string Sname, SID, SaID, Sscore;	// temporary strings

		getline(DataFile, line);
		recNum = stoi(line);

		DB* db = new DB(DataBase, HashFile, ScoreFile, recNum);

		getline(DataFile, line);
		stringstream ss(line);

		//reading values line by line and insert them into DB
		for (unsigned i = 0; i < recNum; i++) {
			getline(ss, Sname, ',');
			strcpy_s(studName, Sname.c_str());
			getline(ss, SID, ',');
			studID = stoi(SID);
			getline(ss, Sscore, ',');
			score = stof(Sscore);
			getline(ss, SaID);
			advID = stoi(SaID);

			db->InsertRecord(studID, studName, score, advID);
			getline(DataFile, line);
			ss.str(string());
			ss.clear();
			ss << line;
		}

		DataFile.close();
		delete db;
	}
}