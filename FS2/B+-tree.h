#pragma once
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

const unsigned maxNum = (4096 / 8) - 1;
const unsigned minNum = maxNum / 2;

struct BpTreeNode {
	int count;
	union {
		BpTreeNode **sNode;
		int *bNum;
	};
	float *key;
	BpTreeNode *p;

	BpTreeNode() {
		count = maxNum; // default: leaf node
		bNum = new int[maxNum];
		key = new float[maxNum];
	}
	~BpTreeNode() { delete bNum; delete key; }
	bool isLeaf() { return (count / (maxNum + 1) == 1) ? true : false; } // leaf node's count value is bigger than maxNum
	bool isFull() { return (count == maxNum) ? true : false; }
	void setInnerNode() { if (count / (maxNum + 1) == 1) count = count - maxNum; } // inner node's count value is smaller than or equal to maxNum
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
	BpTreeNode *root;
	fstream* indexFile;
	float splitNode(BpTreeNode *x, int i);
public:
	BpTree(fstream* file);
	void insert(float a, int b);
};
