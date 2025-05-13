#include <SFML/Graphics.hpp>
#include <time.h>
#include<sstream>
#include "game.h"
#include "Matching.h"
#include "Save.h"
#include "HashMap.h"
using namespace sf;


// PLayer Profile
void PlayerProfile::ReadPlayers() {

    std::ifstream inputFile("username.txt", std::ios::binary | std::ios::ate);

    if (!inputFile || inputFile.tellg() == 0) {
        std::cout << "File doesn't exist or is empty.\n";
        players = nullptr;
        numPlayers = 0;
        return;
    }

    inputFile.seekg(0);
    string line;
    int i = 0;



    while (getline(inputFile, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line == "---") {
            numPlayers += 1;
            Player* tmp = new Player[numPlayers];
            for (int j = 0; j < numPlayers - 1; j++) {
                tmp[j] = players[j];
            }
            players = tmp;
            i = 0;
            continue;
        }
        if (i == 0) {
            players[numPlayers - 1].name = line;
        }
        else if (i == 1) {
            players[numPlayers - 1].password = line;
        }
        else if (i == 2) {
            players[numPlayers - 1].score = stoi(line);
        }
        else if (i == 3) {
            players[numPlayers - 1].TotalScore = stoi(line);
        }
        else if (i == 4) {
            players[numPlayers - 1].noOfMatches = stoi(line);
        }
        else if (i == 5) {
            players[numPlayers - 1].HighestScore = stoi(line);
        }
        else if (i == 6 && line != "null") {

           
            stringstream ss(line);
            string token;

            // Count friends first
            while (getline(ss, token, ',')) {
                players[numPlayers - 1].numfriends += 1;
            }

            // Allocate and reset stream
            ss.clear();
            ss.seekg(0, ios::beg);
            ss.str(line);

            players[numPlayers - 1].friendNames = new string[players[numPlayers - 1].numfriends];
            int fIdx = 0;
            while (getline(ss, token, ',')) {
                players[numPlayers - 1].friendNames[fIdx++] = token;
            }
            //cout << players[numPlayers - 1].numfriends << endl;
        }
        else if (i == 7 && line != "null") {

            players[numPlayers - 1].matchHistory = new int[players[numPlayers - 1].noOfMatches];
            int j = 0;
            for (int k = 0; k < line.length(); k++) {
                if (line[k] == ',') {
                    continue;
                }
                players[numPlayers - 1].matchHistory[j++] = line[k] - '0';
            }
        }
        else if (i == 8) {
            players[numPlayers - 1].theme = line;
        }
        else if (i == 9) {
            // Accept
            stringstream ss(line);
            string token;

            // Count friends first
            while (getline(ss, token, ',')) {
                players[numPlayers - 1].numAccept += 1;
            }

            // Allocate and reset stream
            ss.clear();
            ss.seekg(0, ios::beg);
            ss.str(line);

            players[numPlayers - 1].accept = new string[players[numPlayers - 1].numAccept];
            int fIdx = 0;
            while (getline(ss, token, ',')) {
                players[numPlayers - 1].accept[fIdx++] = token;
            }
        }
       
        i++;
    }
}
void PlayerProfile::WritePlayers() {
    ofstream outFile("username.txt");

    for (int i = 0; i < numPlayers; ++i) {
        outFile << "---" << '\n';
        outFile << players[i].name << '\n';
        outFile << players[i].password << '\n';
        outFile << players[i].score << '\n';
        outFile << players[i].TotalScore << '\n';
        outFile << players[i].noOfMatches << '\n';
        outFile << players[i].HighestScore << '\n';
        if (players[i].friendNames != nullptr && players[i].numfriends > 0) {
            for (int j = 0; j < players[i].numfriends; ++j) {
                outFile << players[i].friendNames[j];
                if (j != players[i].numfriends - 1)
                    outFile << ',';
            }
            outFile << '\n';
        }
        else {
            outFile << '\n';
        }
        

        if (players[i].matchHistory != nullptr && players[i].noOfMatches > 0) {
            for (int j = 0; j < players[i].noOfMatches; ++j) {
                outFile << players[i].matchHistory[j];
                if (j != players[i].noOfMatches - 1)
                    outFile << ',';
            }
            outFile << '\n';
        }
        else {
            outFile << '\n';
        }
        outFile <<  players[i].theme << '\n';
        if (players[i].accept != nullptr && players[i].numAccept > 0) {
            for (int j = 0; j < players[i].numAccept; ++j) {
                outFile << players[i].accept[j];
                if (j != players[i].numAccept - 1)
                    outFile << ',';
            }
            outFile << '\n';
        }
        else {
            outFile << '\n';
        }
       // outFile << '\n';

    }

    outFile.close();
}

void PlayerProfile::display() {

   // cout << "numPlayers:" << numPlayers << endl;
    for (int i = 0;i < numPlayers; i++) {

        cout << "Name: " << players[i].name << endl;
        cout << "Password: " << players[i].password << endl;
        cout << "Score: " << players[i].score << endl;
        cout << "Total Score: " << players[i].TotalScore << endl;
        cout << "Highest Score: " << players[i].HighestScore << endl;
        cout << "Number of Matches: " << players[i].noOfMatches << endl;
        cout << "Friends: ";
        cout << players[i].numfriends << endl;
        for (int j = 0; j < players[i].numfriends; j++) {
            cout << players[i].friendNames[j] << ", ";
        }
        cout << endl;
        cout << "Match History: ";
       /* for (int j = 0; j < players[i].noOfMatches; j++) {
            cout << players[i].matchHistory[j] << ", ";
        }*/
        cout << endl;
        cout << "Theme: " << players[i].theme << endl;
        cout << players[i].numAccept << endl;
        for (int j = 0; j < players[i].numAccept; j++) {
            cout << players[i].accept[j] << ", ";
        }
        cout << endl;
    }
}
int PlayerProfile::CheckUsername(string username) {
    for (int i = 0; i < numPlayers; i++) {
        if (players[i].name == username) {
            return i;
        }
    }
    return -1;
}
int PlayerProfile::CheckPassword(string username) {
    for (int i = 0; i < numPlayers; i++) {
        if (players[i].password == username) {
            return i;
        }
    }
    return -1;
}
void PlayerProfile::Addplayer(string username, string pass) {
    Player* tmp = new Player[numPlayers + 1];
    for (int i = 0; i < numPlayers; i++) {
        tmp[i] = players[i];
    }
    tmp[numPlayers].name = username;
    tmp[numPlayers].password = pass;
    tmp[numPlayers].HighestScore = 0;
    tmp[numPlayers].score = 0;
    tmp[numPlayers].TotalScore = 0;
    tmp[numPlayers].friendNames = nullptr;
    tmp[numPlayers].noOfMatches = 0;
    tmp[numPlayers].matchHistory = nullptr;
    players = tmp;
    numPlayers += 1;
}
string PlayerProfile::getPlayerName(int num) {
    if (num == 1) {
        return players[currentPlayer1].name;
    }
    return players[currentPlayer2].name;
}

