#pragma once
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

const unsigned maxNum = (4096 / 16) - 2;
const unsigned minNum = maxNum / 2;

struct BpTreeNode {
	int count;
	bool leaf;	
	BpTreeNode *sNode[maxNum + 1];			// pointer to node for smaller value (compared to each entry)
	int bNum[maxNum];					// block number
	double key[maxNum];
	BpTreeNode* next;

	BpTreeNode() {
		int i;
		count = 0;
		leaf = true;
		next = NULL;
		sNode[maxNum + 1] = NULL;
		for (i = 0; i < maxNum; i++) {
			sNode[i] = NULL;
			key[i] = 0;
			bNum[i] = -1;
		}
	}
	~BpTreeNode() { 
		delete bNum;
		delete key;
	}
	
	void sort() {
		int i, j, bTemp;
		double kTemp;
		for (i = 0; i < count; i++) {
			for (j = i; j < count + 1; j++) {
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
	int depth;
	int path[4];
	void arrangePath(BpTreeNode* x);
	void SaveIntoFile(BpTreeNode* root);
	void spiltNode(BpTreeNode*x, BpTreeNode* y);
	void findPath(BpTreeNode* x, double a);
public:
	BpTree(string file);
	~BpTree();
	void insert(double data, int bNum);
	void update(double data, int oldBNum, int newBNum);
	BpTreeNode* searchNode(int k);
};
