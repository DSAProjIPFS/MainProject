#include <iostream>
#include <windows.h>
#include <conio.h>
#include "RINGDHT.h"
using namespace std;

const int MAX_BUFFER_SIZE = 32767;

void startScreen() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	cout << endl << endl << endl << endl << endl;;
	cout << "========================================================================================================================" << endl << endl;
	cout << "                                         ###  #####  ######  ######" << endl;
	cout << "                                          #   #   #  ##      ##" << endl;
	cout << "                                          #   #####  ######  ######" << endl;
	cout << "                                          #   ##     ##          ##" << endl;
	cout << "                                         ###  ##     ##      ######" << endl << endl;;
	cout << "                                         InterPlanetary File System" << endl << endl;
	cout << "                                  Project by i22-0812 | i22-2123 | i22-8222" << endl << endl;
	cout << "========================================================================================================================" << endl << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "                                         Press [Enter] to Proceed" << endl << endl;
	_getch();
	system("cls");
	HANDLE hhConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hhConsole, FOREGROUND_GREEN);
}
void MainMenu(RingDHT* DHT) {
	//just a demo (option 1,4,6 work for now)
	//some options are just for us (to see the current status) (options 6 & 7) ;; will remove later if needed
	while (1) {
		int choice = 0;
		do {
			/*cin.clear();
			cin.ignore(MAX_BUFFER_SIZE, '\n'); */
			cout << "\n=============================================" << endl;
			cout << "> Options" << endl;
			cout << "---------------------------------------------" << endl;
			cout << "1. Insert File" << endl;
			cout << "2. Remove File" << endl;
			cout << "3. Print Routing Table" << endl;
			cout << "4. Add New Machine" << endl;
			cout << "5. Remove a Machine" << endl;
			cout << "6. Print DHT" << endl;
			cout << "7. Print B-Tree of a Machine" << endl;
			cout << "8. Get Path of a File" << endl;
			cout << "9. Info" << endl;
			cout << "0. Exit" << endl;
			cout << "=============================================" << endl << "> ";
			cin >> choice;
			system("cls");
		} while (choice > 10 && choice != 0);

		switch (choice) {
		case 0: cout << "Exiting . . ." << endl; 
			this_thread::sleep_for(chrono::seconds(1));
			return;
			break;
		case 1:
			DHT->insertFile(); //fully working
			break;
		case 2:
			DHT->removeFile(); //fully working
			break;
		case 3:
			DHT->PrintTables(); //fully working
			break;
		case 4:
			DHT->insertMachine();
			break;
		case 5:
			DHT->deleteMachine(); //fully working
			break;
		case 6:
			DHT->showNodes();
			break;
		case 7:
			DHT->printMachineBtree(); //fully wok
			break;
		case 8:
			DHT->getFilePath();
			break;
		case 9:
			DHT->getInfo();
			break;
		}
	}
}


int main() {
	startScreen();
	RingDHT DHT;
	MainMenu(&DHT);
	return 0;
}
