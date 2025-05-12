#ifndef game_H
#define game_H
#pragma once
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include <SFML/Graphics.hpp>
#include "AVLTree.h"

using namespace std;
using namespace sf;
const int M = 25;
const int N = 40;
bool isSleep = 0;
int state = 1; // 1 = title, 2 = menu, 3 = subMenu, 4 = Singular Mode, 5 = Multiplayer Mode, 6 = Theme, 7 == LeaderBoard, 8 = End Menu
int grid[M][N] = { 0 };
int ts = 30; //tile size
bool isLoaded = false;
int prevState = -1;

struct Player {
    string name = "";
    string password = "";
    int score = 0;
    int TotalScore = 0;
    int noOfMatches = 0;
    int HighestScore = 0;
    string* friendNames = nullptr;
    int* matchHistory = nullptr;
    int numfriends = 0;
    string theme;
    Player() {

    }
    void operator=(const Player& tmp) {
        name = tmp.name;
        password = tmp.password;
        score = tmp.score;
        TotalScore = tmp.TotalScore;
        noOfMatches = tmp.noOfMatches;
        HighestScore = tmp.HighestScore;
        friendNames = tmp.friendNames;
        matchHistory = tmp.matchHistory;
    }
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
    int getNumPlyaers() {
        return numPlayers;
    }
    Player getPlayerObject(int index) {
        return players[index];
    }
    void ReadPlayers();
    string getTheme() {
        return players[currentPlayer1].theme;
    }
    int CheckUsername(string name);
    int CheckPassword(string name);
    void Addplayer(string username, string password);
    void WritePlayers();
    void display();
    int& getPlayer1() {
        return currentPlayer1;
    }
    string getPlayerName(int num);
    int getScore(int num);
    void addTheme(string th) {
        players[currentPlayer1].theme = th;
    }
    void scoring(int num, int pt) {
        if (num == 1) {
            players[currentPlayer1].score += pt;
            players[currentPlayer1].TotalScore += pt;
        }
        else {
            players[currentPlayer2].score += pt;
            players[currentPlayer2].TotalScore += pt;
        }
    }
    void setScore(int num) {
        players[currentPlayer1].score = num;
    }
    int getNumFriends() {
        if (currentPlayer1 != -1)
            return players[currentPlayer1].numfriends;
    }
    void displayFriends() {
        for (int i = 0;i < players[currentPlayer1].numfriends;i++) {
            cout << players[currentPlayer1].friendNames[i] << " ";
        }
        cout << endl;
    }
    void addScore(int s, int i) {
        if (i == 1 && currentPlayer1 != -1) {
            players[currentPlayer1].score = s;
            players[currentPlayer1].TotalScore += s;
            players[currentPlayer1].noOfMatches++;
            if (s > players[currentPlayer1].HighestScore) {
                players[currentPlayer1].HighestScore = s;
            }
            WritePlayers();  
        }
        else if (i == 2 && currentPlayer2 != -1) {
            players[currentPlayer2].score = s;
            players[currentPlayer2].TotalScore += s;
            players[currentPlayer2].noOfMatches++;

            if (s > players[currentPlayer2].HighestScore) {
                players[currentPlayer2].HighestScore = s;
            }
            WritePlayers();  
        }
    }
  
    int& getPlayer2() {
        return currentPlayer2;
    }
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
        x += dx; if (grid[y / ts][x / ts] == 1 || grid[y / ts][x / ts] == 4 || grid[y / ts][x / ts] == 2) { dx = -dx; x += dx; }
        y += dy; if (grid[y / ts][x / ts] == 1 || grid[y / ts][x / ts] == 4 || grid[y / ts][x / ts] == 2) { dy = -dy; y += dy; }
    }
};
struct singEnemy
{
    int x, y, dx, dy;

