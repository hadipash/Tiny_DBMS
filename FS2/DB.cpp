#include "DB.h"

DB::DB(string db, string hashfile, string score, unsigned n) {
	N = n;
	this->db = db;
	DataBase.open(db, ios_base::in | ios_base::out | ios_base::trunc | ios_base::binary);
	HashFile.open(hashfile, ios_base::in | ios_base::out | ios_base::trunc | ios_base::binary);
	ScoreTree.open(score, ios_base::in | ios_base::out | ios_base::trunc | ios_base::binary);

	// Allocate DB file with initial two blocks
	if (DataBase.is_open()) {
		DataBase.seekp((1 << 13) - 1);	// 8096 bytes
		DataBase.write("", 1);
	}
	
	hash = new Hash(N/bf * 3, &HashFile); // Suppose that the most worst utilization of block is 1/3
	indexTree = new BpTree(&ScoreTree);
}

DB::~DB() {
	DataBase.close();
	HashFile.close();
	ScoreTree.close();

	delete hash;
	delete indexTree;
}

void DB::InsertRecord(unsigned ID, string name, float score, unsigned advID) {
	bool update = false;	// whether need to read and insert again records from an overflowed block or not
	unsigned offset = 0;	// position of a recond in a block (to speed-up inserting)
	unsigned blockNum = hash->Hashing(ID, &update, &offset);	// insert a key value into tha hash table and return a block number

	if (update) {
		Update(blockNum);						// Re-insert records from an overflowed block
		InsertRecord(ID, name, score, advID);	// Insert a new record
	} else {
		// Then, insert new value to the B+-tree with known block number
		indexTree->insert(score, blockNum);

		// Insert a record into DB file
		Record record(ID, name, score, advID);
		cout << "Inserting " << record.ID << " into " << blockNum << " block" << endl;
		DataBase.seekp(blockNum * bs + offset * sizeof(Record));
		DataBase.write(reinterpret_cast<const char*>(&record), sizeof(Record));
	}
}

void DB::Update(unsigned blockNum) {
	Record* record = new Record[bf];

	DataBase.seekg(blockNum * bs);

	// Read all records from an overflowed block
	for (unsigned i = 0; i < bf; i++)
		DataBase.read((char*)&record[i], sizeof(Record));

	// Erase values from an old block
	DataBase.seekp(blockNum * bs);
	DataBase.write("", bs);

	// Allocate space for a new block
	DataBase.seekp(0, ios_base::end);
	DataBase.write("", bs);

	// Insert records to new blocks
	for (unsigned i = 0; i < bf; i++)
		InsertRecord(record[i].ID, record[i].name, record[i].score, record[i].advID);
}