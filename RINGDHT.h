#pragma once

#include <iostream>
#include <time.h>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <stack>
#include <string>
#include <string>

#include "BTree.h"
#include "Hashing.h"
using namespace std;

struct value {
	string content; //content in case of a machine is its name (eg, google.com)
	string directory;
};

class Node {
public:
	int key; //hash value
	value value; //actual content of whatever txt file
	/*
	Machine -> hashed value generated from its name i.e google.com
	Data -> hashed value generated from its content i.e txt file content
	*/
	BTree btree;
	bool isMachine;
	Node* next;
	Node* prev;

	Node() {
		btree = NULL;
		isMachine = false;
		key = 0; value.content = '0';
		value.directory = '0';
	}

	Node(int k, string val, bool flag) { //just in case needed
		btree = NULL;
		isMachine = flag;
		key = k; value.content = val;
	}
};

class RingDHT {
public:
	int identifier_space;
	int NumberOfMachines;
	Node* head;

	RingDHT() { //is == identifier space (default 16)
		string choice;
		cout << "> Do you want to specify the identifier space? YES or NO" << endl;
		cin >> choice;
		if (choice == "yes" || choice == "YES") {
			cout << "> Enter:";
			cin >> identifier_space;
		}
		else
			identifier_space = 4;
		srand(time(0));
		cout << "\n> Do you want to specify the Number of Machines? YES or NO" << endl;
		cin >> choice;
		if (choice == "yes" || choice == "YES") {
			int flag = 0;
			do
			{
				cout << "> Enter the Number:";
				cin >> NumberOfMachines;
				if (NumberOfMachines > pow(2, identifier_space)) {
					cout << "> Number exceeds the Maximum value, enter again" << endl;
					flag = 0;
				}
				else
					flag = 1;
			} while (!flag);

		}
		else
			NumberOfMachines = identifier_space + 1; // default number

		createDHT(identifier_space, NumberOfMachines);

	}

	void createDHT(int idspace, int machines);

	void showNodes(); //displays each node and its flag (machine or nah)

	void randomizeMachines();

	void manualAssignMachines();

	void insertFile();

	string getRandomName();

	void insertMachine();

	void deleteMachine();

	void removeFile();

	void showDirectories();


};

void RingDHT::createDHT(int idspace, int machines) {
	int total = pow(2, idspace);
	cout << "-------------------------------------" << endl;
	cout << "Identifier Space: " << idspace << "(" << total << ")" << endl;
	cout << "Total Machines: " << machines << endl;
	cout << "-------------------------------------" << endl;
	Node* newNode = new Node; head = newNode;
	Node* current = head;
	int count = 0;
	while (count < total) {
		cout << "= Adding: " << count << endl;
		Node* newNode = new Node;
		current->next = newNode;
		current = current->next;
		count++;
	}
	current->next = head; //circular
	total--; //starts from 0 (less one)
	int c;
	cout << "> Do you want to Automatically or Manually assign IDs to Machines? 1. Automatically :: 2. Manually" << endl;
	cin >> c;
	if (c == 1)
		randomizeMachines();
	else
		manualAssignMachines();


	showNodes();

	cout << "Ring DHT Setup Successful, Proceeding..." << endl;
	return;
}

void RingDHT::showNodes() {
	cout << "\nNodes: " << endl;
	cout << "--------------------------------" << endl;
	Node* curr = head; int count = 0;
	while (curr->next != head) {
		cout << count << " :: ";
		if (curr->isMachine) {
			cout << "Machine" <<" >> "<<curr->value.content<< endl;
		}
		else {
			cout << "Data" << endl; //will later show status too (if the node is empty or nah)
		}


		curr = curr->next; count++;
	}
	cout << "--------------------------------" << endl;
}

void RingDHT::randomizeMachines() {
	int total = pow(2, identifier_space);
	cout << "\n> Randomizing Machines . . ." << endl << endl;
	int m = total / NumberOfMachines;
	int i = -1;
	Node* current = nullptr;
	int machines_installed = 0;

	while (i < total){
		int j = i + m;
		while (i < j) {
			if (current)
				current = current->next;
			else
				current = head;
			i++;
		}
		current->value.content = getRandomName();
		current->isMachine = true;
		Node* curr = head; int count = 0;
		while (curr != current) {
			count++;
			curr = curr->next;
		}
		current->key = count;
		machines_installed++;
		if (machines_installed == NumberOfMachines){
			break;}
	}
}

void RingDHT::manualAssignMachines() {
	int total = pow(2, identifier_space);
	cout << "\n> Assign Manual IDs . . ." << endl;
	int m = 1; int assign = 0;
	while (m <= NumberOfMachines) {
		cout << "Machine_" << m << " :: Assign ID: ";//ASSIGN IDS THRU NAME??????????????????
		cin >> assign;
		Node* current = head; int i = 0;
		while (i < assign) {
			current = current->next;
			i++;
		}
		current->value.content = getRandomName();
		current->key = assign;
		
		current->isMachine = true;
		m++;
	}
}

