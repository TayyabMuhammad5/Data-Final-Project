#pragma once
#include<iostream>
#include<fstream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;
const int M = 25;
const int N = 40;
bool isSleep = 0;
int state = 1; // 1 = title, 2 = menu, 3 = Singular Mode
int grid[M][N] = { 0 };
int ts = 30; //tile size


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
    void ReadPlayers();
    int CheckUsername(string name);
    int CheckPassword(string name);
    void Addplayer(string username, string password);
    void WritePlayers();
    void display();
    int& getPlayer1() {
        return currentPlayer1;
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
class SingularMode : public State {
private:
    PlayerProfile* profile;
    Texture t1, t2, t3;
    Sprite sTile, sGameover, sEnemy;
    int enemyCount = 4;
    Enemy a[10];
    bool Game;
    int x = 0, y = 0, dx = 0, dy = 0;
    float timer, delay;
    Clock clock;
public:
    SingularMode(PlayerProfile* player) : profile(player) {
        t1.loadFromFile("images/tiles_30.png");
        t2.loadFromFile("images/gameover.png");
        t3.loadFromFile("images/enemy.png");
        sTile.setTexture(t1);
        sGameover.setTexture(t2);
        sEnemy.setTexture(t3);
        Game = true;
        sGameover.setPosition(100, 100);
        sEnemy.setOrigin(25, 25);
        delay = 0.07;
        timer = 0;
        for (int i = 0;i < M;i++)
            for (int j = 0;j < N;j++)
                if (i == 0 || j == 0 || i == M - 1 || j == N - 1)  grid[i][j] = 1;
    }
    void handleEvents(Event& event) override;
    void run() override;
    void render(RenderWindow& window) override;
};
class MultiPlayerMode : public State {
private:
    PlayerProfile* profile;
    Texture t1, t2, t3;
    Sprite sPlayer1, sGameover, sEnemy, sPlayer2;
    int enemyCount = 4;
    Enemy a[10];
    bool Game;
    int x = 0, y = 0, dx = 0, dy = 0, x2 = 20, y2 = 0, dx2 = 0, dy2 = 0;
    float timer, delay;
    char keyPressed_Player1;
    char keyPressed_Player2;
    Clock clock;
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
        for (int i = 0;i < M;i++)
            for (int j = 0;j < N;j++)
                if (i == 0 || j == 0 || i == M - 1 || j == N - 1)  grid[i][j] = 1;
        keyPressed_Player1 = '\0';
        keyPressed_Player2 = '\0';
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





