#include <SFML/Graphics.hpp>
#include <time.h>
#include "game.h"
using namespace sf;



// PLayer Profile
void PlayerProfile::ReadPlayers() {
    ifstream inputFile("username.txt");
    string line;
    while (getline(inputFile, line)) {

    }
}

// State Manager:
bool StateManager::isStateChanged() {
    if (substate != state) {
        substate = state;
        return true;
    }
    else
        return false;
}
void StateManager::ChangeState(int state) {
    if (state == 1) {
        state_class = new Title;
    }
    else if (state == 2) {
        state_class = new Menu(window);
    }
    
}
void StateManager::handleEvent(Event& e) {
    state_class->handleEvents(e);
}
void StateManager::run() {
    state_class->run();
}
void StateManager::render(RenderWindow& window) {
    state_class->render(window);
}

// Title
Title::Title() {
    if (!backGroundTexture.loadFromFile("images/background2.png")) {
        cout << "Image was not loaded" << endl;
    }
    backGroundSprite.setTexture(backGroundTexture);
}
void Title::handleEvents(Event& event) {
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Enter) {
            cout << state << endl;
            state = 2;
        }
    }
}
void Title::run() {
    // Nothing
}
void Title::render(RenderWindow& window) {
    window.clear();
    window.draw(backGroundSprite);
   // window.display();
}

// Menu