int PlayerProfile::getScore(int num) {
    if (num == 1) {
        return players[currentPlayer1].score;
    }
    return players[currentPlayer2].score;
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
void StateManager::ChangeState(int newstate) {
    if (state != newstate) {
        prevState = state;
        state = newstate;
    }
    if (state_class) {
        delete state_class;
    }
    if (newstate == 1) {
        state_class = new Title;
    }
    else if (newstate == 2) {
        state_class = new Menu(window, profile);
    }
    else if (newstate == 3) {
        state_class = new SubMenu(profile);
    }
    else if (newstate == 4) {
        state_class = new SingularMode(profile);
    }
    else if (newstate == 5) {
        state_class = new MultiPlayerMode(profile);
    }
    else if (newstate == 6) {
        state_class = new Inventory(profile);
    }
    else if (newstate == 7) {
        state_class = new LeaderBoard(profile);
    }
    else if (state == 8) {
        state_class = new EndMenu(profile);
    }
    else if (state == 9) {
        state_class = new MatchMaking(profile);
    }
    else if (state == 10) {
        state_class = new Friends(profile);
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
    if (auth.getNumTimes() != 0) {
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
                        // isAuthenticationOn = true;
                        auth.getNumTimes() = 1;
                        // auth = new Authentication(window);
                        return;
                    }
                    else if (i == 1) {
                        // isAuthenticationOn = true;
                        // isSecondAuthenticationOn = true;
                        auth.getNumTimes() = 2;
                    }
                    else if (i == 3) {
                        state = 7;
                    }
                    else if (i == 2) {
                        state = 9;
                    }
                    else if (i == 4) {
                        window.close();
                    }
                }
            }
        }
    }
}
void Menu::run() {

    if (auth.getNumTimes() != 0) {
        auth.run();
        return;
    }


    menuName.setCharacterSize(50);
    menuName.setFont(font);
    menuName.setFillColor(Color::White);
    menuName.setString("Menu");
    menuName.setPosition(550, 200);

    for (int i = 0; i < numOptions; i++) {

        buttons[i].setSize(Vector2f(350.0f, 50.0f));
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

    if (auth.getNumTimes() != 0) {
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

    
}
// Sub-Menu
void SubMenu::handleEvents(Event& event) {
    
    if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Left) {
            int mouse_x = event.mouseButton.x;
            int mouse_y = event.mouseButton.y;
            for (int i = 0; i < numOptions; i++) {
                if (buttons[i].getGlobalBounds().contains(static_cast<float>(mouse_x), static_cast<float>(mouse_y))) {
                    if (i == 0) {
                        state = 4;
                        return;
                    }
                    else if (i == 1) {
                        state = 6;
                    }
                    else if (i == 2) {
                        subState = 1;
                    }
                    else if (i == 3) {
                        state = 10;
                    }
                    else if (i == 4) {
                        state = 2;
                    }
                }
            }
        }
    }
    if (subState == 1) {
        // Handle Enter key
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            subState = 0;
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                string filename = number + ".txt";
                ifstream infile(filename);
                if (!infile.is_open()) {
                    isNotfound = true;
                    cout << "Save file not found.\n";
                }
                else {
                    string savedPlayer;
                    string score;
                    getline(infile, savedPlayer);
                    getline(infile, score);
                    infile.close();

                    if (savedPlayer != profile->getPlayerName(1)) {
                        otherPlayer = true;
                        cout << "Save file belongs to a different player.\n";
                    }
                    else {
                        Save s;
                        s.deserialize(number + ".txt");
                        for (int i = 0; i < M; i++) {
                            for (int j = 0; j < N; j++) {
                                cout << grid[i][j] << " ";
                            }
                            cout << endl;
                        }
                        isLoaded = true;
                        if (score != "") {
                            profile->setScore(stoi(score));
                        }
                        else {
                            profile->setScore(0);
                        }
                        state = 4;
                        return;
                        
                    }
                }
            }
            if (event.key.code == sf::Keyboard::BackSpace) {
                if (!number.empty()) {
                    number.pop_back();
                }
            }

        }

        // Handle character input
        else if (event.type == sf::Event::TextEntered) {
            char enteredChar = static_cast<char>(event.text.unicode);
            if(isalnum(enteredChar))
                number += enteredChar;
        }
    }
}

