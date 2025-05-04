#pragma once
#include<iostream>
#include<fstream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;
const int M = 25;
const int N = 40;
bool isSleep = 0;
int state = 1; // 1 = title, 2 = menu, 3 =  
int grid[M][N] = { 0 };
int ts = 30; //tile size

struct Player {
    string name;
    string password;
    int score = 0;
    int TotalScore = 0;
    int noOfMatches = 0;
    int HighestScore = 0;
    Player* FriendsList = nullptr;
    int matchHistory[50];
};
class PlayerProfile {
private:
    Player* players;
    int currentPlayer1;
    int currentPlayer2;
    int numPlayers;
public:
    PlayerProfile() {
        currentPlayer1 = -1;
        currentPlayer2 = -1;
        numPlayers = 0;
        players = nullptr;
    }
    void ReadPlayers();
};
struct Enemy
{
    int x, y, dx, dy;

    Enemy()
    {
        x = y = 300;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
    }

    void move()
    {
        x += dx; if (grid[y / ts][x / ts] == 1) { dx = -dx; x += dx; }
        y += dy; if (grid[y / ts][x / ts] == 1) { dy = -dy; y += dy; }
    }
};

class Authentication {
private:
    char enteredChar;
    RenderWindow& window;
    string username;
    string password;
    string username2;
    string password2;
    Texture log, sign;
    Sprite logSp, signSp;
    Clock clock;
    int subState;
    Font font;
    Text nameT, passT, text;
    int state0state = 0;
    bool userFound = true;
    bool isPassCorrect = true;
    bool isAlreadyPresent = false;

public:
    Authentication(RenderWindow& window) :window(window) {
        enteredChar = '\0';
        username = "";
        password = "";
        username2 = "";
        password2 = "";
        subState = 0;
        if (!log.loadFromFile("images/Login.jpg")) {
            cout << "faltu" << endl;
        }
        sign.loadFromFile("images/Sign-In.jpg");
        logSp.setTexture(log);
        signSp.setTexture(sign);
        font.loadFromFile("images/Arial.ttf");
        nameT.setFont(font);
        passT.setFont(font);
    }
    bool signUp(string user, string pass);
    bool logIn(string user, string pass);
    void run();
    void handleEvents(Event& event);
    void render(RenderWindow& win);

};
class State {
private:
    //RenderWindow& window;
public:
    virtual void handleEvents(Event& event) = 0;
    virtual void run() = 0;
    virtual void render(RenderWindow& window) = 0;
};

class Title : public State {
private:
    Texture backGroundTexture;
    Sprite backGroundSprite;
public:
    Title();
    void handleEvents(Event& event) override;
    void run() override;
    void render(RenderWindow& window) override;
};
class Menu : public State {
private:
    Authentication auth;
    int numOptions;
    string* optionNames;
    RectangleShape* buttons;
    Text* text;
    Font font;
    Texture backGroundTexture;
    Sprite backGroundSprite;
    Text menuName;
    RenderWindow& window;
    bool isAuthenticationOn;
    bool isSecondAuthenticationOn;
public:
    Menu(RenderWindow& render_window) :window(render_window),auth(render_window) {
        numOptions = 7;
        optionNames = new string[numOptions]{ "Singular Mode","Multiplayer Mode","Match Making","Change Theme","Set Difficulty Level","Leader Board","Exit" };
        buttons = new RectangleShape[numOptions];
        text = new Text[numOptions];

        if (!font.loadFromFile("images/arial.ttf")) {
            std::cerr << "Could not load font.\n";
        }
        if (!backGroundTexture.loadFromFile("images/Menu.jpg")) {
            cout << "Image was not loaded" << endl;
        }
        backGroundSprite.setTexture(backGroundTexture);
        isAuthenticationOn = false;
        isSecondAuthenticationOn = false;
    };
    void handleEvents(Event& event) override;
    void run() override;
    void render(RenderWindow& window) override;
};
class StateManager {

private:
	State* state_class;
    bool isChanged;
	int substate;
    RenderWindow& window;
public:
    StateManager(RenderWindow& render_window):window(render_window) {
        isChanged = 0;
        substate = state;
        state_class = new Title;
    }
    bool isStateChanged();
    void ChangeState(int state);
    void handleEvent(Event& e);
    void run();
    void render(RenderWindow& window);
};
class Game {
private:
	StateManager state_manager;
	RenderWindow& window;
public:
	Game(RenderWindow& render_window) :window(render_window),state_manager(render_window){};
	void start();
};



