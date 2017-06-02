#include "B+-tree.h"

BpTree::BpTree(string file) {
	root = NULL;
	indexFile.open(file, ios_base::out | ios_base::binary);
}

BpTree::~BpTree() {
	SaveIntoFile();
	indexFile.close();
}

float BpTree::splitNode(BpTreeNode *x, int i)
{
	int j, middleBNum;
	float middleKey;
	BpTreeNode *nParentNode, *nChildNode, *child, *middleSNode;

	nChildNode = new BpTreeNode;					// new leaf node

	if (i == -1) {								// if root node is full
		nParentNode = new BpTreeNode;			// new parent node
		nParentNode->leaf = false;
		middleKey = x->key[minNum];				// middle value will be stored in parent's node

		if (x->leaf) {								//root && leaf
			middleBNum = x->bNum[minNum];
			x->key[minNum] = 0;						// remove value (set to initial value)
			x->bNum[minNum] = -1;
			x->count--;

			// set newly created node value & remove original node value
			for (j = minNum + 1; j < maxNum; j++) {
				nChildNode->key[j - (minNum + 1)] = x->key[j];
				nChildNode->bNum[j - (minNum + 1)] = x->bNum[j];
				nChildNode->count++;
				x->count--;
			}
		}
			nParentNode->key[0] = middleKey;
			nParentNode->sNode[0] = x;
			nParentNode->sNode[1] = nChildNode;
			nParentNode->count++;
			nChildNode->ptr = x->ptr;
			x->ptr = nChildNode;
			root = nParentNode;
	}
	else {										// if x's child node is full
		child = x->sNode[i];

		if (child->leaf) {						// nonroot && leaf
			middleKey = child->key[minNum];

			child->key[minNum] = 0;
			child->bNum[minNum] = -1;
			child->count--;

			for (j = (minNum + 1); j < maxNum; j++) {
				nChildNode->key[j - (minNum + 1)] = child->key[j];
				nChildNode->bNum[j - (minNum + 1)] = child->bNum[j];
				nChildNode->count++;
				child->key[j] = 0;
				child->bNum[j] = -1;
				child->count--;
			}

			nChildNode->p = child->p;
			child->p = nChildNode;
		}
		else {									// nonroot && inner
			child->key[minNum] = 0;
			child->sNode[minNum] = NULL;
			child->count--;

			for (j = (minNum + 1); j < maxNum; j++) {
				nChildNode->key[j - (minNum + 1)] = child->key[j];
				nChildNode->sNode[j - (minNum + 1)] = child->sNode[j];
				nChildNode->count++;
				child->key[j] = 0;
				child->sNode[j] = NULL;
				child->count--;
			}
		}

		nChildNode->p = child->p;
		child->p = nChildNode;
		x->leaf = false;
		x->sNode[i] = child;
		x->sNode[i + 1] = nChildNode;
		x->count = x->count - maxNum;
	}
	return middleKey;
}

void BpTree::insert(float a, int b) {
	int i;
	float temp;
	BpTreeNode *x;

	x = root;
	if (x == NULL) {
		x = new BpTreeNode;
		x->key[0] = a;
		x->bNum[0] = b;
		x->count++;
		root = x;
	}
	else {
		// find path to insert

		// spilt nodes in path

		// insert data
		if (x->leaf == true && x->count == maxNum) {	// leafnode overhead
			temp = splitNode(x, -1);
			x = root;
			for (i = 0; i < (x->count); i++) {
				if (a < x->key[0])
					x = x->sNode[0];
				else if (a >= x->key[x->count])
					x = x->ptr;
				else {
					for (i = 0; i < (x->count); i++) {
						if ((a >= x->key[i]) && (a < x->key[i + 1])) {
							x = x->sNode[i++];
							break;
						}
					}
				}
			}
			x = x->sNode[i];
		}
		else {		// innernode
			while (!x->leaf) {						// find leafnode
				if (a < x->key[0])
					x = x->sNode[0];
				else if (a >= x->key[x->count])
					x = x->ptr;
				else {
					for (i = 0; i < (x->count); i++) {
						if ((a >= x->key[i]) && (a < x->key[i + 1])) {
							x = x->sNode[i++];
							break;
						}
					}
				}

				if (x->sNode[i]->count == maxNum) {		// innernode overhead
					temp = splitNode(x, i);
					x->key[x->count] = temp;
					x->count++;
					continue;
				}
				else
					x = x->sNode[i];
			}
		}
		// insert value
		x->key[x->count] = a;
		x->bNum[x->count] = b;
		x->sort();
		x->count++;
	}	
}

BpTreeNode* findPath(BpTreeNode* x, float a) {
	int i;
	BpTreeNode* path[4];
	path[i++] = x;
	while (!x->leaf) {
		if (a < x->key[0])
			x = x->sNode[0];
		else if (a >= x->key[x->count]) {
			if (x->count < maxNum)
				x = x->sNode[x->count + 1];
			else if (x->count = maxNum)
				x = x->ptr;
		}
		else {
			for (i = 0; i < x->count; i++) {
				if ((a >= x->key[i]) && (a < x->key[i + 1])) {
					x = x->sNode[i++];
					break;
				}
			}
		}
		path[i++] = x;
	}
}

void BpTree::update(float a, int oldBnum, int newBNum) {
	int i;
	BpTreeNode* x = root;

	if (x == NULL)
		return;
	else {
		while (x->leaf == false) {						// find leafnode
			if (a < x->key[0])
				x = x->sNode[0];
			else if (a >= x->key[x->count])
				x = x->ptr;
			else {
				for (i = 0; i < (x->count); i++) {
					if ((a >= x->key[i]) && (a < x->key[i + 1])) {
						x = x->sNode[i++];
						break;
					}
				}
			}
		}

		for (i = 0; i < x->count; i++) {
			if ((a == x->key[i]) && (oldBnum == x->bNum[i])) {
				x->bNum[i] = newBNum;
				break;
			}
		}
	}
}

BpTreeNode* BpTree::searchNode(int k) {
	int i;
	BpTreeNode *x = root;

	// find fisrt leaf node
	while (!x->leaf) {
		x = x->sNode[0];
	}

	// move to k-th leaf node
	for (i = 0; i < k; i++) {
		x = x->p;
	}

	cout << "Number Of " << k << "th node entries:" << x->count << endl;
	for (i = 0; i <= x->count; i++) {
		cout << "Key: " << x->key[i] << "\tValue: " << x->bNum[i] << endl;
	}

	return x;
}

void BpTree::SaveIntoFile() {
	BpTreeNode* x = root;
	int i;

	for (i = 0; i < x->count; i++)
		indexFile << x->key << x->bNum;
}