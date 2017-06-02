#pragma once
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

const unsigned maxNum = (4096 / 8) - 2;
const unsigned minNum = maxNum / 2;

struct BpTreeNode {
	int count;
	bool leaf;
	union {
		BpTreeNode **sNode;			// pointer to node for smaller value (compared to each entry)
		int *bNum;					// block number
	};
	float *key;
	BpTreeNode *p;					//pointer to node for larger value (compared to every key value in node)

	BpTreeNode() {
		count = 0;
		leaf = true;
		bNum = new int[maxNum];
		key = new float[maxNum];
	}
	~BpTreeNode() { delete bNum; delete key; }
	void sort() {
		int i, j, bTemp;
		float kTemp;
		for (i = 0; i < count; i++) {
			for (j = i; j <= count; j++) {
				if (key[i] > key[j]) {
					kTemp = key[i];
					bTemp = bNum[i];
					key[i] = key[j];
					bNum[i] = bNum[i];
					key[j] = kTemp;
					bNum[j] = bTemp;
				}
			}
		}
	}
};

class BpTree {
private:
	BpTreeNode* root;
	ofstream indexFile;
	float splitNode(BpTreeNode* x, int i);
	void SaveIntoFile();
public:
	BpTree(string file);
	~BpTree();
	void insert(float data, int bNum);
	void update(float data, int oldBNum, int newBNum);
	BpTreeNode* searchNode(int k);
};
