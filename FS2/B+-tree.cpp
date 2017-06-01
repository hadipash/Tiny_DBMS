#include "B+-tree.h"

BpTree::BpTree(fstream* file) {
	root = NULL;
	indexFile = file;
}

float BpTree::splitNode(BpTreeNode *x, int i)
{
	int j, middleBNum;
	float middleKey;
	BpTreeNode *nParentNode, *nLeafNode, *leaf;

	nLeafNode = new BpTreeNode;					// new leaf node

	if (i == -1) {								// if root node is full
		nParentNode = new BpTreeNode;			// new parent node
		nParentNode->setInnerNode();
		middleKey = x->key[minNum];				// middle value will be stored in parent's node
		middleBNum = x->bNum[minNum];
		x->key[minNum] = 0;						// remove value (set to initial value)
		x->bNum[minNum] = -1;
		x->count--;

		// set newly created node value & remove original node value
		for (j = minNum + 1; j < maxNum; j++) {
			nLeafNode->key[j - (minNum + 1)] = x->key[j];
			nLeafNode->bNum[j - (minNum + 1)] = x->bNum[j];
			nLeafNode->count++;
			x->count--;
		}

		nParentNode->key[0] = middleKey;
		nParentNode->bNum[0] = middleBNum;
		nParentNode->sNode[0] = x;
		nParentNode->sNode[1] = nLeafNode;
		nParentNode->count++;
		root = nParentNode;
	}
	else {										// if x's child node is full
		leaf = x->sNode[i];
		middleKey = leaf->key[minNum];
		leaf->key[minNum] = 0;
		leaf->bNum[minNum] = -1;
		leaf->count--;

		for (j = (minNum + 1); j < maxNum; j++) {
			nLeafNode->key[j - (minNum + 1)] = leaf->key[j];
			nLeafNode->bNum[j - (minNum + 1)] = leaf->bNum[j];
			nLeafNode->count++;
			leaf->key[j] = 0;
			leaf->bNum[j] = -1;
			leaf->count--;
		}

		x->setInnerNode();
		x->sNode[i] = leaf;
		x->sNode[i + 1] = nLeafNode;
		x->count = x->count - maxNum;
	}
	return middleKey;
}

void BpTree::insert(float a, int b) {
	int i;
	float temp;
	vector<BpTreeNode*> path;
	BpTreeNode *x;

	x = root;
	if (x == NULL) {
		root = new BpTreeNode;
		x = root;
		x->key[0] = a;
		x->bNum[0] = b;
		x->count++;
	}
	else {
		if (x->isLeaf() == true && (x->count % maxNum) == 0) {	// leafnode overhead
			temp = splitNode(x, -1);
			x = root;
			for (i = 0; i < (x->count); i++) {
				if ((a > x->key[i]) && (a < x->key[i + 1])) {
					i++;
					break;
				}
				else if (a < x->key[0])
					break;
			}
			x = x->sNode[i];
		}
		else {		// innernode
			while (x->isLeaf() == false) {						// find leafnode
				for (i = 0; i < (x->count); i++) {
					if ((a > x->key[i]) && (a < x->key[i + 1])) {
						i++;
						break;
					}
					else if (a < x->key[0])
						break;
				}

				if ((x->sNode[i]->count % maxNum) == 0) {		// innernode overhead
					temp = splitNode(x, i);
					x->key[x->count] = temp;
					x->count++;
					continue;
				}
				else
					x = x->sNode[i];
			}
		}
	}

	// insert value
	x->key[x->count] = a;
	x->bNum[x->count] = b;
	x->sort();
	x->count++;
}