    singEnemy()
    {
        //x = y = 300;
        x = 600;
        y = 360;
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
    int numTimes;
    PlayerProfile* profile;
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
    bool isDouble = false;
public:
    Authentication(RenderWindow& window, PlayerProfile* player) :window(window), profile(player) {
        numTimes = 0;
        enteredChar = '\0';
        username = "";
        password = "";
        username2 = "";
        password2 = "";
        subState = 0;
        if (!log.loadFromFile("images/Login.jpg")) {

        }
        sign.loadFromFile("images/Sign-In.jpg");
        logSp.setTexture(log);
        signSp.setTexture(sign);
        font.loadFromFile("images/Arial.ttf");
        nameT.setFont(font);
        passT.setFont(font);
    }
    int& getNumTimes() {
        return numTimes;
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
    PlayerProfile* profile;
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
    Menu(RenderWindow& render_window, PlayerProfile* player) :window(render_window), auth(render_window, profile), profile(player) {
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

class SubMenu : public State {
private:
    PlayerProfile* profile;
    int subState;
    int numOptions;
    string* optionNames;
    RectangleShape* buttons;
    Text* text;
    Font font;
    Texture backGroundTexture;
    Sprite backGroundSprite;
    Text menuName;
    string number;
    bool isNotfound = false;
    bool otherPlayer = false;
public:
    SubMenu(PlayerProfile* player):profile(player) {
        int subState = 0;
        numOptions = 6;
        optionNames = new string[numOptions]{ "Start Game","Select Difficulty Level","Change Theme","Load Saved Game","Friends","Exit" };
        buttons = new RectangleShape[numOptions];
        text = new Text[numOptions];

        if (!font.loadFromFile("images/arial.ttf")) {
            std::cerr << "Could not load font.\n";
        }
        if (!backGroundTexture.loadFromFile("images/Menu.jpg")) {
            cout << "Image was not loaded" << endl;
        }
        backGroundSprite.setTexture(backGroundTexture);
    }
    void handleEvents(Event& event) override;
    void run() override;
    void render(RenderWindow& window) override;
};
class EndMenu : public State {
private:
    PlayerProfile* profile;
    int numOptions;
    string* optionNames;
    RectangleShape* buttons;
    Text* text;
    Font font;
    Texture backGroundTexture;
    Sprite backGroundSprite;
    Text menuName;
    bool isSaved = false;
public:
    EndMenu(PlayerProfile* player):profile(player) {
        numOptions = 3;
        optionNames = new string[numOptions]{ "Resume","Save","Exit"};
        buttons = new RectangleShape[numOptions];
        text = new Text[numOptions];

        if (!font.loadFromFile("images/arial.ttf")) {
            std::cerr << "Could not load font.\n";
        }
        if (!backGroundTexture.loadFromFile("images/Menu.jpg")) {
            cout << "Image was not loaded" << endl;
        }
        backGroundSprite.setTexture(backGroundTexture);
    }
    void handleEvents(Event& event) override;
    void run() override;
    void render(RenderWindow& window) override;
};
class Friends :public State {
private:
    //int subState;
    PlayerProfile* players;
    int numOptions;
    string* optionNames;
    RectangleShape* buttons;
    Text* text;
    Font font;
    Texture backGroundTexture;
    Sprite backGroundSprite;
    Text menuName;
public:
    Friends(PlayerProfile* player) :players(player) {
        numOptions = 4;
        optionNames = new string[numOptions]{ "Friends","Send Requests","Accept Requests","Exit" };
        buttons = new RectangleShape[numOptions];
        text = new Text[numOptions];

        if (!font.loadFromFile("images/arial.ttf")) {
            std::cerr << "Could not load font.\n";
        }
        if (!backGroundTexture.loadFromFile("images/Menu.jpg")) {
            cout << "Image was not loaded" << endl;
        }
        backGroundSprite.setTexture(backGroundTexture);
    }
    void handleEvents(Event& event) override;
    void run() override;
    void render(RenderWindow& window) override;
};
class LeaderBoard :public State {
    PlayerProfile* profile;
    Player minheap[10];
    int root;
    int index;
    int numOptions;
    string* topPlayers;
    Player temp[10];
    RectangleShape* buttons;
    Text* text;
    Font font;
    Texture backGroundTexture;
    Sprite backGroundSprite;
    Text menuName;
    void heapifyDown(int i) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < index && minheap[left].TotalScore < minheap[smallest].TotalScore) {
            smallest = left;
        }
        if (right < index && minheap[right].TotalScore < minheap[smallest].TotalScore) {
            smallest = right;
        }
        if (smallest != i) {
            Player temp = minheap[i];
            minheap[i] = minheap[smallest];
            minheap[smallest] = temp;
            heapifyDown(smallest);
        }
    }

    void heapifyUp(int i) {
        if (i == 0)
            return;
        int parent = (i - 1) / 2;
        if (minheap[i].TotalScore < minheap[parent].TotalScore) {
            Player temp = minheap[i];
            minheap[i] = minheap[parent];
            minheap[parent] = temp;
            heapifyUp(parent);
        }
    }

public:

    LeaderBoard(PlayerProfile* player) :profile(player) {
        root = 0;
        index = 0;
        numOptions = 10;
        topPlayers = new string[numOptions];
        buttons = new RectangleShape[numOptions];
        text = new Text[numOptions];
        for (int i = 0;i < numOptions;i++) {
            topPlayers[i] = "";
        }
        if (!font.loadFromFile("images/arial.ttf")) {
            std::cerr << "Could not load font.\n";
        }
        if (!backGroundTexture.loadFromFile("images/Menu.jpg")) {
            cout << "Image was not loaded" << endl;
        }
        backGroundSprite.setTexture(backGroundTexture);
        loadFromProfile();
    }
    int getNumPlayers() {
        return index;
    }
    Player& getPlayers(int i) {
        return temp[i];
    }
    void loadFromProfile() {
        for (int i = 0; i < profile->getNumPlyaers(); i++) {
            Player p = profile->getPlayerObject(i);
            insert(p);
        }
    }
    void insert(Player player) {
        for (int i = 0; i < index; i++) {
            if (minheap[i].name == player.name) {
                minheap[i].TotalScore = player.TotalScore;
                heapifyDown(i);
                heapifyUp(i);
                return;
            }
        }

        if (index < 10) {
            minheap[index] = player;
            heapifyUp(index);
            index++;
        }
        else if (player.TotalScore > minheap[0].TotalScore) {
            minheap[0] = player;
            heapifyDown(0);
        }
    }


    void sorting() {
        for (int i = 0;i < index;i++) {
            temp[i] = minheap[i];
        }
        for (int i = 0;i < index - 1;i++) {
            for (int j = 0;j < index - i - 1;j++) {
                if (temp[j].TotalScore < temp[j + 1].TotalScore) {
                    Player tempe = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = tempe;
                }
            }
        }
        for (int i = 0;i < index;i++) {
            topPlayers[i] = to_string(i + 1) + ". " + temp[i].name + "       " + to_string(temp[i].TotalScore);
        }
    }
    void handleEvents(Event& event) override;
    void run() override;
    void render(RenderWindow& window) override;

};
class SingularMode : public State {
private:
    PlayerProfile* profile;
    Texture t1, t2, t3;
    Sprite sTile, sGameover, sEnemy;
    int enemyCount = 4;
    singEnemy a[10];
    bool Game;
    int x = 0, y = 1, dx = 0, dy = 0;
    float timer, delay;
    Clock clock;
    int score = 0;
    Font font;
    int occurence = 0, threshold = 10, multiple = 2;
    Sprite sTrail, sWall, sPlayer;

public:
    SingularMode(PlayerProfile* player) : profile(player) {
        if (!font.loadFromFile("images/arial.ttf")) {
            std::cerr << "Could not load font.\n";
        }
        score = profile->getScore(1);
        t1.loadFromFile("images/tiles_30.png");
        t1.loadFromFile("images/tiles_30.png");
        t2.loadFromFile("images/gameover.png");
        t3.loadFromFile("images/enemy.png");
        sTile.setTexture(t1);
        sTrail.setTexture(t1);
        sWall.setTexture(t1);
        sPlayer.setTexture(t1);
        sGameover.setTexture(t2);
        sEnemy.setTexture(t3);
        Game = true;
        sGameover.setPosition(100, 100);
        sEnemy.setOrigin(25, 25);
        delay = 0.07;
        timer = 0;
        for (int i = 0;i < N; i++) {
            grid[0][i] = -2;
        }
        for (int i = 1;i < M;i++)
            for (int j = 0;j < N;j++)
                if (i == 1 || j == 0 || i == M - 1 || j == N - 1)  grid[i][j] = 1;
    }
    void handleEvents(Event& event) override;
    void run() override;
    void render(RenderWindow& window) override;
    void FloodFill(int, int);
};
class MultiPlayerMode : public State {
private:
    PlayerProfile* profile;
    Texture t1, t2, t3;
    Sprite sPlayer1, sGameover, sEnemy, sPlayer2;
    int enemyCount = 4;
    Enemy a[10];
    bool Game;
    Font font;
    int x = 0, y = 1, dx = 0, dy = 0, x2 = 20, y2 = 1, dx2 = 0, dy2 = 0;
    float timer, delay;
    char keyPressed_Player1;
    char keyPressed_Player2;
    Clock clock;
    int score1 = 0;
    int score2 = 0;
    bool isPlayer1Dead = false, isPlayer2Dead = false;
    Text score;
    int tempGrid[M][N] = { 0 };
    static int count;
    bool isPlayer1moving = false;
    bool isPlayer2moving = false;
    int occurence1 = 0, threshold1 = 10, multiple1 = 2,captured1 = 0,captured2 = 0;
    int occurence2 = 0, threshold2 = 10, multiple2 = 2;
public:
    MultiPlayerMode(PlayerProfile* player) : profile(player) {
        t1.loadFromFile("images/tiles_30.png");
        t2.loadFromFile("images/gameover.png");
        t3.loadFromFile("images/enemy.png");
        sPlayer1.setTexture(t1);
        sPlayer2.setTexture(t1);
        sGameover.setTexture(t2);
        sEnemy.setTexture(t3);
        Game = true;
        sGameover.setPosition(100, 100);
        sEnemy.setOrigin(25, 25);
        delay = 0.07;
        timer = 0;
        for (int i = 1;i < M;i++)
            for (int j = 0;j < N;j++)
                if (i == 1 || j == 0 || i == M - 1 || j == N - 1)  grid[i][j] = 1;
        keyPressed_Player1 = '\0';
        keyPressed_Player2 = '\0';
        if (!font.loadFromFile("images/arial.ttf")) {
            std::cerr << "Could not load font.\n";
        }
        score.setFont(font);

    }
    void handleEvents(Event& event) override;
    void run() override;
    void render(RenderWindow& window) override;
    
    void FloodFill(int, int);
};
class Inventory :public State {
private:
    PlayerProfile* profile;
    AvlTree avl;
    int substate;
    int numOptions;
    string* optionNames;
    RectangleShape* buttons;
    Text* text;
    Font font;
    Texture backGroundTexture;
    Sprite backGroundSprite;
    Text themeName;
    string number = "";
    string selected = "";
    Text init;
    
public:
    Inventory(PlayerProfile* player):profile(player) {
        
        avl.Insert("Blue Enchantment", "Blue background, Red Player, green trail and Light blue walls", 0);
        avl.Insert("Purple Universe", "Purple backgroud, Green Player, red trail and blue walls", 1);
        avl.Insert("Green's Paradise", "Green Background, Golden Player, purple trail and Red walls", 2);
        avl.Insert("Crimson Depths", "Dark Red background, Cyan Player, orange trail and Yellow walls", 3);
        avl.Insert("Neon Mirage", "Black background, Yellow Player, red trail and Green walls", 4);
        avl.Insert("Frozen Circuit", "White background, Light blue Player, dark blue trail and Purple walls", 5);
        avl.Insert("Tropical Pulse", "Sunset Orange background, Green Player, cyan trail and Red walls", 6);
        avl.Insert("Galactic Bloom", "Starry Sky background, Purple Player, yellow trail and Dark blue walls", 7);
        avl.Insert("Cyber Grove", "Neon Green background, Red Player, purple trail and Orange walls", 8);
        avl.Insert("Solar Storm", "Bright Yellow background, Blue Player, green trail and Cyan walls", 9);

        substate = 0;
        numOptions = 2;
        optionNames = new string[numOptions]{ "View Themes","Select Theme by ID"};
        buttons = new RectangleShape[numOptions];
        text = new Text[numOptions];

        if (!font.loadFromFile("images/FORTE.ttf")) {
            std::cerr << "Could not load font.\n";
        }
        if (!backGroundTexture.loadFromFile("images/Menu.jpg")) {
            cout << "Image was not loaded" << endl;
        }
        backGroundSprite.setTexture(backGroundTexture);
    }
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
    PlayerProfile* profile;
public:
    StateManager(RenderWindow& render_window, PlayerProfile* player) :window(render_window) {
        profile = player;
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
    PlayerProfile* profile;
public:
    Game(RenderWindow& render_window, PlayerProfile* player) :window(render_window), state_manager(render_window, player), profile(player) {};
    void start();
};
int MultiPlayerMode::count = 0;
#endif




