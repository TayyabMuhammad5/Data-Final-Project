#pragma once
#include "game.h"

struct list {
	int index = -1;
	string key = "";
	bool isOccupied = false;
};
class HashMap {
private:
	list* hashtable;
public:
	HashMap() {
		hashtable = new list[101];

	}
	int generateKey(string key) {
		int ptr = 0;
		for (int i = 0; i < key.length(); i++) {
			ptr += key[i];
		}
		return ptr % 101;
	}
	void insert(string name,int num) {
		int key = generateKey(name);
		while (hashtable[key].isOccupied) {
			key = (key + 1) % 101;
		}
		hashtable[key].index = num;
		hashtable[key].key = name;
		hashtable[key].isOccupied = true;
	}
	int findIndex(string name) {
		int index = generateKey(name);
		int start = index;
		while (hashtable[index].isOccupied) {
			if (hashtable[index].key == name) {
				return hashtable[index].index;
			}
			index = (index + 1) % 101;
			if (index == start) {
				break; // Searched the whole table
			}
		}
		cout << "Not found" << endl;
		return -1;
	}

};