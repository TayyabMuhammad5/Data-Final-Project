#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;
struct Theme {
	Theme* Right;
	Theme* Left;
	int Height;
	int ID;
	string Name;
	string description;
	Theme() {
		Right = nullptr;
		Left = nullptr;
		ID = 0;
		Name = "";
		description = "";
	}
};
int Height(Theme* P)
{
	if (P == NULL)
		return -1;
	else
		return P->Height;
}
class AvlTree {
private:
	Theme* root;
public:
	AvlTree() {
		root = nullptr;
	}
	void Insert(string name, string des, int id) {
		root = insert(name, des, id, root);
	}
	int Max(int op1, int op2) {
		if (op1 > op2) {
			return op1;
		}
		else
			return op2;
	}
	Theme* insert(string name, string des, int id, Theme* root) {
		if (root == nullptr) { /* Create and return a one-node tree */
			root = new Theme;
			root->description = des;
			root->Name = name;
			root->ID = id;
			root->Right = nullptr;
			root->Left = nullptr;
			root->Height = 0; // Height of a leaf node should be 0, not -1
		}
		else if (id < root->ID) {
			root->Left = insert(name, des, id, root->Left);
			if (Height(root->Left) - Height(root->Right) == 2)
				if (id < root->Left->ID)
					root = SingleRotateWithLeft(root);
				else
					root = DoubleRotateWithLeft(root);
		}
		else if (id > root->ID) {
			root->Right = insert(name, des, id, root->Right); // Fixed: Use root->Right instead of root->Left
			if (Height(root->Right) - Height(root->Left) == 2) // Fixed: Check imbalance correctly
				if (id > root->Right->ID) // Fixed: Check correct condition
					root = SingleRotateWithRight(root);
				else
					root = DoubleRotateWithRight(root);
		}
		root->Height = Max(Height(root->Left), Height(root->Right)) + 1;
		return root;
	}
	Theme* SingleRotateWithRight(Theme* K1) {
		Theme* K2;
		K2 = K1->Right; // K1: node whose balance factor is violated
		K1->Right = K2->Left;
		K2->Left = K1;
		K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
		K2->Height = Max(Height(K2->Right), K1->Height) + 1;
		return K2; /* New root */
	}
	Theme* SingleRotateWithLeft(Theme* K1) {
		Theme* K2;
		K2 = K1->Left; // K1: node whose balance factor is violated
		K1->Left = K2->Right;
		K2->Right = K1;
		K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
		K2->Height = Max(Height(K2->Left), K1->Height) + 1;
		return K2; /* New root */
	}
	Theme* DoubleRotateWithRight(Theme* K1)
	{
		/* RR rotation between K3 and K2 */
		K1->Right = SingleRotateWithLeft(K1->Right);
		/* LL rotation between K1 and K2 */
		return SingleRotateWithRight(K1);
	}
	Theme* DoubleRotateWithLeft(Theme* K3)
	{
		/* LL rotation between K1 and K2 */
		K3->Left = SingleRotateWithRight(K3->Left);
		/* RR rotation between K3 and K2 */
		return SingleRotateWithLeft(K3);
	}
	void displayThemes(Theme* node, sf::RenderWindow& window, sf::Font& font, float& y,int& x) {
		if (node == nullptr) return;

		displayThemes(node->Left, window, font, y,x);

		sf::Text nameText;
		nameText.setFont(font);
		nameText.setCharacterSize(25);
		nameText.setFillColor(sf::Color::White);
		nameText.setString(to_string(x) + " Name: " + node->Name);
		nameText.setFillColor(Color::Black);
		nameText.setPosition(50, y);
		//y += 35;

		sf::Text descText;
		descText.setFont(font);
		descText.setCharacterSize(22);
		descText.setFillColor(sf::Color::Cyan);
		descText.setString("Description: " + node->description);
		descText.setFillColor(sf::Color::Cyan);
		descText.setPosition(350, y);
		y += 45;

		window.draw(nameText);
		window.draw(descText);
		x += 1;
		displayThemes(node->Right, window, font, y,x);
	}

	void displayThemes(sf::RenderWindow& window, sf::Font& font) {
		float y = 180.0f;  // Initial vertical position
		int x = 1;
		displayThemes(root, window, font, y,x);
		
	}
	string getThemeNameByID(int id) {
		Theme* current = root;
		while (current != nullptr) {
			if (id == current->ID) {
				return current->Name;
			}
			else if (id < current->ID) {
				current = current->Left;
			}
			else {
				current = current->Right;
			}
		}
		return "Theme not found"; // ID not in tree
	}
	// For debugging
	void printInOrder() {
		printInOrder(root);
		cout << endl;
	}

	void printInOrder(Theme* node) {
		if (node == nullptr) return;
		printInOrder(node->Left);
		cout << "ID: " << node->ID << " Name: " << node->Name << " | ";
		printInOrder(node->Right);
	}
};