void Menu::handleEvents(Event& event) {
    if (isAuthenticationOn) {
        auth.handleEvents(event);
        return;
    }
    if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Left) {
            int mouse_x = event.mouseButton.x;
            int mouse_y = event.mouseButton.y;
            for (int i = 0; i < numOptions; i++) {
                if (buttons[i].getGlobalBounds().contains(static_cast<float>(mouse_x), static_cast<float>(mouse_y))) {
                    if (i == 0) {
                        cout << "yes its me" << endl;
                        isAuthenticationOn = true;
                       // auth = new Authentication(window);
                        return;
                    }
                    else if (i == 1) {
                        isAuthenticationOn = true;
                        isSecondAuthenticationOn = true;
                    }
                }
            }
        }
    }
}
void Menu::run() {
   
    if (isAuthenticationOn) {
        auth.run();
        return;
    }
   

    menuName.setCharacterSize(50);
    menuName.setFont(font);
    menuName.setFillColor(Color::White);
    menuName.setString("Menu");
    menuName.setPosition(550, 200);

    for (int i = 0; i < numOptions; i++) {
       
        buttons[i].setSize(Vector2f(350.0f,50.0f));
        buttons[i].setPosition(450, 300 + i * 56);
        buttons[i].setFillColor(Color::Transparent);
        buttons[i].setOutlineThickness(2);
        buttons[i].setOutlineColor(Color::White);
        buttons[i].setOrigin(0, 0);

        text[i].setFont(font);
        text[i].setString(optionNames[i]);
        text[i].setCharacterSize(24);
        text[i].setFillColor(Color::Black);
        text[i].setPosition(525, 305 + i * 56);
    }
}
void Menu::render(RenderWindow& window) {

    if (isAuthenticationOn) {
        auth.render(window);
        return;
    }
    // BackGround 
    window.draw(backGroundSprite);
    window.draw(menuName);
    
    for (int i = 0; i < numOptions; i++) {
        int mouseX = Mouse::getPosition(window).x;
        int mouseY = Mouse::getPosition(window).y;

        if (buttons[i].getGlobalBounds().contains(static_cast<float>(mouseX), static_cast<float>(mouseY))) {
            // Hover effect here
            buttons[i].setFillColor(sf::Color(100, 100, 100)); // grey box
            text[i].setFillColor(sf::Color::Yellow);
        }
        else {
            buttons[i].setFillColor(Color::Transparent);   // transparent background
            text[i].setFillColor(Color::White);           // default font color
        }
    }

    for (int i = 0; i < numOptions; i++) {
        window.draw(buttons[i]);
        window.draw(text[i]);
    }
   
   // window.display();
}
// Authentication
bool Authentication::signUp(string user, string pass) {
    
   
    
    if (pass.length() < 8) {
        isPassCorrect = false;
    }
    ifstream inFile("username.txt");
    string line;
    while (getline(inFile, line)) {
        if (user == line) {
            isAlreadyPresent = true;
            return false;
        }
    }
    ofstream outFile("username.txt", ios::app);
    outFile << user << endl;
    outFile << pass << endl;
    outFile.close();
}
bool Authentication::logIn(string user, string pass) {
  
    ifstream inFile("username.txt");
    string line;
   
    while (getline(inFile, line)) {
        if (line == user) {
            string passw;
            getline(inFile, passw);
            if (pass == passw)
            {

                cout << "Account is found" << endl;
                userFound = true;
                return userFound;
            }
        }
    }
    inFile.close();
    userFound = false;
    return false;
}
void Authentication::run() {
   
}
void Authentication::handleEvents(Event& event) {
    if (event.type == Event::MouseButtonPressed &&
        event.mouseButton.button == Mouse::Left) {

        int x = event.mouseButton.x;
        int y = event.mouseButton.y;
        int tileX = x / ts;
        int tileY = y / ts;
        cout << tileX << " " << tileY << endl;
       
        if (tileX >= 27 && tileX <= 33 && tileY >= 23 && tileY <= 25) {
            subState = 1;
        }
        cout << subState << endl;
    }
 
    if (subState == 0 && event.type == sf::Event::TextEntered) {
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
                    if (!logIn(username, password)) {
                        cout << "Not correct" << endl;
                        state0state = 0;
                        username = "";
                        password = "";
                    }
                    else {
                        state = 3;
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
    else if (subState == 1 && event.type == sf::Event::TextEntered) {
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

                    if (!signUp(username2, password2)) {
                        state0state = 0;
                        username2 = "";
                        password2 = "";
                    }
                    else {
                        state = 3;
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
void Authentication::render(RenderWindow& win) {
    
   

    if (subState == 0) {
        window.draw(logSp);

        nameT.setPosition(10 * ts, 13 * ts);
        nameT.setString(username);
        nameT.setCharacterSize(37);
        nameT.setFillColor(sf::Color::Black);
        window.draw(nameT);

        passT.setPosition(10 * ts, 19.5 * ts);
        passT.setString(password);
        passT.setCharacterSize(37);
        passT.setFillColor(sf::Color::Black);
        window.draw(passT);

    }
    else if (subState == 1) {
        window.draw(signSp);
        nameT.setPosition(10 * ts, 13 * ts);
        nameT.setString(username2);
        nameT.setCharacterSize(37);
        nameT.setFillColor(Color::Black);
        window.draw(nameT);

        passT.setPosition(10 * ts, 19.5 * ts);
        passT.setString(password2);
        passT.setCharacterSize(37);
        passT.setFillColor(Color::Black);
        window.draw(passT);
    }

    if (!userFound) {
        cout << userFound << endl;
        text.setFont(font);
        text.setPosition(5 * ts, 5 * ts);
        text.setFillColor(sf::Color::Red);
        text.setCharacterSize(27);
        text.setString("You have either enterd wrong username or password");
        window.draw(text);
        window.display();
        sleep(sf::seconds(2));
        cout << "You have either enterd wrong username or password" << endl;
        userFound = true;
        isSleep = 1;
    }
    if (!isPassCorrect) {
        isPassCorrect = true;
        text.setFont(font);
        text.setPosition(5 * ts, 5 * ts);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(27);
        text.setString("Password should atleast have 8 characters");
        window.draw(text);
        window.display();
        sf::sleep(sf::seconds(2));
        cout << "Password should atleast have 8 characters" << endl;
    }
    if (isAlreadyPresent) {
        isAlreadyPresent = false;
        text.setFont(font);
        text.setPosition(5 * ts, 5 * ts);
        text.setFillColor(sf::Color::Red);
        text.setCharacterSize(27);
        text.setString("This username already exists");
        window.draw(text);
        window.display();
        sf::sleep(sf::seconds(2));
        cout << "This username already exists" << endl;
    }
}

// Game:
void drop(int y, int x)
{
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (grid[y - 1][x] == 0) drop(y - 1, x);
    if (grid[y + 1][x] == 0) drop(y + 1, x);
    if (grid[y][x - 1] == 0) drop(y, x - 1);
    if (grid[y][x + 1] == 0) drop(y, x + 1);
}

void Game::start() {

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
                window.close();
            }
            state_manager.handleEvent(event);       
        }
        if (state_manager.isStateChanged()) {
            state_manager.ChangeState(state);
        }
        state_manager.run();
        state_manager.render(window);
        window.display();
    
        window.clear();
        
    }

}

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(N * ts, M * ts), "Xonix Game!");
    window.setFramerateLimit(60);
    Game game(window);
    game.start();
   

    return 0;
}