void RingDHT::insertFile() {
	string path; bool flag = 1;
	cout << "Inserting File..." << endl;
	ifstream file;
	do {
		flag = 1;
		cout << "> Enter the File Directory: " << endl;
		cin.ignore(1);
		getline(cin, path);

		if (path == "exit" || path == "Exit")
			return;
		file.open(path);
		if (!file) {
			cout << "Cannot open the file, enter again." << endl;
			flag = 0;
		}
	} while (!flag);
	int i = 0;
	while (path[i + 1] != '\0')
		i++;
	string name;
	stack<char> getName;
	while (path[i] != '\\' && i > 0) {
		getName.push(path[i]);
		i--;
	}
	i = 0;
	while (!getName.empty()) {
		name += getName.top();
		getName.pop();
		if (getName.top() == '.')
			break;
	} //name will have the file name (in case if needed later though the hash will be generated thru content)
	string fileContent;
	string line;

	while (getline(file, line)) {
		fileContent += line; // Concatenate each line
	}
	string choice;
	cout << "> Show Content (File :" << name << ") ? YES or NO" << endl;
	cin >> choice; cout << endl;
	if (choice == "yes" || choice == "YES") {
		cout << ">>";
		cout << fileContent << endl << endl;
	}
	int total = pow(2, identifier_space);
	long long int fileKey = getHash(fileContent);
	fileKey = fileKey % total;

	cout << "> Storing on Node_" << fileKey << endl;

	Node* current = head; int indx = 0;
	while ((indx++) < fileKey)
		current = current->next;

	current->key = fileKey; 
	current->value.content = fileContent;
	current->value.directory = path; 

	while (!current->isMachine) {
		current = current->next;
	}

	cout << "> Managed by Machine_\"" << current->value.content << "\"" << endl;
	//current->btree.insert(fileKey); //store value where <?>
	//insert in BTree of THAT Machine NEXT (current)

	//done after that

}

string RingDHT::getRandomName() {
	string list[20] = { "hub","tech","code","MCL (Maria Cooperation Limited)", "Zirwah K Organization", "One Piece", "FTN", "Lava Swimmer", "Pastry", "Mario", "DataStr", "TechComp", "AOT", "TR", "SanjiLimited", "Yowaimo", "Vinland", "FASTNUCES", "ClockServices", "WebNovel" };
	string returnName = list[rand() % 20];

	return returnName;
}

void RingDHT::insertMachine() {
	string key;
	cout << "\n> Enter Machine Name: ";
	cin >> key;
	int total = pow(2, identifier_space);
	if ((NumberOfMachines + 1) > pow(2, identifier_space)) {
		cout << "> Cannot insert, Maximum Number reached" << endl;
		return;
	}
	long long int hash = getHash(key);
	hash = hash & total;
	int i = 0; Node* current = head;
	while (i < hash) {
		current = current->next;
		i++;
	}

	if (current->isMachine) {
		cout << "> Cannot insert, Machine already exists" << endl;
		return;
	}

	if (current->key) {
		//INSERT IN BTREE IF DATA IS STORED ALREADY
	}
	current->isMachine = true;
	current->key = hash;
	current->value.content = key;

	cout << "> Machine\"" << key << "\" inserted Successfully" << endl;
	cout << "Proceeding . . ." << endl << endl;
	return;
}

void RingDHT:: deleteMachine() {
	cout << "> Enter Machine Key: " << endl;
	int key;
	cin >> key;
	bool found = false;
	// machine becomes actual node<?>
	Node* current=head;
	while (current->next != head) {
		current = current->next;
		if (current->key == key) {
			found = true;
			break;
		}
	}
	if (!found) {
		cout << "> The machine does not exist." << endl;
		return;
	}
	if (!current->btree.root) {
		current->isMachine = false;
		current->key = 0;
		current->value.content = '0';
		cout << "> Machine \"" << current->value.content << "\" removed successfully" << endl;

		return;
	}
	else {
		//traversal of btree
		int* keysToAdd = {};
	}

}

void RingDHT::showDirectories() {
	Node* current = head; bool flag = false;
	//for now i'll just traverse thru the RingDHT, will change it to Btree traversing later (as the paths will be stored in btree)
	while (current->next != head) {
		if (current->key && !current->isMachine) {
			cout << "> " << current->value.directory << endl;
			flag = true;
		}
		current = current->next;
		if (!flag)
			cout << "> No File in the System" << endl;
	}
}

void RingDHT::removeFile() {
	string choice;
	cout << "> Enter the content of the file to be deleted: " << endl;
	cin >> choice;
	//to be deleted using the content? name? directory?

}






//Left:
/*
-> BTree Insertion & Deletion (Implementation is Done, but inserting actual keys/directories left)
-> Routing tables
-> Searching (will be possible after routing tables are implemented)
-> some functions i made are incomplete, will complete em too
*/
