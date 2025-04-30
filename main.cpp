#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include <string>
#include <time.h>
#include <fstream>
using namespace std;
using namespace sf;

const int M = 30;
const int N = 30;

int grid[M][N] = {0};
int ts = 32; //tile size

struct Enemy
{
    int x,y,dx,dy;

  Enemy()
   {
    x=y=300;
    dx=4-rand()%8;
    dy=4-rand()%8;
   }

  void move()
   { 
    x+=dx;
    if (grid[y/ts][x/ts]==1)
    {
        dx=-dx;
        x+=dx;
    }
    y+=dy;
    if (grid[y/ts][x/ts]==1) 
    {
        dy=-dy;
        y+=dy;
    }
   }
};

void drop(int y,int x)
{
  if (grid[y][x]==0) grid[y][x]=-1;
  if (grid[y-1][x]==0) drop(y-1,x);
  if (grid[y+1][x]==0) drop(y+1,x);
  if (grid[y][x-1]==0) drop(y,x-1);
  if (grid[y][x+1]==0) drop(y,x+1);
}

class Authentiacation {
    string username;
    string password;
public:
    bool signUp(sf::RenderWindow& window, string user, string pass) {
        sf::Font font;
        font.loadFromFile("images/Arial.ttf");
        sf::Text text;
        text.setFont(font);
        text.setPosition(7.5 * ts, 27.5 * ts);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(27);
        if (pass.length() < 8) {
            text.setString("Password should atleast have 8 characters");
            window.draw(text);
            window.display();
            sf::sleep(sf::seconds(2));
            cout << "Password should atleast have 8 characters" << endl;
            return false;
        }
        ifstream inFile("username.txt");
        string line;
        while (getline(inFile, line)) {
            if (user == line) {
                text.setString("This username already exists");
                window.draw(text);
                window.display();
                sf::sleep(sf::seconds(2));
                cout << "This username already exists" << endl;
                return false;
            }
            if (line == pass) {
                text.setString("This password already exists");
                window.draw(text);
                window.display();
                sf::sleep(sf::seconds(2));
                cout << "This password already exists" << endl;
                return false;
            }
        }
        ofstream outFile("username.txt", ios::app);
        outFile << user << endl;
        outFile << pass << endl;
        outFile.close();
    }
    bool logIn(sf::RenderWindow& window, string user, string pass) {
        sf::Font font;
        font.loadFromFile("images/Arial.ttf");
        sf::Text text;
        text.setFont(font);
        text.setPosition(7.25 * ts, 26.75 * ts);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(27);
        ifstream inFile("username.txt");
        string line;
        bool userFound = false;
        while (getline(inFile, line)) {
            if (line == user) {
                string passw;
                getline(inFile, passw);
                if (pass == passw)
                {

                    cout << "Account is found" << endl;
                    userFound = true;
                }
            }
        }
        inFile.close();
        if (!userFound) {
            text.setString("You have either enterd wrong username or password");
            window.draw(text);
            window.display();
            sf::sleep(sf::seconds(2));
            cout << "You have either enterd wrong username or password" << endl;
        }
        return userFound;
    }
};
int main()
{
    int n = 2;
    srand(time(0));

    RenderWindow window(VideoMode(N*ts, M*ts), "Xonix Game!");
    window.setFramerateLimit(60);

    Texture t1,t2,t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100,100);
    sEnemy.setOrigin(20,20);

    int enemyCount = 4;
    Enemy a[10];

    bool Game=true;
    int x=0, y=0, dx=0, dy=0;
    float timer=0, delay=0.07; 
    Texture log, sign,playersNo;
    log.loadFromFile("images/Login.png");
    sign.loadFromFile("images/Sign In.png");
    playersNo.loadFromFile("images/No of Players.png");
    Sprite logSp(log), signSp(sign),playerSp(playersNo);
    Clock clock;
    int state = -1,noOfPlayers;
    sf::Font font;
    Authentiacation auth;
    string username = "", password = "";
    string username2 = "", password2 = "";
    sf::Text nameT, passT;
    int state0state = 0;
    font.loadFromFile("images/Arial.ttf");
    nameT.setFont(font);
    passT.setFont(font);

    for (int i=0;i<M;i++)
     for (int j=0;j<N;j++)
      if (i==0 || j==0 || i==M-1 || j==N-1)  grid[i][j]=1;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::Escape)
                {
                    for (int i = 1;i < M - 1;i++)
                        for (int j = 1;j < N - 1;j++)
                            grid[i][j] = 0;

                    x = 10;y = 0;
                    Game = true;
                }
            if (event.type == Event::MouseButtonPressed &&
                event.mouseButton.button == Mouse::Left) {

                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                int tileX = x / ts;
                int tileY = y / ts;
                cout << tileX << " " << tileY << endl;
                if (state == 0) {
                    if (tileX >= 21 && tileX < 25 && tileY >= 27 && tileY < 29) {
                        state = 1;
                    }
                }
                else if (state == -1) {
                    if (tileX >= 5 && tileX < 25 && tileY >= 11 && tileY < 15) {
                        state = 0;
                        noOfPlayers = 1;
                    }
                    else if (tileX >= 5 && tileX < 25 && tileY >= 16 && tileY < 20) {
                        state = 0;
                        noOfPlayers = 2;
                    }
                }
            }
            if (state == 0 && event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 8) {
                        if (!state0state && !username.empty()) {
                            username.pop_back();
                        }
                        else if (state0state && !password.empty()) {
                            password.pop_back();
                        }
                    }
                    else if (event.text.unicode == 13) {
                        if (!state0state) {
                            state0state = 1;

                        }
                        else {
                            state0state = 0;
                            if (!auth.logIn(window, username, password)) {
                                state0state = 0;
                                username = "";
                                password = "";
                            }
                            else {
                                state = 2;
                            }

                        }
                    }
                    else {
                        char enteredChar = static_cast<char>(event.text.unicode);
                        if (isprint(enteredChar)) {
                            if (isalnum(enteredChar) || enteredChar == ' ') {
                                if (!state0state)
                                    username += enteredChar;
                                else
                                    password += enteredChar;
                            }
                        }
                    }

                }
            }
            else if (state == 1 && event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 8) {
                        if (!state0state && !username2.empty()) {
                            username2.pop_back();
                        }
                        else if (state0state && !password2.empty()) {
                            password2.pop_back();
                        }
                    }
                    else if (event.text.unicode == 13) {
                        if (!state0state)
                            state0state = 1;
                        else {

                            if (!auth.signUp(window, username2, password2)) {
                                state0state = 0;
                                username2 = "";
                                password2 = "";
                            }
                            else {
                                state = 2;
                            }
                        }
                    }
                    else {
                        char enteredChar = static_cast<char>(event.text.unicode);
                        if (isprint(enteredChar)) {
                            if (isalnum(enteredChar) || enteredChar == ' ') {
                                if (!state0state)
                                    username2 += enteredChar;
                                else
                                    password2 += enteredChar;
                            }
                        }
                    }

                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) { dx = -1;dy = 0; };
        if (Keyboard::isKeyPressed(Keyboard::Right)) { dx = 1;dy = 0; };
        if (Keyboard::isKeyPressed(Keyboard::Up)) { dx = 0;dy = -1; };
        if (Keyboard::isKeyPressed(Keyboard::Down)) { dx = 0;dy = 1; };

        if (!Game) continue;

        if (timer > delay)
        {
            x += dx;
            y += dy;

            if (x < 0) x = 0; if (x > N - 1) x = N - 1;
            if (y < 0) y = 0; if (y > M - 1) y = M - 1;

            if (grid[y][x] == 2) Game = false;
            if (grid[y][x] == 0) grid[y][x] = 2;
            timer = 0;
        }

        for (int i = 0;i < enemyCount;i++) a[i].move();

        if (grid[y][x] == 1)
        {
            dx = dy = 0;

            for (int i = 0;i < enemyCount;i++)
                drop(a[i].y / ts, a[i].x / ts);

            for (int i = 0;i < M;i++)
                for (int j = 0;j < N;j++)
                    if (grid[i][j] == -1) grid[i][j] = 0;
                    else grid[i][j] = 1;
        }

        for (int i = 0;i < enemyCount;i++)
            if (grid[a[i].y / ts][a[i].x / ts] == 2) Game = false;

        /////////draw//////////
        window.clear();
        if (state == -1) {
            window.draw(playerSp);
        }
        if (state == 0) {
            window.draw(logSp);

            nameT.setPosition(7 * ts, 16 * ts);
            nameT.setString(username);
            nameT.setCharacterSize(37);
            nameT.setFillColor(sf::Color::Black);
            window.draw(nameT);

            passT.setPosition(7 * ts, 23.5 * ts);
            passT.setString(password);
            passT.setCharacterSize(37);
            passT.setFillColor(sf::Color::Black);
            window.draw(passT);

        }
        else if (state == 1) {
            window.draw(signSp);
            nameT.setPosition(7 * ts, 16 * ts);
            nameT.setString(username2);
            nameT.setCharacterSize(37);
            nameT.setFillColor(sf::Color::Black);
            window.draw(nameT);

            passT.setPosition(7 * ts, 23.5 * ts);
            passT.setString(password2);
            passT.setCharacterSize(37);
            passT.setFillColor(sf::Color::Black);
            window.draw(passT);
        }
        else if(state==2){
            for (int i = 0;i < M;i++)
                for (int j = 0;j < N;j++)
                {
                    if (grid[i][j] == 0) continue;
                    if (grid[i][j] == 1) sTile.setTextureRect(IntRect(0, 0, ts, ts));
                    if (grid[i][j] == 2) sTile.setTextureRect(IntRect(54, 0, ts, ts));
                    sTile.setPosition(j * ts, i * ts);
                    window.draw(sTile);
                }

            sTile.setTextureRect(IntRect(36, 0, ts, ts));
            sTile.setPosition(x * ts, y * ts);
            window.draw(sTile);

            sEnemy.rotate(10);
            for (int i = 0;i < enemyCount;i++)
            {
                sEnemy.setPosition(a[i].x, a[i].y);
                window.draw(sEnemy);
            }

            if (!Game) window.draw(sGameover);
        }
            window.display();
       
    }

    return 0;
}
