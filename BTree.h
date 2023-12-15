#include <iostream>
#include <iomanip>
#include <queue>
using namespace std;


struct Directory {
	string path;
	Directory* next;
	Directory() {
		next = NULL;
	}
};

class BTreeNode {
public:
	int* keys; //N size
	Directory* paths;
	int size; //varies
	int currsize;
	int min;
	BTreeNode* parent;
	BTreeNode** child;
	bool isLeaf;

	//------------------ FUNCTIONS -----------------------//
	BTreeNode() {
		size = 4; // Default (?)
		min = (size + 1) / 2;
		keys = new int[size - 1];
		paths = new Directory[size]; // Adjusted to size
		child = new BTreeNode * [size];
		parent = nullptr;
		isLeaf = true;
		currsize = 0;
	}

	BTreeNode(int n) {
		size = n;
		min = (size + 1) / 2;
		keys = new int[size - 1] {0};
		paths = new Directory[size]; // Adjusted to size
		child = new BTreeNode * [size];
		currsize = 0;
	}

	void traverse(){
		int i;
		for (i = 0; i < currsize; i++)
		{
			if (isLeaf == false)
				child[i]->traverse();
			cout << " " << keys[i];
		}
		if (isLeaf == false) {
			child[i]->traverse();
		}
	}

	bool Search(int x)
	{
		int i;
		bool check = false;
		for (i = 0; i < currsize; i++)
		{
			if (keys[i] == x)
				return true;
			if (isLeaf == false)
				check = child[i]->Search(x);

			if (check)
				return true;
		}

		if (isLeaf == false) {
			check = child[i]->Search(x);
			if (check)
				return true;
		}
		return check;
	}

	void Not_full(int k, string p) {
		int i = currsize - 1;

		if (isLeaf == true) {
			while (i >= 0 && keys[i] > k) {
				keys[i + 1] = keys[i];
				paths[i + 1] = paths[i];
				i--;
			}
			if (keys[i] == k) {
				paths[i].next = new Directory();
				paths[i].next->path = p;
			}
			else {
				keys[i + 1] = k;
				paths[i + 1].path = p;
				currsize = currsize + 1;
			}
		}
		else {
			while (i >= 0 && keys[i] > k) {
				i--;
			}

			if (child[i + 1]->currsize == size) {
				splitChild(i + 1, child[i + 1]);

				if (keys[i + 1] < k) {
					i++;
				}
			}

			child[i + 1]->Not_full(k, p);
		}
	}

	void splitChild(int i, BTreeNode* Node) {

		BTreeNode* temp = new BTreeNode(Node->size);
		temp->isLeaf = Node->isLeaf;
		temp->currsize = min;

		for (int j = 0; j < min; j++) {
			temp->keys[j] = Node->keys[j + min];
			temp->paths[j] = Node->paths[j + min];
		}

		if (Node->isLeaf == false) {
			for (int j = 0; j < min + 1; j++) {
				temp->child[j] = Node->child[j + min];
			}
		}

		Node->currsize = min - 1;

		for (int j = currsize; j >= i + 1; j--) {
			child[j + 1] = child[j];
		}

		child[i + 1] = temp;

		for (int j = currsize - 1; j >= i; j--) {
			keys[j + 1] = keys[j];
			paths[j + 1] = paths[j];
		}
		

		keys[i] = Node->keys[min - 1];
		paths[i] = Node->paths[min - 1];

		currsize = currsize + 1;
	}

