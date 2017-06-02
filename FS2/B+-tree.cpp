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
		else {										// root && nonleaf
			nChildNode->leaf = false;
			middleSNode = x->sNode[minNum];
			x->key[minNum] = 0;						// remove value (set to initial value)
			x->sNode[minNum + 1] = NULL;
			x->count--;

			// set newly created node value & remove original node value
			for (j = minNum + 1; j < maxNum; j++) {
				nChildNode->key[j - (minNum + 1)] = x->key[j];
				nChildNode->sNode[j - (minNum + 1)] = x->sNode[j];
				nChildNode->count++;
				x->count--;
			}
		}

			nParentNode->key[0] = middleKey;
			nParentNode->sNode[0] = x;
			nParentNode->sNode[1] = nChildNode;
			nParentNode->count++;
			nChildNode->p = x->p;
			x->p = nChildNode;
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
		if (x->leaf == true && x->count == maxNum) {	// leafnode overhead
			temp = splitNode(x, -1);
			x = root;
			for (i = 0; i < (x->count); i++) {
				if ((a >= x->key[i]) && (a < x->key[i + 1])) {
					i++;
					break;
				}
				else if (a < x->key[0])
					break;
			}
			x = x->sNode[i];
		}
		else {		// innernode
			while (!x->leaf) {						// find leafnode
				for (i = 0; i < (x->count); i++) {
					if ((a > x->key[i]) && (a < x->key[i + 1])) {
						i++;
						break;
					}
					else if (a < x->key[0])
						break;
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
	}

	// insert value
	x->key[x->count] = a;
	x->bNum[x->count] = b;
	x->sort();
	x->count++;
}

void BpTree::update(float a, int oldBnum, int newBNum) {
	int i;
	BpTreeNode* x = root;

	if (x == NULL)
		return;
	else {
		while (x->leaf == false) {						// find leafnode
			for (i = 0; i < (x->count); i++) {
				if ((a >= x->key[i]) && (a < x->key[i + 1])) {
					i++;
					break;
				}
				else if (a < x->key[0])
					break;
			}
				x = x->sNode[i];
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
		x->sNode[0];
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
}