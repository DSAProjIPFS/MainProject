#pragma once
#include<iostream>
using namespace std;

//will implement SHA-1 here later, for now just using a simple hash function for testing

unsigned int getHash(string& input, int num) {
	int hash = 0;
	for (int i = 0; i < input.length(); i++) {
		hash += (int)input[i];
	}

	int key = hash % (num);
	return key;
}
