#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include "game.h"
using namespace std;
using namespace sf;
struct Node {
	Player player;
	Node* next;
};

class GameRoom {
	Node* head;
	Node* tail;
public:
	GameRoom() {
		head = NULL;
		tail = NULL;
	}
	bool isEmpty() {
		if (head) {
			return false;
		}
		return true;
	}
	void enqueue(Player p) {
		Node* temp = new Node;
		temp->player = p;
		temp->next = NULL;
		if (!head) {
			head = temp;
			tail = temp;
		}
		else {
			tail->next = temp;
			tail = temp;
		}
	}
	Player& dequeue() {
		if (!isEmpty()) {
			Node* temp = head;
			head = head->next;
			return temp->player;
		}
	}
	Player& peek() {
		if (!isEmpty()) {
			return head->player;
		}
	}

};
class MatchMaking :public LeaderBoard{
	static GameRoom gr;
	PlayerProfile* profile;
	Text* text;
	Font font;
	Texture backGroundTexture;
	Sprite backGroundSprite;
	Text menuName;
	Player player1, player2;
	string s;
	bool end = false;
	Clock matchClock;
	float matchDelay = 3.0f;
	static bool matchShown ;

public:
	MatchMaking(PlayerProfile*profile):profile(profile),LeaderBoard(profile) {
		if (!font.loadFromFile("images/arial.ttf")) {
			std::cerr << "Could not load font.\n";
		}
		if (!backGroundTexture.loadFromFile("images/Menu.jpg")) {
			cout << "Image was not loaded" << endl;
		}
		backGroundSprite.setTexture(backGroundTexture);
		loadFromProfile();
		sorting();
		if (gr.isEmpty() && !matchShown) {
			for (int i = 0; i < getNumPlayers(); i++) {
				gr.enqueue(getPlayers(i));
			}
		}

		if (!matchShown) {
			matches();
			matchShown = true;
		}
		if (gr.isEmpty() && matchShown) {
			prevState = -1;
			return;
		}
	}
	void matches() {
		player1 = gr.dequeue();
		player2 = gr.dequeue();
		s = player1.name + " vs " + player2.name;
		matchClock.restart();
	}

	void handleEvents(Event& event) {
		if (event.type == Event::KeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::Enter)) {
				profile->getPlayer1() = profile->CheckUsername(player1.name);
				profile->getPlayer2() = profile->CheckUsername(player2.name);
				prevState = 8;
				state = 5;
				
			}
			
		}
	}
	void run() {
		
		if (matchClock.getElapsedTime().asSeconds() > matchDelay) {
			matches();
		}
		
		
		
		menuName.setCharacterSize(50);
		menuName.setFont(font);
		menuName.setFillColor(Color::White);
		menuName.setString(s);
		menuName.setPosition(450, 200);
	}
	void render(RenderWindow& window) {
		window.draw(backGroundSprite);
		window.draw(menuName);
	}
};
GameRoom MatchMaking::gr;
