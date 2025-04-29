#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
using namespace sf;
using namespace std;
const int M = 30;
const int N = 30;

int grid[M][N] = { 0 };
int ts = 32;
class Authentiacation {
    string username;
    string password;
public:
    bool signUp(sf::RenderWindow &window,string user, string pass) {
        sf::Font font;
        font.loadFromFile("images/Arial.ttf");
        sf::Text text;
        text.setFont(font);
        text.setPosition(7.5 * ts, 27.5 * ts);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(27);
        if (pass.length()<8) {
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
    bool logIn(sf::RenderWindow& window,string user, string pass) {
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
int main() {

    srand(time(0));
    RenderWindow window(VideoMode(N * ts, M * ts), "Xonix Game!");
    window.setFramerateLimit(60);


    Texture log, sign;
    log.loadFromFile("images/Login.png");
    sign.loadFromFile("images/Sign In.png");
    Sprite logSp(log), signSp(sign);
    Clock clock;
    int state = 0;
    sf::Font font;
    Authentiacation a;
    string username = "", password = "";
    string username2 = "", password2 = "";
    sf::Text nameT,passT;
    int state0state = 0;
    font.loadFromFile("images/Arial.ttf");
    nameT.setFont(font);
    passT.setFont(font);
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed &&
                event.mouseButton.button == Mouse::Left) {

                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                int tileX = x / ts;
                int tileY = y / ts;
                cout << tileX << " " << tileY << endl;
                if (tileX >= 21 && tileX < 25 && tileY >= 27 && tileY < 29) {
                    state = 1;
                }
            }
            if (state == 0 && event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 8 ) {
                        if (!state0state && !username.empty()) {
                            username.pop_back();
                        }
                        else if (state0state && !password.empty()){
                                password.pop_back();
                        }
                    }
                    else if (event.text.unicode == 13) {
                        if (!state0state) {
                            state0state = 1;
                           
                        }
                        else {
                            state0state = 0;
                            if (!a.logIn(window, username, password)) {
                                state0state = 0;
                                username = "";
                                password = "";
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
            else if(state == 1 && event.type == sf::Event::TextEntered) {
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
                          
                            if (!a.signUp(window, username2, password2)) {
                                state0state = 0;
                                username2 = "";
                                password2 = "";
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
        window.clear();


           

        if (state == 0 ) {
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
            window.display();
        }

    }
