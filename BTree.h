#include <iostream>
#include <iomanip>
#include <queue>
using namespace std;

class BTreeNode {
public:
	int* keys; //N size
	int size; //varies
	int currsize;
	int min;
	BTreeNode* parent;
	BTreeNode** child;
	bool isLeaf;


	BTreeNode() {
		size = 16; //default (?)
		min = (16/2)-1;
		keys = new int[size - 1];
		child = new BTreeNode * [size];
		parent = NULL;
		isLeaf = true;
		currsize = 0;
	}

	BTreeNode(int n) {
		size = n;
		min = (n) / 2;
		keys = new int[size - 1];
		child = new BTreeNode * [size];
		currsize = 0;
	}
	void Not_full(int k)
	{
		int i = currsize - 1;

		if (isLeaf == true)
		{
			while (i >= 0 && keys[i] > k)
			{
				keys[i + 1] = keys[i];
				i--;
			}


			keys[i + 1] = k;
			currsize = currsize + 1;
		}
		else // If this node is not isLeaf
		{
			// child which will have new key
			while (i >= 0 && keys[i] > k)
			{
				i--;
			}

			// codition to check if child is full
			if (child[i + 1]->currsize == size - 1)
			{
				BTreeNode* temp = new BTreeNode(child[i + 1]->size);
				temp->isLeaf = child[i + 1]->isLeaf;
				temp->currsize = min - 1;

				for (int j = 0; j < min - 1; j++)
					temp->keys[j] = child[i + 1]->keys[j + min];

				if (child[i + 1]->isLeaf == false)
				{
					for (int j = 0; j < min; j++)
						temp->child[j] = child[i + 1]->child[j + min];
				}

				child[i + 1]->currsize = min - 1;


				for (int j = currsize; j >= i + 2; j--)
				{
					child[j + 1] = child[j];		//space for new child
				}


				child[i + 1] = temp;


				for (int j = currsize - 1; j >= i + 1; j--)
					keys[j + 1] = keys[j];


				keys[i + 1] = child[i + 1]->keys[min - 1];

				currsize = currsize + 1;



				if (keys[i + 1] < k)
				{
					i++;
				}
			}
			child[i + 1]->Not_full(k);
		}
	}

	void splitChild(int i, BTreeNode* Node)
	{
		BTreeNode* temp = new BTreeNode(Node->size);
		temp->isLeaf = Node->isLeaf;
		temp->currsize = min - 1;

		for (int j = 0; j < min - 1; j++)
			temp->keys[j] = Node->keys[j + min];

		if (Node->isLeaf == false)
		{
			for (int j = 0; j < min; j++)
				temp->child[j] = Node->child[j + min];
		}
		Node->currsize = min - 1;

		for (int j = currsize; j >= i + 1; j--)
			child[j + 1] = child[j];

		child[i + 1] = temp;

		for (int j = currsize - 1; j >= i; j--)
			keys[j + 1] = keys[j];

		keys[i] = Node->keys[min - 1];

		currsize = currsize + 1;
	}



	int KeyIndx(int key) { //find key
		int indx = 0;
		while (indx < currsize && keys[indx] < key) {
			indx++;
		}
		return indx;
	}

	bool isKey(int k) { //checks if the provided key is in the current node or nah
		int indx = 0;

		while (indx < currsize) {
			if (keys[indx] == k)
				return true;
			indx++;
		}
		return false;
	}

	void removeKey(int k) {
		int indx = KeyIndx(k);
		if (isKey(k)) { //present in this node 
			if (isLeaf) {
				int n = indx + 1;
				while (n < currsize) {
					keys[n - 1] = keys[n];
					n++;
				}
				currsize--;
				return;
			}
			else {
				int toRemove = keys[indx];

				if (child[indx]->currsize >= min) { //case 1, k precedes
					int pred = getPredecessor(indx);
					keys[indx] = pred;
					child[indx]->removeKey(pred);
				}
				else if (child[indx + 1]->currsize >= min) { //case 2, k succeeds
					int succ = getSuccessor(indx);
					keys[indx] = succ;
					child[indx + 1]->removeKey(succ);
				}
				else { //neither
					mergeNodes(indx);
					child[indx]->removeKey(toRemove);
				}
				return;
			}
		}
		else {
			//internal nodes
			if (isLeaf) {
				//the key does not exist in the current tree
				return;
			}

			if (child[indx]->currsize < min) {
				child[indx]->fillNode(indx);
			}

			if (indx == currsize)
				child[indx - 1]->removeKey(k);
			else
				child[indx]->removeKey(k);

		}
		return;

	}

