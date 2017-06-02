#include "B+-tree.h"

BpTree::BpTree(string file) {
	root = NULL;
	indexFile.open(file, ios_base::out | ios_base::binary);
}

BpTree::~BpTree() {
	SaveIntoFile(root);
	indexFile.close();
}

float BpTree::splitNode(BpTreeNode* x, int* path)
{
	int i, j, middleBNum;
	float middleKey;
	BpTreeNode *nParent, *nChild, *middleNode;
	int num = sizeof(path) / sizeof(int);

	i = 0;
	while (num - i != 0) {
		if (i == 0 && x->count == maxNum) {
			nParent = new BpTreeNode();
			nChild = copyHalfNode(x, 1);
			x = copyHalfNode(x, 0);
			nChild->count = minNum;
			x->count = maxNum;

			nParent->leaf = false;
			nParent->key[0] = middleKey;
			nParent->bNum[0] = middleBNum;
			nParent->sNode[0] = x;
			nParent->sNode[1] = nChild;
			nParent->count++;
			nChild->next = x;
		}
		else if (i != num && x->count == maxNum) {
			nChild = copyHalfNode(x, 1);
			x = copyHalfNode(x, 0);
			nChild->next = x->next;
			x->next = nChild;

			nParent = x;
			for (j = 0; j < i; j++) {
				nParent = nParent->sNode[path[j + 1]];
			
				//not finished yet
			}
		}
		i++;
		x = x->sNode[i];
	}

	return middleKey;
}

BpTreeNode* BpTree::copyHalfNode(BpTreeNode*x, int flag) {
	int i;
	BpTreeNode* newNode = new BpTreeNode;
	
	newNode->leaf = x->leaf;
	newNode->next = x->next;
	if (flag == 0) {
		for (i = 0; i < minNum; i++) {
			newNode->key[i] = x->key[i];
			newNode->bNum[i] = x->bNum[i];
			newNode->sNode[i] = x->sNode[i];
		}
	}
	else {
		for (i = maxNum; i >= minNum; i--) {
			newNode->key[maxNum - i] = x->key[i];
			newNode->bNum[maxNum - i] = x->bNum[i];
			newNode->sNode[maxNum - i] = x->sNode[i];
		}
	}

	return newNode;
}

void BpTree::insert(float a, int b) {
	int i, j = 0, *path;
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
		path = findPath(x, a);

		// split nodes in path
		splitNode(x, path);

		// go through the path and insert data
		x->key[x->count] = a;
		x->bNum[x->count] = b;
		x->sort();
		x->count++;
	}	
}

int* BpTree::findPath(BpTreeNode* x, float a) {
	int i, j, path[4];
	path[j++] = 0;
	while (!x->leaf) {
		if (a < x->key[0]) {
			path[j++] = 0;
			x = x->sNode[0];
		}
		else if (a >= x->key[x->count]) {
			x = x->sNode[x->count + 1];
			path[j++] = x->count + 1;
		}
		else {
			for (i = 0; i < x->count; i++) {
				if ((a >= x->key[i]) && (a < x->key[i + 1])) {
					path[j++] = i + 1;
					x = x->sNode[i + 1];
					break;
				}
			}
		}
	}

	return path;
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
				x = x->sNode[x->count + 1];
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
	for (i = 0; i < k; i++)
		x = x->next;

	cout << "Number Of " << k << "th node entries:" << x->count << endl;
	for (i = 0; i < x->count; i++) {
		cout << "Key: " << x->key[i] << "\tValue: " << x->bNum[i] << endl;
	}

	return x;
}

void BpTree::SaveIntoFile(BpTreeNode* x) {
	if (x != NULL) {
		for (int i = 0; i < maxNum; i++)
			indexFile << x->key << x->bNum;
		indexFile << x->next;

		if (!(x->leaf)) {
			for (int i = 0; i < root->count; i++)
				if (x->sNode[i])
					SaveIntoFile(x->sNode[i]);

			if(x->next)
				SaveIntoFile(x->next);
		}
	}
}