void SubMenu::run() {

    if (subState == 0) {
        menuName.setCharacterSize(50);
        menuName.setFont(font);
        menuName.setFillColor(Color::White);
        menuName.setString("Menu");
        menuName.setPosition(550, 200);

        for (int i = 0; i < numOptions; i++) {

            buttons[i].setSize(Vector2f(350.0f, 50.0f));
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
    
}
void SubMenu::render(RenderWindow& window) {

    if (subState == 0) {
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
    }
    else if (subState == 1) {

        Text text;
        text.setFont(font);
        text.setString("Enter the Save ID: ");
        text.setPosition(200, 200);
        text.setCharacterSize(25);
        window.draw(text);
        text.setString(number);
        text.setPosition(550, 200);
        window.draw(text);
        if (isNotfound || otherPlayer) {
            text.setString("The save Id is not valid");
            text.setFillColor(Color::Red);
            text.setPosition(200, 250);
            window.draw(text);
        }
        
    }
   

}
// End Menu
void EndMenu::handleEvents(Event& event) {

    if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Left) {
            int mouse_x = event.mouseButton.x;
            int mouse_y = event.mouseButton.y;
            for (int i = 0; i < numOptions; i++) {
                if (buttons[i].getGlobalBounds().contains(static_cast<float>(mouse_x), static_cast<float>(mouse_y))) {
                    if (i == 0) {
                        state = 4;
                        return;
                    }
                    else if (i == 1) {
                        Save s(profile->getPlayerName(1),profile->getScore(1));
                        for (int i = M-1; i >= 0; i--) {
                            for (int j = N-1; j >= 0; j--) {
                                s.addElement(i, j, grid[i][j]);
                            }
                        }
                        string filename = s.getSaveID() + ".txt";
                        s.serialize(filename);
                        isSaved = true;
                    }
                    else if (i == 2) {
                        state = 3;
                    }
                }
            }
        }
    }
}

