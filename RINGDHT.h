#pragma once

#include <iostream>
#include <chrono>
#include <thread>
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

//utility functions
void PressEnterToProceed() {
	cout << "Press any key to Proceed..." << endl;
	_getch();
	system("cls");
}

void sleepFor(int seconds) {
	this_thread::sleep_for(chrono::seconds(seconds));
}

struct value {
	string content; //content in case of a machine is its name (eg, google.com)
	string directory;
};

class Node {
public:
	long long int key; //hash value
	value value; //actual content of whatever txt file
	/*
	Machine -> hashed value generated from its name i.e google.com
	Data -> hashed value generated from its content i.e txt file content
	*/
	int indx;
	BTree btree;
	bool isMachine;
	Node* next;
	Node* prev;
	Node** TableArr = nullptr;
	Node() {
		isMachine = false;
		key = 0; value.content = '0';
		value.directory = '0';
		indx = 0;
	}

	Node(int k, string val, bool flag) {   //just in case needed

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
		cout << endl;
		cout << "========================================================================================================================" << endl << endl;
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

	void CreateTable();

	Node* FindTableElements(Node* curr);

	void PrintTable(Node* n1);

	Node* SearchingFromTable(int start, int find);

	Node* Recurrsion(Node* Start, int find);

	void PrintOnlyOne();

};
void RingDHT::createDHT(int idspace, int machines) {
	int total = pow(2, idspace);
	cout << "-------------------------------------" << endl;
	cout << "Identifier Space: " << idspace << "(" << total << ")" << endl;
	cout << "Total Machines: " << machines << endl;
	cout << "-------------------------------------" << endl;
	Node* newNode = new Node; newNode->indx = 0;
	head = newNode;
	Node* current = head;
	int count = 1;
	while (count < total) {
		newNode = new Node; // Use the existing variable
		newNode->indx = count;
		current->next = newNode;
		newNode->prev = current;
		current = current->next;
		count++;
	}
	current->next = head; // Circular
	head->prev = current;
	total--; //starts from 0 (less one)
	int c;
	cout << "> Do you want to Automatically or Manually assign IDs to Machines? 1. Automatically :: 2. Manually" << endl;
	cin >> c;
	system("cls");
	if (c == 1)
		randomizeMachines();
	else
		manualAssignMachines();

	CreateTable();
	showNodes();

	cout << "Ring DHT Setup Successful, Proceeding..." << endl;
	this_thread::sleep_for(chrono::seconds(2));
	system("cls");
	return;
}

void RingDHT::showNodes() {
	system("cls");
	cout << "\nNodes: " << endl;
	cout << "--------------------------------" << endl;
	Node* curr = head; int count = 0;
	do
	{
		cout << count << " :: ";
		if (curr->isMachine) {
			cout << "Machine" << " >> " << curr->value.content << endl;
		}
		else {
			cout << "Data" << endl; //will later show status too (if the node is empty or nah)
		}
		curr = curr->next; count++;
	} while (curr != head);

	cout << "--------------------------------" << endl;
}

void RingDHT::randomizeMachines() {
	int total = pow(2, identifier_space);
	cout << "\n> Randomizing Machines . . ." << endl << endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	int m = total / NumberOfMachines;
	int i = -1;
	Node* current = nullptr;
	int machines_installed = 0;

	while (i < total) {
		int j = i + m;
		while (i < j) {
			if (current)
				current = current->next;
			else
				current = head;
			i++;
		}
		if (current) {
			current->value.content = getRandomName();
			current->isMachine = true;
		}
		Node* curr = head; int count = 0;
		while (curr != current) {
			count++;
			curr = curr->next;
		}
		current->key = count;
		machines_installed++;
		if (machines_installed == NumberOfMachines) {
			break;
		}
	}
	cout << "Proceeding..." << endl;
}

void RingDHT::manualAssignMachines() {
	int total = pow(2, identifier_space);
	bool aFlag = false;
	cout << "\n> Assign Manual IDs . . ." << endl << endl;
	int m = 1; int assign = 0;
	Node* current;
	while (m <= NumberOfMachines) {
		do {
			aFlag = false;
			cout << "Machine_" << m << " :: Assign ID: ";
			cin >> assign;
			current = head; int i = 0;
			while (i < assign) {
				current = current->next;
				i++;
			}
			if (current->isMachine) {
				cout << "> ID is already a Machine, Enter again." << endl;
				aFlag = true;

			}
		} while (aFlag);
		current->value.content = getRandomName();
		current->key = assign;

		current->isMachine = true;
		m++;
	}
	system("cls");
}

//File insertion: -> ask user for the starting machine, -> Ask for the file directory, ->generate the sha1 hash and its modulus, ->use routing to get to the machine to be stored in
void RingDHT::insertFile() {
	int machineKey = 0;
	cout << "> Enter the Machine (Key) to insert the File from." << endl;
	cin >> machineKey;
	Node* currentMachine = head; int ii = 0;
	while (ii < machineKey) {
		currentMachine = currentMachine->next;  ii++;
	}
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
	long long int SHAhash = fileKey;
	fileKey = fileKey % total;

	cout << "> Storing on Node_" << fileKey << endl;
	//use of routing to table to get at the designated node
	cout << "------------------------------" << endl;
	Node* currenttMachine = SearchingFromTable(machineKey, fileKey);
	if (!currenttMachine) {
		return;
	}
	cout << "------------------------------" << endl;
	cout << "> Managed by Machine_\"" << currenttMachine->value.content << "\"" <<" [Key: "<<currenttMachine->indx<<"]"<< endl;
	currenttMachine->btree.insert(fileKey, path); //store value where <?>
	cout << "------------------------------------------------------------------" << endl;
	cout << "Insert Successful, Hash:" << fileKey << "::" << SHAhash << endl;
	cout << "--------------------------------------------------------------------" << endl;
	/*cout << "BTree:" << endl;
	currenttMachine->btree.traverse();
	cout << endl << endl;*/ //uncomment this to see the working of the btree (inserted in)
	PressEnterToProceed();
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
	current->isMachine = true;
	current->key = hash;
	current->value.content = key;

	cout << "> Machine\"" << key << "\" inserted Successfully" << endl;
	cout << "Proceeding . . ." << endl << endl;
	return;
}

void RingDHT::deleteMachine() {
	cout << "> Enter Machine Key: " << endl;
	int key;
	cin >> key;
	bool found = false;
	// machine becomes actual node<?>
	Node* current = head;
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
	//for a starter, we'll be randomizing the current machine from which user is searching
	int machineKey = 0;
	cout << "> Enter the Machine (Key) to insert the File from." << endl;
	cin >> machineKey;
	Node* currentMachine = head; int ii = 0;
	while (ii < machineKey) {
		currentMachine = currentMachine->next;  ii++;
	}
	int key;
	cout << "> Current Machine Node: " << machineKey << endl;
	cout << "> Enter the Key of The File to remove: " << endl;
	cin >> key;
	//there may be a possibility that the key doesn't exist (in that case direct to the start)

	//case 1 - current machine contains the key i.e key<=machine_key
	Node* prevMachine = currentMachine->prev;
	while (!prevMachine->isMachine) {
		prevMachine = prevMachine->prev;
	}
	int prevKey = prevMachine->indx;
	//i've not tested this yet (will do later)
	if (key <= currentMachine->indx && key > prevMachine->indx) {
		cout << "> Present in current Machine. . ." << endl;
		currentMachine->btree.remove(key);
	}
	//routing...
	cout << "> Not present in current Machine, Routing to the next machines. . ." << endl;
	Node* designatedMachine = SearchingFromTable(machineKey, key);
	designatedMachine->btree.remove(key);
	cout << "File Removed Successfully." << endl;
	PressEnterToProceed();

}


void RingDHT::PrintTable(Node* curr) {
	cout << "Routing Table for Machine " << curr->key << endl;
	for (int i = 0; i < identifier_space; i++) {
		cout << curr->TableArr[i]->key << "   ";
	}
	cout << endl << endl;
}

Node* RingDHT::FindTableElements(Node* curr) {
	int mul = 1;
	int total = (pow(2, identifier_space));
	curr->TableArr = new Node * [identifier_space];
	for (int i = 0; i < identifier_space; i++) {
		int index = (pow(2, i));
		Node* curr2 = curr;
		while (index > 0) {
			curr2 = curr2->next;
			index--;
		}
		while (curr2->isMachine == false) {
			curr2 = curr2->next;
		}
		curr->TableArr[i] = curr2;
	}
	return curr;
}

void RingDHT::CreateTable() {
	Node* curr = head;
	if (head != nullptr)
		do
		{
			if (curr->isMachine) {
				curr = FindTableElements(curr);
				//PrintTable(curr);
			}
			curr = curr->next;
		} while (curr != head);
		return;
}

Node* RingDHT::Recurrsion(Node* Start, int find) {
	do {
		if (Start->key < find && find < Start->TableArr[0]->key) {
			Start = Start->TableArr[0];
		}
		else {
			for (int i = 0; i < identifier_space; i++) {
				if (i + 1 == identifier_space) {
					Start = Start->TableArr[i];
					break;
				}
				else if (Start->TableArr[i]->key < find && find < Start->TableArr[i + 1]->key) {
					Start = Start->TableArr[i];
					break;
				}
			}
		}
					cout << ">>> Machine_" << Start->indx << endl;
	} while ((Start->key < find));
	//cout << Start->key << ;
	return Start;

}

// This is the main function
Node* RingDHT::SearchingFromTable(int start, int find) {

	Node* Start = head;
	for (int i = 0; i < start; i++) {
		Start = Start->next;
	}

	if (Start->isMachine == false) {
		cout << "Starting Key is not a Machine\n";
		return nullptr;
	}
	cout << "Routing: " << endl;
	return (Recurrsion(Start, find));
}

void RingDHT::PrintOnlyOne() {
	bool flag = false;
	Node* Start = head;
	do {
		flag = false;
		cout << "\n> Enter the key of the Machine [Routing Table Print] " << endl;;
		int x;
		cin >> x;
		for (int i = 0; i < x; i++) {
			Start = Start->next;
		}
		if (!Start->isMachine) {
			cout << "> The Machine for this key does not exist. Try again." << endl;
			flag = true;
		}
	} while (flag);
	PrintTable(Start);
	return;
}

//Left:
/*
-> Btree Deletion (main) 
-> Inserting a machine; data store on the next machine (with key lower than the new inserted one) to be stored on the new machine
-> same scenario with deletion, the deleted machine will have all data transfered to the one next to it
-> linked list in the string of btree paths
*/