	int KeyIndx(int key) {
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
					paths[n - 1] = paths[n];
					n++;
				}
				currsize--;
				return;
			}
			else {
				int toRemove = keys[indx];

				if (child[indx]->currsize >= min) { //case 1, k precedes
					int pred = getPredecessor(indx);
					string predpath = getPredecessorPath(indx);
					keys[indx] = pred;
					paths[indx].path = predpath;

					child[indx]->removeKey(pred);
				}
				else if (child[indx + 1]->currsize >= min) { //case 2, k succeeds
					int succ = getSuccessor(indx);
					string succpath = getSuccessorPath(indx);
					paths[indx].path = succpath;
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

			bool check = false;
			if (indx == currsize)
			{
				check = true;
			}

			if (child[indx]->currsize < min) {
				fillNode(indx);
			}

			if (indx > currsize && check)
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
		c->paths[min - 1] = paths[indx];

		if (!c->isLeaf) {
			for (int i = 0; i <= s->currsize; i++) {
				c->child[i + min] = s->child[i];
			}
		}

		for (int i = 0; i < s->currsize; i++) {
			c->keys[i + min] = s->keys[i];
			c->paths[i + min] = s->paths[i];
		}

		for (int i = indx + 1; i < currsize; i++) {
			keys[i - 1] = keys[i];
			paths[i - 1] = paths[i];
		}

		for (int i = indx + 2; i <= currsize; i++)
			child[i - 1] = child[i];

		c->currsize += s->currsize;
		delete s;

		return;
	}

	int getPredecessor(int indx) {
		BTreeNode* curr = child[indx];
		while (!curr->isLeaf) {
			curr = curr->child[curr->currsize];
		}
		return curr->keys[curr->currsize - 1];
	}

	string getPredecessorPath(int indx) {
		BTreeNode* curr = child[indx];
		while (!curr->isLeaf) {
			curr = curr->child[curr->currsize];
		}
		return curr->paths[curr->currsize - 1].path;
	}

	int getSuccessor(int indx) {
		BTreeNode* curr = child[indx];
		while (!curr->isLeaf) {
			cout << "p" << endl;
			curr = curr->child[0];
		}
		return curr->keys[0];
	}

	string getSuccessorPath(int indx) {
		BTreeNode* curr = child[indx];
		while (!curr->isLeaf) {
			cout << "p" << endl;
			curr = curr->child[0];
		}
		return curr->paths[0].path;
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

	void borrowfromLeftSibling(int indx) {
		for (int i = child[indx]->currsize - 1; i >= 0; i--) { //moving keys one index ahead (to free space for the borrowed one at 0th index)
			child[indx]->keys[i + 1] = child[indx]->keys[i];
			child[indx]->paths[i + 1] = child[indx]->paths[i];
		}
		child[indx]->keys[0] = keys[indx - 1]; //adding parent value at the 0th index
		child[indx]->paths[0] = paths[indx - 1];

		if (!child[indx]->isLeaf) {
			for (int i = child[indx]->currsize; i >= 0; i--) { //if it ain't a leaf, moving all child pointer one step ahead too
				child[indx]->child[i + 1] = child[indx]->child[i];
			}
			child[indx]->child[0] = child[indx - 1]->child[child[indx - 1]->currsize];
		}

		keys[indx - 1] = child[indx - 1]->keys[child[indx - 1]->currsize - 1]; //replacing the parent with the key from sibling (the one borrowed from)
		paths[indx - 1] = child[indx - 1]->paths[child[indx - 1]->currsize - 1];
		child[indx]->currsize++; //the filled node now has one more value
		child[indx - 1]->currsize--; //the node from which the value is borrowed now has one less
	}

	void borrowfromRightSibling(int indx) {
		child[indx]->keys[child[indx]->currsize] = keys[indx]; //adding parent value at the last index
		child[indx]->paths[child[indx]->currsize] = paths[indx];

		if (!child[indx]->isLeaf) {
			child[indx]->child[child[indx]->currsize + 1] = child[indx + 1]->child[0];
		}


		keys[indx] = child[indx - 1]->keys[0]; //replacing the parent with the key from sibling (the one borrowed from)
		paths[indx] = child[indx - 1]->paths[0];

		for (int i = 1; i < child[indx + 1]->currsize; ++i) {//moving keys behind to fill the gap (in the one borrowed from)
			child[indx + 1]->keys[i - 1] = child[indx + 1]->keys[i];
			child[indx + 1]->paths[i - 1] = child[indx + 1]->paths[i];
		}
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

	BTree(int size = 8)
	{
		this->size = size;
		root = nullptr;
	}

	void insert(int k, const string& directory)
	{
		if (root == NULL)
		{
			root = new BTreeNode(size);
			root->isLeaf = true;
			root->paths[0].path = directory;
			root->keys[0] = k;
			root->currsize = 1;
		}
		else 
		{

			if (root->currsize == size)
			{
				BTreeNode* s = new BTreeNode(size);
				s->isLeaf = false;
				s->child[0] = root;

				s->splitChild(0, root);


				int i = 0;
				if (s->keys[0] < k)
				{
					i++;
				}
				s->child[i]->Not_full(k, directory);
				root = s;
			}
			else
			{
				root->Not_full(k, directory);
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
			BTreeNode* root_cpy = root;
			if (!root->isLeaf)
				root = root->child[0]; 
			else
				root = NULL;

			delete root_cpy;
		}
		return;
	}

	void getValuesLess(int rangeDigit, stack<long long int>& values, stack<string>& paths) {
		if (root != nullptr) {
			getValuesLess(root, rangeDigit, values, paths);
		}
	}

	void getValuesLess(BTreeNode* node, int rangeDigit, stack<long long int>& values, stack<string>& paths) {
		if (node != nullptr) {
			int i;
			for (i = 0; i < node->currsize; i++) {
				if (!node->isLeaf) {
					getValuesLess(node->child[i], rangeDigit, values, paths);
				}
				if (node->keys[i] < rangeDigit) {
					values.push(node->keys[i]);
					paths.push(node->paths[i].path);
					
				}
				else {
					break; 
				}
			}

			for (int j = 0; j < i; ++j) {
				node->removeKey(node->keys[j]);
			}

			if (!node->isLeaf) {
				getValuesLess(node->child[i], rangeDigit, values, paths);
			}
		}
	}

	void getAllValues(stack<long long int>& values, stack<string>& paths) {
		if (root != nullptr) {
			getAllValues(root, values, paths);
		}
	}

	void getAllValues(BTreeNode* node, stack<long long int>& values, stack<string>& paths) {
		if (node != nullptr) {
			int i;
			for (i = 0; i < node->currsize; i++) {
				if (!node->isLeaf) {
					getAllValues(node->child[i], values, paths);
				}
				values.push(node->keys[i]);
				paths.push(node->paths[i].path);
			}

			if (!node->isLeaf) {
				getAllValues(node->child[i], values, paths);
			}

			for (int j = 0; j < i; ++j) {
				node->removeKey(node->keys[j]);
			}
		}
	}

	void traverse()
	{
		if (root != NULL) root->traverse();
	}

	bool search(int x)
	{
		bool y;
		if (root != NULL)
			y = root->Search(x);

		return x;
	}

};

void rootCheck(BTree* &b) {
	if (b->root->currsize == 0)
		b->root = NULL;
}

void printTree(BTreeNode* node, int level) {
	if (node != nullptr) {
		cout << std::string(level * 4, ' ');

		for (int i = 0; i < node->currsize; ++i) {
			cout << node->keys[i] << " ";
		}
		cout << endl;

		if (!node->isLeaf) {
			for (int i = 0; i <= node->currsize; ++i) {
				printTree(node->child[i], level + 1);
				if (i < node->currsize)
					cout << std::string((level + 1) * 4, ' ') << "| ";
			}
		}
	}
}

void printBTree(BTree* btree) {
	int lvl = 0;
	if (btree != nullptr && btree->root != nullptr) {
		printTree(btree->root, lvl);
	}
	else {
		cout << "The B-tree is empty." << endl;
	}
}

bool SearchHelper(BTreeNode* node, int key) {
	if (node != nullptr) {
		for (int i = 0; i < node->currsize; ++i) {
			if (node->keys[i] == key) {
				return true;
			}
		}
		bool x = false;
		if (!node->isLeaf) {
			for (int i = 0; i <= node->currsize; ++i) {
				x = SearchHelper(node->child[i], key);
				if (x == true)
					break;
			}
		}
		return x;
	}
}

void SearchBTree(BTree btree, int key) {
	if (SearchHelper(btree.root, key)) {
		cout << "Element Found\n";
	}
	else {
		cout << "Element Not Found\n";
	}
}

void getPathHelper(BTreeNode* node, int key, stack<string> &paths) {
	if (node != nullptr) {
		for (int i = 0; i < node->currsize; ++i) {
			if (node->keys[i] == key) {
				Directory* current = &node->paths[i];
				do {
					paths.push(current->path);
					current = current->next;

				} while (current);
			}
		}
		bool x = false;
		if (!node->isLeaf) {
			for (int i = 0; i <= node->currsize; ++i) {
				x = SearchHelper(node->child[i], key);
				if (x == true)
					break;
			}
		}
	}
}

void getPath(BTree btree, int key, stack<string> &paths) {
	if (SearchHelper(btree.root, key)) {
		getPathHelper(btree.root, key, paths);
	}
	else {
		cout << "> Key not found" << endl;
		return;
	}
}