void EndMenu::run() {

    menuName.setCharacterSize(50);
    menuName.setFont(font);
    menuName.setFillColor(Color::White);
    menuName.setString("End Menu");
    menuName.setPosition(550, 200);

    for (int i = 0; i < numOptions; i++) {

        buttons[i].setSize(Vector2f(350.0f, 50.0f));
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
void EndMenu::render(RenderWindow& window) {

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
    if (isSaved) {
        Text text;
        text.setFont(font);
        text.setString("*saved");
        text.setPosition(850, 360);
        text.setCharacterSize(25);
        window.draw(text);
    }
    for (int i = 0; i < numOptions; i++) {
        window.draw(buttons[i]);
        window.draw(text[i]);
    }

}
// Friends
void Friends::handleEvents(Event& event) {
    if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Left) {
            int mouse_x = event.mouseButton.x;
            int mouse_y = event.mouseButton.y;
            if (subState == 0) {
                for (int i = 0; i < numOptions; i++) {
                    if (buttons[i].getGlobalBounds().contains(static_cast<float>(mouse_x), static_cast<float>(mouse_y))) {
                        if (i == 0) {
                            subState = 1;
                        }
                        else if (i == 1) {
                            subState = 3;
                        }
                        else if (i == 2) {
                            subState = 2;
                        }
                        else if (i == 3) {
                            state = 3;
                        }
                    }
                }
            }
            if (subState == 2) {
                for (int i = 0; i < players->getNumAccept(); i++) {
                    if (buttons[i].getGlobalBounds().contains(static_cast<float>(mouse_x), static_cast<float>(mouse_y))) {

                        for (int i = 0; i < players->getNumPlyaers(); i++) {
                            string ptr = players->getName(i);
                            mm.insert(players->getName(i), i);
                        }
                        string str = players->playerAccept(i);
                        int index = mm.findIndex(players->playerAccept(i));
                        players->addFriend(players->playerAccept(i), players->getPlayer1());
                        players->removeAccept(i);
                        players->addFriend(players->getName(players->getPlayer1()), index);
                        players->WritePlayers();
                        players->display();
                    }
                }
            }
            
        }
    }
    if (subState == 3) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                if (players->isFriend(number) || number == players->getName(players->getCurrent1())) {
                    isAvailable = true;
                    return;
                }

                for (int i = 0; i < players->getNumPlyaers(); i++) {
                    string ptr = players->getName(i);
                    mm.insert(players->getName(i), i);
                }
                int index = mm.findIndex(number);
                players->addAccept(players->getName(players->getCurrent1()), index);
                players->WritePlayers();
                players->display();
                subState = 0;
            }
        }
        if (event.key.code == sf::Keyboard::BackSpace) {
            if (!number.empty()) {
                number.pop_back();
                isAvailable = false;
            }
        }
        else if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode < 128) {  // ASCII characters only
                char enteredChar = static_cast<char>(event.text.unicode);
                cout << number << endl;
                if (std::isalnum(enteredChar) || std::isspace(enteredChar)) {
                    number += enteredChar;
                }
                else if (enteredChar == 8 && !number.empty()) {  // Handle backspace
                    number.pop_back();
                }
            }
        }
    }
}
void Friends::run() {
   
    if (subState == 0) {
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            state = 3;
        }
        menuName.setCharacterSize(50);
        menuName.setFont(font);
        menuName.setFillColor(Color::White);
        menuName.setString("Menu");
        menuName.setPosition(550, 200);

        for (int i = 0; i < numOptions; i++) {
            number = "";
            buttons[i].setSize(Vector2f(350.0f, 50.0f));
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
    if (subState == 1) {
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            subState = 0;
        }

        for (int i = 0; i < players->getNumFriends(); i++) {
            buttons[i].setSize(Vector2f(350.0f, 50.0f));
            buttons[i].setPosition(450, 300 + i * 56);
            buttons[i].setFillColor(Color::Transparent);
            buttons[i].setOutlineThickness(2);
            buttons[i].setOutlineColor(Color::White);
            buttons[i].setOrigin(0, 0);

            text1[i].setFont(font);
            text1[i].setString(players->playerFriend(i));
            text1[i].setCharacterSize(24);
            text1[i].setFillColor(Color::Black);
            text1[i].setPosition(525, 305 + i * 56);
        }
    }
    if (subState == 2) {
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            subState = 0;
        }
        text2 = new Text[players->getNumAccept()];
        for (int i = 0; i < players->getNumAccept(); i++) {
            buttons[i].setSize(Vector2f(350.0f, 50.0f));
            buttons[i].setPosition(450, 300 + i * 56);
            buttons[i].setFillColor(Color::Transparent);
            buttons[i].setOutlineThickness(2);
            buttons[i].setOutlineColor(Color::White);
            buttons[i].setOrigin(0, 0);

            text2[i].setFont(font);
            string tmp = players->playerAccept(i);
            text2[i].setString(players->playerAccept(i));
            text2[i].setCharacterSize(24);
            text2[i].setFillColor(Color::Black);
            text2[i].setPosition(525, 305 + i * 56);
        }
    }
    if (subState == 3) {
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            subState = 0;
        }
    }
}
void Friends::render(RenderWindow& window) {
    window.draw(backGroundSprite);
    if (subState == 0) {
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
    }
    if (subState == 1) {
        window.draw(backGroundSprite);
        window.draw(menuName);
        for (int i = 0; i < players->getNumFriends(); i++) {
            buttons[i].setFillColor(Color::Transparent);   // transparent background
                text1[i].setFillColor(Color::White);           // default font color
        }

        for (int i = 0; i < players->getNumFriends(); i++) {
            window.draw(buttons[i]);
            window.draw(text1[i]);
        }

    }
    if (subState == 2) {
        window.draw(menuName);
        for (int i = 0; i < players->getNumAccept(); i++) {
            buttons[i].setFillColor(Color::Transparent);   // transparent background
            text2[i].setFillColor(Color::White);           // default font color
        }

        for (int i = 0; i < players->getNumAccept(); i++) {
            window.draw(buttons[i]);
            window.draw(text2[i]);
        }
    }
    if (subState == 3) {

        Text text;
        text.setFont(font);
        text.setString("Enter the Player Name: ");
        text.setPosition(200, 200);
        text.setCharacterSize(25);
        window.draw(text);
        text.setString(number);
        text.setPosition(550, 200);
        window.draw(text);
        if (isAvailable) {
            text.setCharacterSize(30);
            text.setString("*Invalid input ");
            text.setPosition(450, 300);
            text.setFillColor(Color::Red);
            window.draw(text);
        }
    }

    
}
// Leader Board
void LeaderBoard::handleEvents(Event& event) {
  
    if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Left) {
            int mouse_x = event.mouseButton.x;
            int mouse_y = event.mouseButton.y;
            for (int i = 0; i < numOptions; i++) {
                if (buttons[i].getGlobalBounds().contains(static_cast<float>(mouse_x), static_cast<float>(mouse_y))) {
                    if (i == 0) {

                    }
                    else if (i == 1) {

                    }
                    else if (i == 2) {

                    }
                }
            }
        }
    }
}
void LeaderBoard::run() {
    sorting();
    menuName.setCharacterSize(50);
    menuName.setFont(font);
    menuName.setFillColor(Color::White);
    menuName.setString("LeaderBoard");
    menuName.setPosition(550, 200);
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        state = 2;
    }
    for (int i = 0; i < numOptions; i++) {

        buttons[i].setSize(Vector2f(350.0f, 50.0f));
        buttons[i].setPosition(450, 290 + i * 46);
        buttons[i].setFillColor(Color::Transparent);
        buttons[i].setOutlineThickness(2);
        buttons[i].setOutlineColor(Color::White);
        buttons[i].setOrigin(0, 0);

        text[i].setFont(font);
        text[i].setString(topPlayers[i]);
        //cout << topPlayers[i] << endl;
        text[i].setCharacterSize(24);
        text[i].setFillColor(Color::Black);
        text[i].setPosition(460, 295 + i * 46);
    }
}
void LeaderBoard::render(RenderWindow& window) {
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
}
// Authentication
bool Authentication::signUp(string user, string pass) {

    if (pass.length() < 8) {
        isPassCorrect = false;
        return false;
    }
    if (profile->CheckUsername(user) != -1) {
        isAlreadyPresent = true;
        return false;
    }
    else {
        profile->Addplayer(user, pass);
        if (profile->getPlayer1() == -1) {
            profile->getPlayer1() = profile->CheckUsername(user);
        }
        else {
            profile->getPlayer2() = profile->CheckUsername(user);
        }
    }
    //profile->display();
    profile->WritePlayers();
    return true;
    /*ofstream outFile("username.txt", ios::app);
    outFile << user << endl;
    outFile << pass << endl;
    outFile.close();*/
}
bool Authentication::logIn(string user, string pass) {
    string line;
    if (profile->CheckUsername(user) != -1 && profile->CheckPassword(pass) != -1) {
        if (profile->getPlayer1() == -1) {
            profile->getPlayer1() = profile->CheckUsername(user);
        }
        else {
            profile->getPlayer2() = profile->CheckUsername(user);
        }
        cout << "Account is found" << endl;
        userFound = true;
        return userFound;
    }
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
            username = "";
            password = "";

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
                       // numTimes -= 1;
                        if (numTimes == 1 && !isDouble) {
                            state = 3;
                        }
                        else if (numTimes == 2) {
                            isDouble = true;
                            subState = 0;
                            state0state = 0;
                            username = "";
                            password = "";
                            numTimes -= 1;
                        }
                        else {
                            state = 5;
                        }

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
                        // numTimes -= 1;
                        if (numTimes == 1 && !isDouble) {
                            state = 3;
                        }
                        else if (numTimes == 2) {
                            isDouble = true;
                            subState = 0;
                            state0state = 0;
                            username2 = "";
                            password2 = "";
                            numTimes -= 1;
                        }
                        else {
                            state = 5;
                        }
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
        text.setCharacterSize(65);
        text.setFont(font);
        text.setPosition(6 * ts, 6.5 * ts);
        if (!isDouble) {
            text.setString("Player 1");
        }
        else {
            text.setString("Player 2");
        }
        window.draw(text);

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
        text.setCharacterSize(65);
        text.setFont(font);
        text.setPosition(5 * ts, 6.5 * ts);
        if (!isDouble) {
            text.setString("Player 1");
        }
        else {
            text.setString("Player 2");
        }
        window.draw(text);
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

// Singular Mode
void SingularMode::handleEvents(Event& event) {
    if (!substate) {
        if (event.type == Event::KeyPressed) {
            if (event.key.code == Keyboard::Escape) {

                state = 8;
            }

        }
    }
}
// Theme
void Inventory::handleEvents(Event& event) {

    if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Left) {
            if (substate == 0) {
                int mouse_x = event.mouseButton.x;
                int mouse_y = event.mouseButton.y;
                for (int i = 0; i < numOptions; i++) {
                    if (buttons[i].getGlobalBounds().contains(static_cast<float>(mouse_x), static_cast<float>(mouse_y))) {
                        if (i == 0) {
                            substate = 1;
                            return;
                        }
                        else if (i == 1) {
                            substate = 2;
                        }
                        else if (i == 2) {
                            substate = 3;
                        }
                    }
                }
            }
        }
    }
    if (event.type == Event::KeyPressed) {
        if (substate == 1) {
            if (event.key.code == sf::Keyboard::Num0) {
                profile->addTheme("Blue Enchantment");
            }
            else if (event.key.code == sf::Keyboard::Num1) {
                profile->addTheme("Purple Universe");
            }
            else if (event.key.code == sf::Keyboard::Num2) {
                profile->addTheme("Green's Paradise");
            }
            else if (event.key.code == sf::Keyboard::Num3) {
                profile->addTheme("Crimson Depths");
            }
            else if (event.key.code == sf::Keyboard::Num4) {
                profile->addTheme("Neon Mirage");
            }
            else if (event.key.code == sf::Keyboard::Num5) {
                profile->addTheme("Frozen Circuit");
            }
            else if (event.key.code == sf::Keyboard::Num6) {
                profile->addTheme("Tropical Pulse");
            }
            else if (event.key.code == sf::Keyboard::Num7) {
                profile->addTheme("Galactic Bloom");
            }
            else if (event.key.code == sf::Keyboard::Num8) {
                profile->addTheme("Cyber Grove");
            }
            else if (event.key.code == sf::Keyboard::Num9) {
                profile->addTheme("Solar Storm");
            }
            profile->WritePlayers();
            profile->display();
            state = 3;
        }
       
    }
    if (substate == 2) {
        // Handle Enter key
        if (event.type == sf::Event::KeyPressed ) {
            if (event.key.code == sf::Keyboard::Enter) {
                selected = avl.getThemeNameByID(stoi(number));
                if (selected == "Theme not found") {
                    cout << "Theme not found" << endl;
                    init.setFont(font);
                    init.setCharacterSize(30);
                    init.setPosition(80, 300);
                    init.setFillColor(Color::White);
                    init.setString(selected);
                }
                else {
                    profile->addTheme(selected);
                    state = 3;
                }
            }
            if (event.key.code == sf::Keyboard::BackSpace) {
                if (number.empty()) {
                    number.pop_back();
                }
            }
            
        }

        // Handle character input
        else if (event.type == sf::Event::TextEntered) {
            char enteredChar = static_cast<char>(event.text.unicode);
            if (std::isdigit(enteredChar)) {
                number += enteredChar;
            }
        }
    }

}
void Inventory::run() {
    themeName.setCharacterSize(50);
    themeName.setFont(font);
    themeName.setFillColor(Color::White);
    themeName.setString("Theme");
    themeName.setPosition(550, 200);

    if (substate == 0) {
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            state = 3;
        }

        for (int i = 0; i < numOptions; i++) {

            buttons[i].setSize(Vector2f(350.0f, 50.0f));
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
    
    
}
void Inventory::render(RenderWindow& window) {

    // BackGround s
    
    if (substate == 0) {
        window.draw(backGroundSprite);
        window.draw(themeName);


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
    }
    else if (substate == 1) {
        Sprite themeBackground;
        Texture themeText;
        themeText.loadFromFile("images/Themes.png");
        themeBackground.setTexture(themeText);
        window.draw(themeBackground);
        avl.displayThemes(window, font);
        Text enter;
        enter.setFont(font);
        enter.setCharacterSize(30);
        enter.setPosition(100, 700);
        enter.setFillColor(Color::White);
        enter.setString("Press keys 1-9 to select theme.....");
        window.draw(enter);
    }
    else if (substate == 2) {
        Sprite themeBackground;
        Texture themeText;
        themeText.loadFromFile("images/Themes.png");
        themeBackground.setTexture(themeText);
        window.draw(themeBackground);
        Text enter;
        enter.setFont(font);
        enter.setCharacterSize(30);
        enter.setPosition(80, 200);
        enter.setFillColor(Color::White);
        enter.setString("Enter ID:  ");
        window.draw(enter);
        enter.setString(number);
        enter.setPosition(230, 200);
        window.draw(enter);
        window.draw(init);
    }
    

}
void SingularMode::run() {
    if (!substate) {
        if (Keyboard::isKeyPressed(Keyboard::Left)) { dx = -1;dy = 0; };
        if (Keyboard::isKeyPressed(Keyboard::Right)) { dx = 1;dy = 0; };
        if (Keyboard::isKeyPressed(Keyboard::Up)) { dx = 0;dy = -1; };
        if (Keyboard::isKeyPressed(Keyboard::Down)) { dx = 0;dy = 1; };
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        if (!Game) {
            for (int i = 1;i < M - 1;i++)
                for (int j = 1;j < N - 1;j++)
                    grid[i][j] = 0;

            x = 10;y = 0;
            Game = true;
            cout << profile->getPlayer1() << endl;
            profile->addScore(score, 1);
            
            profile->display();
            substate = 1;


        }


        if (timer > delay)
        {
            x += dx;
            y += dy;

            if (x < 0) x = 0; if (x > N - 1) x = N - 1;
            if (y < 1) y = 1; if (y > M - 1) y = M - 1;

            if (grid[y][x] == 2) Game = false; // hit own trail
            if (grid[y][x] == 0) {
                grid[y][x] = 2; // trail creation
            }
            timer = 0;
        }

        for (int i = 0;i < enemyCount;i++) a[i].move();



        if (grid[y][x] == 1) {
            dx = dy = 0;

            // Mark enemy positions
            for (int i = 0; i < enemyCount; i++)
                drop(a[i].y / ts, a[i].x / ts);

            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == 2)
                        grid[i][j] = 6;

            for (int i = 0; i < M; i++) {
                if (grid[i][0] == 0) FloodFill(i, 0);
                if (grid[i][N - 1] == 0) FloodFill(i, N - 1);
            }
            for (int j = 0; j < N; j++) {
                if (grid[0][j] == 0) FloodFill(0, j);
                if (grid[M - 1][j] == 0) FloodFill(M - 1, j);
            }


            int captured = 0;
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    if (grid[i][j] == -1) {
                        grid[i][j] = 0;
                    }
                    else if (grid[i][j] == 0) {
                        grid[i][j] = 1;
                        captured++;
                    }
                    else if (grid[i][j] == 6) {
                        grid[i][j] = 1;
                        captured++;
                    }
                }
            }
            if (occurence > 3) {
                threshold = 5;
            }
            else if (occurence > 5) {
                multiple = 4;
            }
            if (captured > threshold) {
                captured *= multiple;
                occurence++;
                cout << occurence << endl;

            }
            score += captured;


        }
        for (int i = 0;i < enemyCount;i++) {
            if (grid[a[i].y / ts][a[i].x / ts] == 2) {
                Game = false;
                return;
            }
        }

    }
    else {
        if (Keyboard::isKeyPressed(Keyboard::Enter)) {
            state = 3;
            return;
      }
    }
}

