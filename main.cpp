#include <iostream>
#include "RINGDHT.h"
using namespace std;

void MainMenu(RingDHT *DHT) {
	//just a demo (options arent working for now lol)
	int choice = 0;
	do {
		cout << "\n=============================================" << endl;
		cout << "> Options" << endl;
		cout << "1. Insert File" << endl;
		cout << "2. Remove File" << endl;
		cout << "3. Print Routing Table" << endl;
		cout << "4. Add New Machine" << endl;
		cout << "5. Remove a Machine" << endl;
		cout << "0. Exit" << endl;
		cout << "=============================================" << endl << "> ";
		cin >> choice;
	} while (choice > 5 && choice != 0);

	switch (choice) {
	case 0: return; 
	case 1: DHT->insertFile();
		break;
	}

}


int main() {
	RingDHT DHT;
	MainMenu(&DHT);
}