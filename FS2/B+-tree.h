#pragma once
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

const unsigned maxNum = (4096 / 8) - 2;
const unsigned minNum = maxNum / 2;

struct BpTreeNode {
	int count;
	bool leaf;	
	BpTreeNode **sNode;			// pointer to node for smaller value (compared to each entry)
	int *bNum;					// block number
	float *key;
	BpTreeNode *nextNode;
	BpTreeNode *ptr;

	BpTreeNode() {
		int i;
		count = 0;
		leaf = true;
		bNum = new int[maxNum];
		key = new float[maxNum];
		ptr = NULL;
		sNode = new BpTreeNode*[maxNum];

		for (i = 0; i < maxNum; i++) {
			sNode[i] = NULL;
		}
	}
	~BpTreeNode() { delete bNum; delete key; }
	BpTreeNode* copy(BpTreeNode* x) {

	}
	void sort() {
		int i, j, bTemp;
		float kTemp;
		for (i = 0; i < count; i++) {
			for (j = i; j <= count; j++) {
				if (key[i] > key[j]) {
					kTemp = key[i];
					bTemp = bNum[i];
					key[i] = key[j];
					bNum[i] = bNum[j];
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