void SingularMode::FloodFill(int i, int j) {
    if (i < 0 || j < 0 || i >= M || j >= N || grid[i][j] != 0) {
        return;
    }
    grid[i][j] = -1;
    FloodFill(i + 1, j);
    FloodFill(i - 1, j);
    FloodFill(i, j + 1);
    FloodFill(i, j - 1);
}
void SingularMode::render(RenderWindow& window) {
    window.clear();
    if (!substate) {
        string theme = profile->getTheme();

        // Apply theming using IntRects (changing appearance by selecting texture sub-region)
        if (theme == "Blue Enchantment" || theme == "null" || theme == "") {
            sTrail.setTextureRect(IntRect(120, 0, ts, ts));
            sWall.setTextureRect(IntRect(180, 0, ts, ts));
            sPlayer.setTextureRect(IntRect(60, 0, ts, ts));
        }
        else if (theme == "Purple Universe") {
            sTrail.setTextureRect(IntRect(60, 0, ts, ts));
            sWall.setTextureRect(IntRect(210, 0, ts, ts));
            sPlayer.setTextureRect(IntRect(120, 0, ts, ts));
        }
        else if (theme == "Green's Paradise") {
            sTrail.setTextureRect(IntRect(30, 0, ts, ts));
            sWall.setTextureRect(IntRect(60, 0, ts, ts));
            sPlayer.setTextureRect(IntRect(150, 0, ts, ts));
        }
        else if (theme == "Crimson Depths") {
            sTrail.setTextureRect(IntRect(180, 0, ts, ts));
            sWall.setTextureRect(IntRect(120, 0, ts, ts));
            sPlayer.setTextureRect(IntRect(60, 0, ts, ts));
        }
        else if (theme == "Neon Mirage") {
            sTrail.setTextureRect(IntRect(60, 0, ts, ts));
            sWall.setTextureRect(IntRect(90, 0, ts, ts));
            sPlayer.setTextureRect(IntRect(120, 0, ts, ts));
        }
        else if (theme == "Frozen Circuit") {
            sTrail.setTextureRect(IntRect(30, 0, ts, ts));
            sWall.setTextureRect(IntRect(210, 0, ts, ts));
            sPlayer.setTextureRect(IntRect(150, 0, ts, ts));
        }
        else if (theme == "Tropical Pulse") {
            sTrail.setTextureRect(IntRect(0, 0, ts, ts));
            sWall.setTextureRect(IntRect(150, 0, ts, ts));
            sPlayer.setTextureRect(IntRect(180, 0, ts, ts));
        }
        else if (theme == "Galactic Bloom") {
            sTrail.setTextureRect(IntRect(180, 0, ts, ts));
            sWall.setTextureRect(IntRect(120, 0, ts, ts));
            sPlayer.setTextureRect(IntRect(60, 0, ts, ts));
        }
        else if (theme == "Cyber Grove") {
            sTrail.setTextureRect(IntRect(30, 0, ts, ts));
            sWall.setTextureRect(IntRect(90, 0, ts, ts));
            sPlayer.setTextureRect(IntRect(60, 0, ts, ts));
        }
        else if (theme == "Solar Storm") {
            sTrail.setTextureRect(IntRect(120, 0, ts, ts));
            sWall.setTextureRect(IntRect(30, 0, ts, ts));
            sPlayer.setTextureRect(IntRect(90, 0, ts, ts));
        }
        for (int i = 0;i < M;i++)
            for (int j = 0;j < N;j++)
            {
                if (grid[i][j] == 0) {
                    continue;
                }
                if (grid[i][j] == 1) {
                    sWall.setPosition(j * ts, i * ts);
                    window.draw(sWall);
                }
                if (grid[i][j] == 2) {
                    sTrail.setPosition(j * ts, i * ts);
                    window.draw(sTrail);
                }
            }
        Text display;
        display.setFont(font);
        display.setCharacterSize(20);
        display.setPosition(5, 1);
        display.setString("Player: " + profile->getPlayerName(1) + "               Score: " + to_string(score));
        display.setFillColor(Color::Red);
        window.draw(display);
        profile->setScore(score);
        sPlayer.setPosition(x * ts, y * ts);
        window.draw(sPlayer);

        sEnemy.rotate(20);
        for (int i = 0;i < enemyCount;i++)
        {
            sEnemy.setPosition(a[i].x, a[i].y);
            window.draw(sEnemy);
        }
    }
    else {
        window.draw(sGameover);
        Text text;
        text.setFont(font);
        text.setString("Score :" + to_string(score));
        text.setFillColor(Color::White);
        text.setPosition(200, 400);
        window.draw(text);
        if (profile->getHighScore(1) == score) {
            text.setString("This is your highest score!");
            text.setPosition(200, 440);
            window.draw(text);
        }
    }
  
}
// Multiplayer Mode
void MultiPlayerMode::handleEvents(Event& event) {
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Escape) {
            for (int i = 1;i < M - 1;i++)
                for (int j = 1;j < N - 1;j++)
                    grid[i][j] = 0;
            x = 10;y = 1;
            x2 = 100, y2 = 1;
            state = 3;
        }
    }
}

