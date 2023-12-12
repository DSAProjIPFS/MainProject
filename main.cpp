#include <iostream>
#include <windows.h>
#include <conio.h>
#include "RINGDHT.h"
using namespace std;

void startScreen() {
	cout << endl << endl << endl << endl << endl;;
	cout << "========================================================================================================================" << endl << endl;
	cout << "                                         ###  #####  ######  ######" << endl;
	cout << "                                          #   #   #  ##      ##" << endl;
	cout << "                                          #   #####  ######  ######" << endl;
	cout << "                                          #   ##     ##          ##" << endl;
	cout << "                                         ###  ##     ##      ######" << endl << endl;

	cout << "                                  Project by i22-0812 | i22-2123 | i22-8222" << endl << endl;
	cout << "========================================================================================================================" << endl << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "                                              Press [Enter] to Proceed" << endl << endl;
	_getch();
	system("cls");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
}
void MainMenu(RingDHT* DHT) {
	//just a demo (option 1,4,6 work for now)
	//some options are just for us (to see the current status) (options 6 & 7) ;; will remove later if needed
	while (1) {
		int choice = 0;
		do {
			cout << "\n=============================================" << endl;
			cout << "> Options" << endl;
			cout << "1. Insert File" << endl;
			cout << "2. Remove File" << endl;
			cout << "3. Print Routing Table" << endl;
			cout << "4. Add New Machine" << endl;
			cout << "5. Remove a Machine" << endl;
			cout << "6. Print DHT" << endl;
			cout << "7. Show Paths of Inserted Data" << endl;
			cout << "0. Exit" << endl;
			cout << "=============================================" << endl << "> ";
			cin >> choice;
			system("cls");
		} while (choice > 7 && choice != 0);

		switch (choice) {
		case 0: cout << "Exiting . . ." << endl; return;
			break;
		case 1: DHT->insertFile();
			break;
		case 2: DHT->removeFile();
			break;
		case 3: DHT->PrintOnlyOne();
			break;
		case 4:
			DHT->insertMachine();
			break;
		case 5:
			DHT->removeFile();
			break;
		case 6:
			DHT->showNodes();
			break;
		case 7:
			DHT->showDirectories();
			break;
		}


	}


}


int main() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	startScreen();
	RingDHT DHT;
	MainMenu(&DHT);

}
