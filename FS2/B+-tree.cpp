#include "B+-tree.h"

BpTree::BpTree(string file) {
	root = NULL;
	depth = 0;
	indexFile.open(file, ios_base::out | ios_base::binary);
}

BpTree::~BpTree() {
	SaveIntoFile(root);
	indexFile.close();
}

void BpTree::arrangePath(BpTreeNode* x)
{
	int i, j, middleBNum;
	double middleKey;
	BpTreeNode *nParent, *nChild;

	i = -1;
	while (depth != i) {
		if (i == - 1 && x->count == maxNum) { // root node is NULL;
			middleKey = x->key[minNum];
			middleBNum = x->bNum[minNum];
			nChild = new BpTreeNode;
			nParent = new BpTreeNode;
			spiltNode(x, nChild);

			nParent->leaf = false;
			nParent->key[0] = middleKey;
			nParent->bNum[0] = middleBNum;
			nParent->sNode[0] = x;
			nParent->sNode[1] = nChild;
			nParent->count++;

			root = nParent;
			depth++;
		}
		else if (i != - 1 && x->count == maxNum) {
			middleKey = x->key[minNum];
			middleBNum = x->bNum[minNum];
			nChild = new BpTreeNode;
			spiltNode(x, nChild);
			
			nParent = root;
			for (j = 0; j < i; j++) {
				nParent = nParent->sNode[path[j]];			
			}
			nParent->sNode[nParent->count + 1] = nParent->sNode[nParent->count];
			for (j = nParent->count; j > path[i]; j--) {
				nParent->key[j] = nParent->key[j - 1];
				nParent->bNum[j] = nParent->bNum[j - 1];
				//nParent->sNode[j + 1] = nParent->sNode[j];
				nParent->sNode[j] = nParent->sNode[j - 1];
			}
			nParent->leaf = false;
			nParent->key[path[i]] = middleKey;
			nParent->bNum[path[i]] = middleBNum;
			nParent->sNode[path[i]] = x;
			nParent->sNode[path[i] + 1] = nChild;
			nParent->count++;
		}
		i++;
		if (path[i] != -1)
				x = x->sNode[path[i]];
	}
}

void BpTree::spiltNode(BpTreeNode*x, BpTreeNode* y) {
	int i;
	
	y->leaf = x->leaf;
	y->sNode[0] = x->sNode[minNum];
	y->next = x->next;
	for (i = maxNum - 1; i > minNum - 1; i--) {
			y->key[i - minNum] = x->key[i];
			y->bNum[i - minNum] = x->bNum[i];
			y->sNode[i - minNum + 1] = x->sNode[i + 1];
			x->key[i] = 0;
			x->bNum[i] = -1;
			x->sNode[i + 1] = NULL;
			x->count--;
			y->count++;
	}
	
	x->next = y;
}

void BpTree::insert(double a, int b) {
	int i, j = 0;
	BpTreeNode* x;

	x = root;
	if (x == NULL) {
		x = new BpTreeNode;
		x->key[0] = a;
		x->bNum[0] = b;
		x->count++;
		root = x;
	}
	else {
		for (i = 0; i < 4; i++) {
			path[i] = -1;
		}

		// find path to insert
		findPath(x, a);

		// split nodes in path
		arrangePath(x);

		// go through the path and insert data
		x = root;
		if (x->leaf) {
			x->key[x->count] = a;
			x->bNum[x->count] = b;
			x->sort();
			x->count++;
		}
		else {
			while (!x->leaf) {
				if (a < x->key[0])
					x = x->sNode[0];
				else if (a >= x->key[x->count - 1])
					x = x->sNode[x->count];
				else {
					for (i = 0; i < x->count - 1; i++) {
						if ((a >= x->key[i]) && (a < x->key[i + 1])) {
							x = x->sNode[i + 1];
							break;
						}
					}
				}
			}
			x->key[x->count] = a;
			x->bNum[x->count] = b;
			x->sort();
			x->count++;
		}
	}
}

void BpTree::findPath(BpTreeNode* x, double a) {
	int i, j = 0, k = 0;
	
	while (k != depth) {
		if (a < x->key[0]) {
			path[j++] = 0;
			x = x->sNode[0];
		}
		else if (a >= x->key[x->count - 1]) {
			path[j++] = x->count;
			x = x->sNode[x->count];
			
		}
		else {
			for (i = 0; i < x->count - 1; i++) {
				if ((a >= x->key[i]) && (a < x->key[i + 1])) {
					path[j++] = i + 1;
					x = x->sNode[i + 1];
					break;
				}
			}
		}
		k++;
	}

}

void BpTree::update(double a, int oldBnum, int newBNum) {
	int i;
	BpTreeNode* x = root;

	if (x == NULL)
		return;
	else {
		while (x->leaf == false) {						// find leafnode
			if (a < x->key[0])
				x = x->sNode[0];
			else if (a >= x->key[x->count - 1])
				x = x->sNode[x->count];
			else {
				for (i = 0; i < (x->count - 1); i++) {
					if ((a >= x->key[i]) && (a < x->key[i + 1])) {
						x = x->sNode[i + 1];
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
	BpTreeNode* x = root;

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