void MultiPlayerMode::run() {

    // 0 = empty space, 1 = boundary (blue) , 2 = player 1 captured tiles, 3 = player 1 trail (red) , 4 = player 2 captured tiles (green) , 5 = player 2 trail (golden)

    if (Keyboard::isKeyPressed(Keyboard::A)) { dx = -1;dy = 0; };
    if (Keyboard::isKeyPressed(Keyboard::D)) { dx = 1;dy = 0; };
    if (Keyboard::isKeyPressed(Keyboard::W)) { dx = 0;dy = -1; };
    if (Keyboard::isKeyPressed(Keyboard::S)) { dx = 0;dy = 1; };

    if (Keyboard::isKeyPressed(Keyboard::Left)) { dx2 = -1;dy2 = 0; };
    if (Keyboard::isKeyPressed(Keyboard::Right)) { dx2 = 1;dy2 = 0; };
    if (Keyboard::isKeyPressed(Keyboard::Down)) { dx2 = 0;dy2 = 1; };
    if (Keyboard::isKeyPressed(Keyboard::Up)) { dx2 = 0;dy2 = -1; };

    float time = clock.getElapsedTime().asSeconds();
    clock.restart();
    timer += time;
    if (Keyboard::isKeyPressed(Keyboard::Enter)) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                cout << grid[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    if (isPlayer1Dead && isPlayer2Dead) {
        
        Game = false;
    }
    if (isPlayer1Dead) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 3) {
                    grid[i][j] = 0;
                }
            }
        }
    }
    if (isPlayer2Dead) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 5) {
                    grid[i][j] = 0;
                }
            }
        }
    }
    if (!Game) {
        if (prevState == 8) {
            state = 9;
        }
        else {
            state = 2;
        }
        return;
    }
    if (x == x2 && y == y2 && (grid[y][x] == 3 || grid[y][x] == 5 || grid[y][x] == 0)) {
        cout << "Both players collided" << endl;
        cout << grid[y][x] << endl;
        Game = false;
    }

    if (timer > delay)
    {
      

        x += dx;
        y += dy;

        x2 += dx2;
        y2 += dy2;
        if (x == x2 && y == y2 && grid[y][x] == 0) {
            cout << "Both players collided" << endl;
            cout << grid[y][x] << endl;
            Game = false;
        }
       
        if (x < 0) x = 0; if (x > N - 1) x = N - 1;
        if (y < 1) y = 1; if (y > M - 1) y = M - 1;
        if (x2 < 0) x2 = 0; if (x2 > N - 1) x2 = N - 1;
        if (y2 < 1) y2 = 1; if (y2 > M - 1) y2 = M - 1;
        
        
        if (grid[y][x] == 5 || grid[y][x] == 3) {
            isPlayer1Dead = true;
           
        } // Player 1 hits player 2's trail
        if (grid[y2][x2] == 3 || grid[y2][x2] == 5) {
            isPlayer2Dead = true;
            
        }
        // Player 2 hits player 1's trail
        if (!isPlayer1Dead) {
            if (grid[y][x] == 0) {
                grid[y][x] = 3;
                isPlayer1moving = true;
            }
            // trail creation for player 1
           
        }
        if (!isPlayer2Dead) {
            if (grid[y2][x2] == 0) {
                grid[y2][x2] = 5;
                isPlayer2moving = true;
            } // trail creation for player 2
           
        }
        timer = 0;
    }
   
 
    for (int i = 0;i < enemyCount;i++) a[i].move();
    if (!isPlayer1moving) {
        dx = 0;
        dy = 0;
    }
    if (!isPlayer2moving) {
        dx2 = 0;
        dy2 = 0;
    }
   
    //// Player 1
    if ((grid[y][x] == 1 || grid[y][x] == 2 || grid[y][x] == 4) && !isPlayer1Dead && isPlayer1moving && !isPlayer2moving)
    {
        dx = dy = 0;
        for (int i = 0;i < enemyCount;i++)
            drop(a[i].y / ts, a[i].x / ts);

        for (int i = 0;i < M;i++) {
            for (int j = 0;j < N;j++) {
                if (grid[i][j] == 0) {
                    grid[i][j] = 2;  // Player 1 captures empty spaces as 4
                    captured1++;
                }
                else if (grid[i][j] == 3) {
                    grid[i][j] = 2;  // Player 1's trail becomes Player 1's captured
                    captured1++;
                }
                else if (grid[i][j] == -1) {
                    grid[i][j] = 0;
                }
            }
        }
        isPlayer1moving = false;
        
    }
    
    //// Player 2
    if ((grid[y2][x2] == 1 || grid[y2][x2] == 4 || grid[y2][x2] == 2) && !isPlayer2Dead && isPlayer2moving && !isPlayer1moving) {
        dx2 = dy2 = 0;

        for (int i = 0;i < enemyCount;i++)
            drop(a[i].y / ts, a[i].x / ts);

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 0) {
                    cout << "I am running " << endl;
                    cout << isPlayer2moving << endl;
                    //cout << "Captured using player 2" << endl;
                    grid[i][j] = 4;  // Player 2 captures empty spaces as 2
                    captured2++;
                }
                else if (grid[i][j] == 5) {
                    grid[i][j] = 4;  // Player 2's trail becomes Player 2's captured
                    captured2++;
                }
                else if (grid[i][j] == -1) {
                    grid[i][j] = 0;
                }
            }
        }
        isPlayer2moving = false;
    }

    if ((grid[y][x] == 1 || grid[y][x] == 2 || grid[y][x] == 4) && !isPlayer1Dead && isPlayer1moving && isPlayer2moving)
    {
        dx = dy = 0;
        for (int i = 0;i < enemyCount;i++)
            drop(a[i].y / ts, a[i].x / ts);

        for (int i = 0;i < M;i++) {
            for (int j = 0;j < N;j++) {
                if (grid[i][j] == 0) {
                    grid[i][j] = 2;  // Player 1 captures empty spaces as 4
                    captured1++;
                }
                else if (grid[i][j] == 3) {
                    grid[i][j] = 2;  // Player 1's trail becomes Player 1's captured
                    captured1++;
                }
                else if (grid[i][j] == -1) {
                    grid[i][j] = 0;
                }
            }
        }
        isPlayer1moving = false;

    }

    if ((grid[y2][x2] == 1 || grid[y2][x2] == 4 || grid[y2][x2] == 2) && !isPlayer2Dead && isPlayer2moving && isPlayer1moving) {
        dx2 = dy2 = 0;

        for (int i = 0;i < enemyCount;i++)
            drop(a[i].y / ts, a[i].x / ts);

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 0) {
                    grid[i][j] = 4;  // Player 2 captures empty spaces as 2
                    captured2++;
                }
                else if (grid[i][j] == 5) {
                    grid[i][j] = 4;  // Player 2's trail becomes Player 2's captured
                    captured2++;
                }
                else if (grid[i][j] == -1) {
                    grid[i][j] = 0;
                }
            }
        }
        isPlayer2moving = false;
    }
    
    if (occurence1 > 3) {
        threshold1 = 5;
    }
    else if (occurence1 > 5) {
        multiple1 = 4;
    }
    if (captured1 > threshold1) {
        captured1 *= multiple1;
        occurence1++;
    }
    score1 += captured1;
    captured1 = 0;

    if (occurence2 > 3) {
        threshold2 = 5;
    }
    else if (occurence2 > 5) {
        multiple2 = 4;
    }
    if (captured2 > threshold2) {
        captured2 *= multiple2;
        occurence2++;
    }
    score2 += captured2;
    captured2 = 0;
   
    for (int i = 0;i < enemyCount;i++) {
        if (grid[a[i].y / ts][a[i].x / ts] == 5) {
            isPlayer2Dead = true;
        }
        if (grid[a[i].y / ts][a[i].x / ts] == 3) {
            isPlayer1Dead = true;
        }
    }
        
}
void MultiPlayerMode::render(RenderWindow& window) {
    window.clear();
    // 0 = empty space, 1 = boundary (blue) , 2 = player 1 captured tiles, 3 = player 1 trail (red) , 4 = player 2 captured tiles (green) , 5 = player 2 trail (golden)
    for (int i = 1; i < M; i++)
        for (int j = 0; j < N; j++)
        {
            if (grid[i][j] == 0) {
                continue;
            }
            if (grid[i][j] == 1) {
                sPlayer1.setTextureRect(IntRect(0, 0, ts, ts));
                sPlayer1.setPosition(j * ts, i * ts);
                window.draw(sPlayer1);
            }
            if (grid[i][j] == 2) {
                sPlayer1.setTextureRect(IntRect(60, 0, ts, ts));
                sPlayer1.setPosition(j * ts, i * ts);
                window.draw(sPlayer1);
            }
            if (grid[i][j] == 3) {
                sPlayer2.setTextureRect(IntRect(60, 0, ts, ts));
                sPlayer2.setPosition(j * ts, i * ts);
                window.draw(sPlayer2);
            }
            if (grid[i][j] == 4) {
                sPlayer2.setTextureRect(IntRect(120, 0, ts, ts));
                sPlayer2.setPosition(j * ts, i * ts);
                window.draw(sPlayer2);
            }
            if (grid[i][j] == 5) {
                sPlayer2.setTextureRect(IntRect(120, 0, ts, ts));
                sPlayer2.setPosition(j * ts, i * ts);
                window.draw(sPlayer2);
            }
        }
    if (!isPlayer1Dead) {
        sPlayer1.setTextureRect(IntRect(150, 0, ts, ts));
        sPlayer1.setPosition(x * ts, y * ts);
        window.draw(sPlayer1);
    }

    if (!isPlayer2Dead) {
        sPlayer2.setTextureRect(IntRect(180, 0, ts, ts));
        sPlayer2.setPosition(x2 * ts, y2 * ts);
        window.draw(sPlayer2);
    }

    // Player 1 score section
    score.setFillColor(Color::Red);
    score.setPosition(5, 3);
    score.setCharacterSize(24);
    score.setString("Player 1: ");
    window.draw(score);

    // Player 1 name with "Dead" indicator if applicable
    score.setPosition(120, 3);
    score.setString(profile->getPlayerName(1));
    window.draw(score);

    // If player is dead, show the indicator
    if (isPlayer1Dead) {
        Text deadText;
        deadText.setFont(*score.getFont());
        deadText.setFillColor(Color::Red);
        deadText.setCharacterSize(24);
        deadText.setString(" (Dead)");
        deadText.setPosition(310,3);
        window.draw(deadText);
    }

    // Player 1 score
    score.setPosition(250, 3);
    score.setString(to_string(score1));
    window.draw(score);

    // Player 2 score section
    score.setFillColor(Color::Green);
    score.setPosition(600, 3);
    score.setCharacterSize(24);
    score.setString("Player 2: ");
    window.draw(score);

    // Player 2 name
    score.setPosition(720, 3);
    score.setString(profile->getPlayerName(2));
    window.draw(score);

    // If player is dead, show the indicator
    if (isPlayer2Dead) {
        Text deadText;
        deadText.setFont(*score.getFont());
        deadText.setFillColor(Color::Red);
        deadText.setCharacterSize(24);
        deadText.setString(" (Dead)");
        deadText.setPosition(900, 3);
        window.draw(deadText);
    }

    // Player 2 score
    score.setPosition(820, 3);
    score.setString(to_string(score2));
    window.draw(score);

    sEnemy.rotate(20);
    for (int i = 0; i < enemyCount; i++)
    {
        sEnemy.setPosition(a[i].x, a[i].y);
        window.draw(sEnemy);
    }
    if (!Game) window.draw(sGameover);
}
// Main Loop
void Game::start() {

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed) {
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
    PlayerProfile* players = new PlayerProfile;
    players->ReadPlayers();
    players->display();
    Game game(window, players);
    game.start();


    return 0;
}
bool MatchMaking::matchShown = false;