#pragma once

#include <iostream>
#include <time.h>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <stack>
#include <string>

#include "Hashing.h"
using namespace std;

class Node {
public:
	int key; //hash value
	string value; //actual content of whatever txt file
	/*
	Machine -> hashed value generated from its name i.e google.com
	Data -> hashed value generated from its content i.e txt file content
	*/
	bool isMachine;
	Node* next;
	Node* prev;

	Node() {
		isMachine = false;
		key = 0; value = '0';
	}

	Node(int k, string val, bool flag) {
		isMachine = flag;
		key = k; value = val;
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
		if (curr->isMachine)
			cout << "Machine" << endl;
		else
			cout << "Data" << endl; //will later show status too (if the node is empty or nah)
		curr = curr->next; count++;
	}
	cout << "--------------------------------" << endl;
}

void RingDHT::randomizeMachines() {
	int total = pow(2, identifier_space);
	cout << "\n> Randomizing Machines . . ." << endl << endl;
	int m = total/NumberOfMachines;
	int i = -1;
	Node* current = nullptr;
	int machines_installed = 0;

	while (i < total) 
	{
		int j = i+m;

		while (i < j) {
			if (current)
			{
				current = current->next;
			}
			else
			{
				current = head;
			}
			i++;
		}
		
		current->isMachine = true;
		machines_installed++;
		if (machines_installed == NumberOfMachines)
		{
			cout << "aa" << endl;
			break;
		}
	}
}

void RingDHT::manualAssignMachines() {
	int total = pow(2, identifier_space);
	cout << "\n> Assign Manual IDs . . ." << endl;
	int m = 1; int assign = 0;
	while (m <= NumberOfMachines) {
		cout << "Machine_" << m << " :: Assign ID: ";
		cin >> assign;
		Node* current = head; int i = 0;
		while (i < assign) {
			current = current->next;
			i++;
		}
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
		cin >> path;
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
	int fileKey = getHash(fileContent, total);

	cout << "> Storing on Node_" << fileKey << endl;

	Node* current = head; int indx = 0;
	while ((indx++) <= fileKey)
		current = current->next;

	current->key = fileKey;
	current->value = fileContent;

	// still incomplete

}