	void mergeNodes(int indx) {
		BTreeNode* c = child[indx];
		BTreeNode* s = child[indx + 1];

		c->keys[min - 1] = keys[indx];

		if (!c->isLeaf) {
			for (int i = 0; i <= s->currsize; i++) {
				c->child[i + min] = s->child[i];
			}
		}

		for (int i = 0; i < s->currsize; i++) {
			c->keys[i + min] = s->keys[i];
		}

		for (int i = indx + 1; i < currsize; i++) {
			keys[i - 1] = keys[i];
		}

		for (int i = indx + 2; i <= currsize; i++)
			child[i - 1] = child[i];

		c->currsize += s->currsize;
		delete s;

		return;
	}

	int getPredecessor(int indx) {
		BTreeNode* curr = child[indx];
		while (!child[indx]->isLeaf) {
			curr = curr->child[curr->currsize];
		}
		return curr->keys[curr->currsize - 1];
	}

	int getSuccessor(int indx) {
		BTreeNode* curr = child[indx];
		while (!child[indx]->isLeaf) {
			cout << "p" << endl;
			curr = curr->child[0];
		}
		return curr->keys[0];
	}

	void fillNode(int indx) {
		if (indx != 0 && child[indx - 1]->currsize >= min)
			borrowfromLeftSibling(indx);
		else if (indx != currsize && child[indx + 1]->currsize >= min)
			borrowfromRightSibling(indx);
		else {
			if (indx != currsize)
				mergeNodes(indx);
			else
				mergeNodes(indx - 1);
		}
	}
	//adding more comments (above) later for better understanding, cope with it for now 
	void borrowfromLeftSibling(int indx) {
		for (int i = child[indx]->currsize - 1; i >= 0; i--) { //moving keys one index ahead (to free space for the borrowed one at 0th index)
			child[indx]->keys[i + 1] = child[indx]->keys[i];
			child[indx]->keys[0] = keys[indx - 1]; //adding parent value at the 0th index
		}

		if (!child[indx]->isLeaf) {
			for (int i = child[indx]->currsize; i >= 0; i--) { //if it ain't a leaf, moving all child pointer one step ahead too
				child[indx]->child[i + 1] = child[indx]->child[i];
			}
			child[indx]->child[0] = child[indx - 1]->child[child[indx - 1]->currsize];
		}

		keys[indx - 1] = child[indx - 1]->keys[child[indx - 1]->currsize - 1]; //replacing the parent with the key from sibling (the one borrowed from)
		child[indx]->currsize++; //the filled node now has one more value
		child[indx - 1]->currsize--; //the node from which the value is borrowed now has one less
	}

	void borrowfromRightSibling(int indx) {
		child[indx]->keys[child[indx]->currsize] = keys[indx]; //adding parent value at the last index

		if (!child[indx]->isLeaf) {
			child[indx]->child[child[indx]->currsize + 1] = child[indx + 1]->child[0];
		}


		keys[indx] = child[indx - 1]->keys[0]; //replacing the parent with the key from sibling (the one borrowed from)

		for (int i = 1; i < child[indx + 1]->currsize; ++i) //moving keys behind to fill the gap (in the one borrowed from)
			child[indx + 1]->keys[i - 1] = child[indx + 1]->keys[i];
		if (!child[indx + 1]->isLeaf)
		{
			for (int i = 1; i <= child[indx + 1]->currsize; ++i)
				child[indx + 1]->child[i - 1] = child[indx + 1]->child[i];
		}
		child[indx]->currsize++; //the filled node now has one more value
		child[indx - 1]->currsize--; //the node from which the value is borrowed now has one less
	}
};

class BTree {
public:
	BTreeNode* root;
	int size;

	BTree(int size = 16)
	{
		this->size = size;
		root = nullptr;
	}
	void insert(int k)
	{

		if (root == NULL)
		{
			root = new BTreeNode(size);
			root->isLeaf = true;
			root->keys[0] = k;
			root->currsize = 1;
		}
		else // If tree is not empty
		{

			if (root->currsize == root->min - 1)
			{
				BTreeNode* s = new BTreeNode(size);
				s->isLeaf = false;
				s->child[0] = root;

				s->splitChild(0, root);


				int i = 0;
				if (s->keys[0] < k)
					i++;
				s->child[i]->Not_full(k);
				root = s;
			}
			else
			{
				root->Not_full(k);
			}
		}
	}




	void remove(int key)
	{
		if (root == NULL) {
			//empty
			return;
		}
		root->removeKey(key);

		if (root->currsize == 0) {
			if (!root->isLeaf)
				root = root->child[0]; //make the first child root(can be null)
			else
				root = NULL;
		}
		return;
	}

};

