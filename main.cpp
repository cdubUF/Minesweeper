#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <chrono>
using namespace std;

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
    }
bool isValid(int x, int y, int row, int col) {
    return (x >= 0 && x < row && y >= 0 && y < col);
}
int countNeighboringMines(int x, int y, const vector<vector<Tile>>& tileData, int row, int col) {
    int count = 0;

    // Define the eight possible directions to check
    int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    for (int i = 0; i < 8; i++) {
        int newX = x + dx[i];
        int newY = y + dy[i];

        // Check if the neighboring cell is valid and contains a mine
        if (isValid(newX, newY, row, col) && tileData[newX][newY].mines) {
            count++;
        }
    }

    return count;
}
void revealCells(int x, int y, vector<vector<sf::Sprite>>& drawnTiles, vector<vector<Tile>>& tileData, int row, int col) {
    // Base case: if the cell is outside the grid or already revealed, stop recursion
    if (!isValid(x, y, row, col) || tileData[x][y].revealedTile || tileData[x][y].flag) {
        return;
    }

    // Reveal the current cell
    drawnTiles[x][y].setTexture(TextureManager::getTexture("tile_revealed"));
    tileData[x][y].revealedTile = true;
    // If the current cell has no neighboring mines, recursively reveal neighboring cells
    if (countNeighboringMines(x, y, tileData, row, col) == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                revealCells(x + i, y + j, drawnTiles, tileData, row, col);
            }
        }
    }
}

map<int, sf::Sprite> parseDigits(sf::Sprite digits){
    map<int, sf::Sprite> digitsMap;

    for(int i = 0; i <= 11; i++){
        sf::IntRect rect(i*21,0,21,32);
        digits.setTextureRect(rect);
        sf::Sprite sprite = digits;
        digitsMap.emplace(i, sprite);
    }

    return digitsMap;
}


int main() {
    // get col row and mine count for correct dimensions
    vector<string> playerTimes;
    vector<string> playerNames;
    string colStr, rowStr, mineStr;
    fstream inFile("config.cfg", ios_base::in);
    getline(inFile, colStr);
    getline(inFile, rowStr);
    getline(inFile, mineStr);
    int col = stoi(colStr);
    int row = stoi(rowStr);
    int mines = stoi(mineStr);

    // reading leaderboard txt;=
    string firstPlayertime, secondPlayertime, thirdPlayertime, fourthPlayertime, fifthPlayertime;
    string playerName1, playerName2, playerName3, playerName4, playerName5;
    fstream leaderboardInFile("leaderboard.txt", ios_base::in);
    if (leaderboardInFile.is_open()){
        string Firstline;
        while(getline(leaderboardInFile, Firstline)) {
            string filler1;
            string filler2;
            istringstream Data(Firstline);
            getline(Data, filler1, ',');
            playerTimes.push_back(filler1);
            getline(Data, filler2);
            playerNames.push_back(filler2);
        }
        leaderboardInFile.close();
    }
    for (int i = 0; i < 5  ; i++){
        cout << playerTimes[i] << ", " << playerNames[i] << endl;
    }



    sf::RenderWindow window(sf::VideoMode(col * 32, row * 32 + 100), "Welcome Window");
    // GAME WINDOW !
    float windowHeight = row * 32 + 100;
    float windowWidth = col * 32;
    cout << row << endl;
    cout << col << endl;
    cout << windowHeight << endl;
    cout << windowWidth << endl;
    sf::Font font;
    font.loadFromFile("font.ttf");
    if (!font.loadFromFile("font.ttf")){
        std::cout << "cant load" << std::endl;
        return 0;
    }
    // "WELCOME TO MINESWEEPER" Text
    sf::Text welcomeText;
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setFont(font);
    welcomeText.setCharacterSize(24);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcomeText.setFillColor(sf::Color::White);

    setText(welcomeText, windowWidth/2.0f, windowHeight/2.0f - 150);
    // enter name text
    sf::Text enterNameText;
    enterNameText.setString("Enter your name:");
    enterNameText.setFont(font);
    enterNameText.setCharacterSize(20);
    enterNameText.setStyle(sf::Text::Bold);
    enterNameText.setFillColor(sf::Color::White);

    setText(enterNameText, windowWidth/2.0f, windowHeight/2.0f - 75);
    // input text for welcome window
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setStyle(sf::Text::Bold);
    inputText.setFillColor(sf::Color::Yellow);
    setText(inputText, windowWidth/2.0f, windowHeight/2.0f - 45);
    inputText.setString("|");
    // leaderboard text
    sf::Text leaderboardText;
    leaderboardText.setString("LEADERBOARD");
    leaderboardText.setFont(font);
    leaderboardText.setCharacterSize(20);
    leaderboardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    leaderboardText.setFillColor(sf::Color::White);
    setText(leaderboardText, ((row * 16) + 50) /2.0, (col * 16)/2.0 - 120);

    // leaderboardstatstext
    sf::Text playerNameText, playerTimeText;
    playerNameText.setFont(font);
    playerTimeText.setFont(font);
    playerNameText.setCharacterSize(18);
    playerTimeText.setCharacterSize(18);
    playerNameText.setFillColor(sf::Color::White);
    playerTimeText.setFillColor(sf::Color::White);
    playerNameText.setStyle(sf::Text::Bold);
    playerTimeText.setStyle(sf::Text::Bold);


    // happy face button
    sf::Texture& faceHappyTexture = TextureManager::getTexture("face_happy");
    sf::Sprite faceHappySprite;
    faceHappySprite.setTexture(faceHappyTexture);
    faceHappySprite.setPosition(col / 2.0 * 32 - 32, 32 * (row + 0.5));
    // face when you lose png
    sf::Texture& faceLoseTexture = TextureManager::getTexture("face_lose");
    sf::Sprite faceLoseSprite;
    faceLoseSprite.setTexture(faceLoseTexture);
    faceLoseSprite.setPosition(col / 2.0 * 32 - 32, 32 * (row + 0.5));

    // face win
    sf::Texture& faceWinTexture = TextureManager::getTexture("face_win");
    sf::Sprite faceWinSprite;
    faceWinSprite.setTexture(faceWinTexture);
    faceWinSprite.setPosition(col / 2.0 * 32 - 32, 32 * (row + 0.5));

    // debug button
    sf::Texture& debugButtonTexture = TextureManager::getTexture("debug");
    sf::Sprite debugButtonSprite;
    debugButtonSprite.setTexture(debugButtonTexture);
    debugButtonSprite.setPosition((col * 32) - 304, 32 * (row + 0.5));

    // pause button
    sf::Texture& pauseButtonTexture = TextureManager::getTexture("pause");
    sf::Sprite pauseButtonSprite;
    pauseButtonSprite.setTexture(pauseButtonTexture);
    pauseButtonSprite.setPosition((col * 32) - 240, 32 * (row + 0.5));
    //leadboard button
    sf::Texture& leaderboardButtonTexture = TextureManager::getTexture("leaderboard");
    sf::Sprite leaderboardButtonSprite;
    leaderboardButtonSprite.setTexture(leaderboardButtonTexture);
    leaderboardButtonSprite.setPosition((col * 32) - 176, 32 * (row + 0.5));
    // flag texture/sprite
    sf::Texture& flagTexture = TextureManager::getTexture("flag");
    sf::Sprite flagSprite;
    flagSprite.setTexture(flagTexture);
    // hidden texture
    sf::Texture& tileHiddenTexture = TextureManager::getTexture("tile_hidden");
    sf::Sprite tileHiddenSprite;
    tileHiddenSprite.setTexture(tileHiddenTexture);
    // mine texture
    sf::Texture& mineTexture = TextureManager::getTexture("mine");
    sf::Sprite mineSprite;
    mineSprite.setTexture(mineTexture);
    // numbers to be displayed on the reveal tiles
    sf::Texture& number1Texture = TextureManager::getTexture("number_1");
    sf::Sprite number1Sprite;
    number1Sprite.setTexture(number1Texture);

    sf::Texture& number2Texture = TextureManager::getTexture("number_2");
    sf::Sprite number2Sprite;
    number2Sprite.setTexture(number2Texture);

    sf::Texture& number3Texture = TextureManager::getTexture("number_3");
    sf::Sprite number3Sprite;
    number3Sprite.setTexture(number3Texture);

    sf::Texture& number4Texture = TextureManager::getTexture("number_4");
    sf::Sprite number4Sprite;
    number4Sprite.setTexture(number4Texture);

    sf::Texture& number5Texture = TextureManager::getTexture("number_5");
    sf::Sprite number5Sprite;
    number5Sprite.setTexture(number5Texture);

    sf::Texture& number6Texture = TextureManager::getTexture("number_6");
    sf::Sprite number6Sprite;
    number6Sprite.setTexture(number6Texture);

    sf::Texture& number7Texture = TextureManager::getTexture("number_7");
    sf::Sprite number7Sprite;
    number7Sprite.setTexture(number7Texture);

    sf::Texture& number8Texture = TextureManager::getTexture("number_8");
    sf::Sprite number8Sprite;
    number8Sprite.setTexture(number8Texture);
    // digits png texture and sprite
    sf::Texture& digitsText = TextureManager::getTexture("digits");
    sf::Sprite digits;
    digits.setTexture(digitsText);

    map<int, sf::Sprite> digitsMap = parseDigits(digits); //map for my digitstimer
    map<int, sf::Sprite> digitsMineCount = parseDigits(digits);
    // play button
    sf::Texture& playButtonTexture = TextureManager::getTexture("play");
    sf::Sprite playButtonSprite;
    playButtonSprite.setTexture(playButtonTexture);
    playButtonSprite.setPosition((col * 32) - 240, 32 * (row + 0.5));

    // variables to help with welcome window logic
    string playerName = "";
    char typedChar;
    bool closed = false;

    // welcome screen
    while(window.isOpen()) {
        sf::Event event;
        setText(inputText, windowWidth/2.0f, windowHeight/2.0f - 45);
        while(window.pollEvent(event)) {
            // allows the x button at the top right of the window to be pressed
            if (event.type == sf::Event::Closed) {
                window.close();
                closed = true;
            }
            // checks if anything is typed
            else if (event.type == sf::Event::TextEntered) {
                // checks if its a letter
                if (event.text.unicode < 128) {
                    typedChar = static_cast<char>(event.text.unicode);

                    // Validate if the character is alphabetical
                        if (isalpha(typedChar)) {
                            if (playerName.length() < 10) {
                                if (playerName.length() == 0) {
                                    // Capitalize the first letter
                                    typedChar = toupper(typedChar);

                                } else {
                                    // changes letter to lower case
                                    typedChar = tolower(typedChar);
                                }

                            }
                            else {
                                break;
                            }
                            playerName += typedChar;
                            inputText.setString(playerName + '|');
                        } else if (typedChar == 8) {  // Backspace
                            if (!playerName.empty()) {
                                playerName.pop_back();

                                inputText.setString(playerName + "|");
                            }
                        }

                }
            }
            // press enter when length is more than 0
            if (event.type == sf::Event::KeyPressed && playerName.length() > 0) { // gets user keyboard input
                if (event.key.code == sf::Keyboard::Enter){
                    window.close();
                }
            }

        }
        window.clear(sf::Color::Blue);
        window.draw(welcomeText);
        window.draw(enterNameText);
        window.draw(inputText);
        window.display();
     }
    if (closed){
        closed = false;
        return 0;
    }
    // window made for the game !
    sf::RenderWindow mainWindow(sf::VideoMode(col * 32, row * 32 + 100), "Game Window");
    vector<vector<sf::Sprite>> drawnTiles;
    vector<vector<sf::Sprite>> pausedTiles;
    vector<vector<Tile>> tileData;


    // load vectors
        for(int i = 0; i < row; i++){
            vector<Tile> pushMe;
            for(int j = 0; j < col; j++){
                Tile temp;
                pushMe.push_back(temp);
            }
            tileData.push_back(pushMe);
        }


    // load mines
        int mineCount = 0;
        random_device rd;
        uniform_int_distribution<int>x(0,row-1);
        uniform_int_distribution<int>y(0,col-1);
        while(mineCount != mines) {
            int width = x(rd);
            int height = y(rd);
            if (tileData[width][height].mines) {

            }
            else{
                tileData[width][height].mines = true;
                mineCount ++;
            }
        }


// tiles implementation
    for (int i = 0; i < row; i++) {
        vector<sf::Sprite> pushMe;

        for (int j = 0; j < col; j++) { // Use j for iterating over columns
            sf::Texture& tileHiddenTexture = TextureManager::getTexture("tile_hidden");
            sf::Sprite tileHiddenSprite;
            tileHiddenSprite.setTexture(tileHiddenTexture);
            tileHiddenSprite.setPosition(i * 32, j * 32);
            pushMe.push_back(tileHiddenSprite); // Push the sprite into the inner vector
        }
        drawnTiles.push_back(pushMe);// Push the inner vector into the outer vector
    }
    for (int i = 0; i < row; i++) {
        vector<sf::Sprite> pushMe;

        for (int j = 0; j < col; j++) { // Use j for iterating over columns
            sf::Texture& tileRevealedTexture = TextureManager::getTexture("tile_revealed");
            sf::Sprite tileRevealedSprite;
            tileRevealedSprite.setTexture(tileRevealedTexture);
            tileRevealedSprite.setPosition(i * 32, j * 32);
            pushMe.push_back(tileRevealedSprite); // Push the sprite into the inner vector
        }
        pausedTiles.push_back(pushMe); // Push the inner vector into the outer vector
    }

    // check for certain stuff lol
    bool debugButtonPressed = false;
    int numDebugButtonPressed = 1;
    int tilesRevealedCount = 0;
    bool flagDrawn = false;
    bool win = false;
    bool lose = false;
    bool recursiveReveal = false;
    bool paused = false;
    bool faceHappyClicked = false;
    int mineCounter = mines;
    bool leaderboard = false;
    bool stopTimer = false;
    string mineCounterStr = to_string(mines);

    // clock implementation
    auto start_time = chrono::high_resolution_clock::now();
    auto pauseTime = chrono::high_resolution_clock::now();
    auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count();

    // game window implementation
    while(mainWindow.isOpen()) {
        sf::Event event;
        float tileWidth = 32.0f;
        float tileHeight = 32.0f;
        int subtractMe = 0;
        mainWindow.clear(sf::Color::White);
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                sf::Sprite& currentTile = drawnTiles[i][j];
                currentTile.setPosition(static_cast<float>(j) * tileWidth, static_cast<float>(i)  * tileHeight);
                mainWindow.draw(currentTile);
            }
        }
        // mine counter
        int negativeSign = 0;
        int firstNum = mines / 10 % 10; //minutes index 0
        int secondNum = mines % 10;
        digitsMineCount[negativeSign].setPosition(12, 32 * (row +0.5) + 16);
        mainWindow.draw(digitsMineCount[negativeSign]);
        digitsMineCount[firstNum].setPosition(33, 32 * (row +0.5) + 16);
        mainWindow.draw(digitsMineCount[firstNum]);
        digitsMineCount[secondNum].setPosition(54, 32 * (row +0.5) + 16);
        mainWindow.draw(digitsMineCount[secondNum]);

        mainWindow.draw(faceHappySprite);
        mainWindow.draw(debugButtonSprite);
        mainWindow.draw(pauseButtonSprite);
        mainWindow.draw(leaderboardButtonSprite);
        while(mainWindow.pollEvent(event)) {

            if(event.type == sf::Event::Closed) {
                mainWindow.close();
                closed = true;
            }
            if(event.type == sf::Event::MouseButtonPressed){
                sf::Vector2i mousePosition;
                mousePosition = sf::Mouse::getPosition(mainWindow);
                cout << mousePosition.x << "," << mousePosition.y << endl;
                // flag placing check!

                if(event.mouseButton.button == sf::Mouse::Right){
                    if(paused){
                        break;
                    }
                    if (win){
                        break;
                    }
                    if (lose){
                        break;
                    }
                    for(int i = 0; i < row; i++){
                        for (int j = 0; j < col; j++){
                            if (drawnTiles[i][j].getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)) && !tileData[i][j].flag && !tileData[i][j].revealedTile){
                                tileData[i][j].flag = true;
                                flagDrawn = true;
                                mineCounter --;
                            }
                            else if (drawnTiles[i][j].getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)) && tileData[i][j].flag){
                                tileData[i][j].flag = false;
                                mineCounter ++;
                            }
                        }
                    }
                    cout << "Minecounter: " << mineCounter << endl;
                }
                // checks for debug button!
                else if (event.mouseButton.button == sf::Mouse::Left && debugButtonSprite.getGlobalBounds().contains((mousePosition.x),(mousePosition.y))){
                    if (paused){
                        break;
                    }
                    if (lose){
                        break;
                    }
                    if (win){
                        break;
                    }
                    debugButtonPressed = !debugButtonPressed;
                    numDebugButtonPressed ++;
                }
                // checks if hidden tile is clicked !
                else if(event.mouseButton.button == sf::Mouse::Left && mousePosition.x <= col * 32 && mousePosition.y <= row * 32){
                    if(paused){
                        break;
                    }
                    if (lose){
                        break;
                    }
                    if (win){
                        break;
                    }
                    int j = mousePosition.x / 32;
                    int i = mousePosition.y / 32;
                    cout << "Clicked on cell: (" << i << ", " << j << ")" << endl;
                    cout << "Revealed: " << tileData[i][j].revealedTile << ", Flag: " << tileData[i][j].flag << endl;
                    if(!tileData[i][j].revealedTile && !tileData[i][j].flag){
                        // recursive part check for neighboring mines
                        if(!tileData[i][j].mines){
                            tileData[i][j].neighboringMines = countNeighboringMines(i, j, tileData, row, col);
                            revealCells(i , j, drawnTiles, tileData, row, col);
                        }
                        else{
                            lose = !lose;
                            stopTimer = !stopTimer;
                            pauseTime = chrono::high_resolution_clock::now();
                        }
                    }
                    else if(!tileData[i][j].revealedTile && tileData[i][j].flag && tileData[i][j].mines){
                        lose = !lose;
                        stopTimer = !stopTimer;
                        pauseTime = chrono::high_resolution_clock::now();
                    }
                }
                // checking if play button is clicked
                else if(event.mouseButton.button == sf::Mouse::Left && playButtonSprite.getGlobalBounds().contains((mousePosition.x),(mousePosition.y))){
                    if (lose){
                        break;
                    }
                    if (win){
                        break;
                    }
                    paused = !paused;
                    if(paused) {
                        pauseTime = chrono::high_resolution_clock::now();

                    }else{
                        //unpaused
                        auto unPausedTime = chrono::high_resolution_clock ::now();
                        elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count(); //Addition is necessary for when hitting the pause button more than once
                        cout << elapsed_paused_time % 60 << " " << endl;
                    }
                }
                else if(event.mouseButton.button == sf::Mouse::Left && faceHappySprite.getGlobalBounds().contains((mousePosition.x),(mousePosition.y))){
                    faceHappyClicked = !faceHappyClicked;
                }
                else if (event.mouseButton.button == sf::Mouse::Left && leaderboardButtonSprite.getGlobalBounds().contains((mousePosition.x),(mousePosition.y))){
                    leaderboard = !leaderboard;
                    sf::RenderWindow leaderboardWindow(sf::VideoMode((row * 16) + 50, col * 16), "Leaderboard Window");
                    while(leaderboardWindow.isOpen()){
                        sf::Event event;
                        leaderboardWindow.clear(sf::Color::Blue);
                        leaderboardWindow.draw(leaderboardText);
                        while(leaderboardWindow.pollEvent(event)){
                            if (event.type == sf::Event::Closed) {
                                leaderboardWindow.close();
                                leaderboard = false;
                            }

                        }
                        for (int i = 0; i < playerNames.size(); i++) {
                            // Set text for player name and time
                            playerNameText.setString(playerNames[i]);
                            playerTimeText.setString(playerTimes[i]);

                            // Calculate positions for player name and time
                            float xPositionName = ((row * 16) + 50) / 2.0;
                            float yPositionName = ((col * 16) / 2.0) - 20;

                            float xPositionTime = xPositionName + 150; // Adjust as needed
                            float yPositionTime = yPositionName; // Adjust as needed

                            // Set positions for player name and time
                            setText(playerNameText, xPositionName, yPositionName);
                            setText(playerTimeText, xPositionTime, yPositionTime);

                            // Draw the text
                            leaderboardWindow.draw(playerNameText);
                            leaderboardWindow.draw(playerTimeText);
                        }
                        leaderboardWindow.display();
                    }
                }
            }
        }
        if (closed){
            closed = false;
            return 0;
        }
        auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
        int total_time = game_duration.count(); // necessary to subtract elapsed time later because "game_duration.count()" is const

        int minutes;
        int seconds;
        int minutes0 = minutes / 10 % 10; //minutes index 0
        int minutes1 = minutes % 10; // minutes index 1
        int seconds0 = seconds / 10 % 10; // seconds index 0
        int seconds1 = seconds % 10; // seconds index 1


        digitsMap[minutes0].setPosition((col * 32) - 97, 32 * (row + 0.5f) + 16);
        mainWindow.draw(digitsMap[minutes0]);

        digitsMap[minutes1].setPosition((col * 32) - 76, 32 * (row + 0.5f) + 16);
        mainWindow.draw(digitsMap[minutes1]);

        digitsMap[seconds0].setPosition((col * 32) - 54, 32 * (row + 0.5f) + 16);
        mainWindow.draw(digitsMap[seconds0]);

        digitsMap[seconds1].setPosition((col * 32) - 33, 32 * (row + 0.5f) + 16);
        mainWindow.draw(digitsMap[seconds1]);
        mainWindow.draw(pauseButtonSprite);
        // checks for how many to draw;
        for(int i = 0; i < row; i++){
            for(int j = 0; j < col; j++){
                int neighboringMines = countNeighboringMines(i,j,tileData, row, col);
                if(neighboringMines > 0 && tileData[i][j].revealedTile){
                    tileData[i][j].neighboringMines = neighboringMines;
                    string num = to_string(tileData[i][j].neighboringMines);
                    if(neighboringMines == 1){
                        sf::Sprite& currentTile = number1Sprite;
                        currentTile.setPosition(drawnTiles[i][j].getPosition().x,drawnTiles[i][j].getPosition().y);
                        mainWindow.draw(currentTile);
                    }
                    else if (neighboringMines == 2){
                        sf::Sprite& currentTile = number2Sprite;
                        currentTile.setPosition(drawnTiles[i][j].getPosition().x,drawnTiles[i][j].getPosition().y);
                        mainWindow.draw(currentTile);
                    }
                    else if (neighboringMines == 3){
                        sf::Sprite& currentTile = number3Sprite;
                        currentTile.setPosition(drawnTiles[i][j].getPosition().x,drawnTiles[i][j].getPosition().y);
                        mainWindow.draw(currentTile);
                    }
                    else if (neighboringMines == 4){
                        sf::Sprite& currentTile = number4Sprite;
                        currentTile.setPosition(drawnTiles[i][j].getPosition().x,drawnTiles[i][j].getPosition().y);
                        mainWindow.draw(currentTile);
                    }
                    else if (neighboringMines == 5){
                        sf::Sprite& currentTile = number5Sprite;
                        currentTile.setPosition(drawnTiles[i][j].getPosition().x,drawnTiles[i][j].getPosition().y);
                        mainWindow.draw(currentTile);
                    }
                    else if (neighboringMines == 6){
                        sf::Sprite& currentTile = number6Sprite;
                        currentTile.setPosition(drawnTiles[i][j].getPosition().x,drawnTiles[i][j].getPosition().y);
                        mainWindow.draw(currentTile);
                    }
                    else if (neighboringMines == 7){
                        sf::Sprite& currentTile = number7Sprite;
                        currentTile.setPosition(drawnTiles[i][j].getPosition().x,drawnTiles[i][j].getPosition().y);
                        mainWindow.draw(currentTile);
                    }
                    else if (neighboringMines == 8){
                        sf::Sprite& currentTile = number8Sprite;
                        currentTile.setPosition(drawnTiles[i][j].getPosition().x,drawnTiles[i][j].getPosition().y);
                        mainWindow.draw(currentTile);
                    }
                }
            }
        }
        // checks if they won
        int tempCount = 0;
        for (int i = 0; i < row; i ++){
            for (int j = 0; j < col; j++){
                if (tileData[i][j].revealedTile){
                    tempCount ++;
                }
            }
            if (tempCount == row * col - mines){
                win = true;
                leaderboard = true;
                stopTimer = true;
            }
        }
        if (win){
            for(int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                    if (tileData[i][j].mines) {
                        sf::Sprite &currentTile = flagSprite;
                        currentTile.setPosition(drawnTiles[i][j].getPosition().x, drawnTiles[i][j].getPosition().y);
                        mainWindow.draw(currentTile);
                    }
                }
            }
            mainWindow.draw(faceWinSprite);
        }
        if(lose){
            for(int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                    if (tileData[i][j].mines) {
                        sf::Sprite &currentTile = mineSprite;
                        currentTile.setPosition(drawnTiles[i][j].getPosition().x, drawnTiles[i][j].getPosition().y);
                        mainWindow.draw(currentTile);
                    }
                }
            }
            mainWindow.draw(faceLoseSprite);
        }
        // checks if debug is true
        if(debugButtonPressed && numDebugButtonPressed % 2 == 0){
            for(int i = 0; i < row; i++){
                for(int j = 0; j < col; j++){
                    if(tileData[i][j].mines){
                        sf::Sprite& currentTile = mineSprite;
                        currentTile.setPosition(drawnTiles[i][j].getPosition().x, drawnTiles[i][j].getPosition().y);
                        mainWindow.draw(currentTile);
                    }
                }
            }
        }
        if (flagDrawn){
            for(int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                    if (tileData[i][j].flag) {
                        sf::Sprite &currentTile = flagSprite;
                        currentTile.setPosition(drawnTiles[i][j].getPosition().x, drawnTiles[i][j].getPosition().y);
                        mainWindow.draw(currentTile);
                    }
                }
            }
            if (mineCounter >= 0){
                int neg = 0;
                int temp1 = mineCounter/ 10 % 10; //minutes index 0
                int temp2 = mineCounter % 10;
                digitsMineCount[neg].setPosition(12, 32 * (row +0.5) + 16);
                mainWindow.draw(digitsMineCount[neg]);
                digitsMineCount[temp1].setPosition(33, 32 * (row +0.5) + 16);
                mainWindow.draw(digitsMineCount[temp1]);
                digitsMineCount[temp2].setPosition(54, 32 * (row +0.5) + 16);
                mainWindow.draw(digitsMineCount[temp2]);
            }
            else{
                int neg = 10;
                int temp1 = mineCounter / 10 % 10 * -1; //minutes index 0
                int temp2 = mineCounter % 10 * -1;
                cout << "temp1: " << temp1 << "temp2: " << temp2 << endl;
                digitsMineCount[10].setPosition(12, 32 * (row +0.5) + 16);
                mainWindow.draw(digitsMineCount[10]);
                digitsMineCount[temp1].setPosition(33, 32 * (row +0.5) + 16);
                mainWindow.draw(digitsMineCount[temp1]);
                digitsMineCount[temp2].setPosition(54, 32 * (row +0.5) + 16);
                mainWindow.draw(digitsMineCount[temp2]);

            }
        }
        if (faceHappyClicked){
            for(int i = 0; i < row; i++){
                for(int j = 0; j < col; j++){
                    tileData[i][j].mines = false;
                    tileData[i][j].flag = false;
                    tileData[i][j].revealedTile = false;
                    drawnTiles[i][j] = tileHiddenSprite;
                    drawnTiles[i][j].setPosition(j * 32, i  * 32);
                    mainWindow.draw(drawnTiles[i][j]);
                }
            }
            int mineCountDupe = 0;
            while(mineCountDupe != mines) {
                int i = x(rd);
                int j = y(rd);
                if (tileData[i][j].mines) {
                }
                else{
                    tileData[i][j].mines = true;
                    mineCountDupe ++;
                }
            }
            faceHappyClicked = false;
            lose = false;
            win = false;
            leaderboard = false;
            stopTimer = false;
            int temp1 = mines / 10 % 10; //minutes index 0
            int temp2 = mines % 10;
            mineCounter = mines;
            digitsMineCount[temp1].setPosition(33, 32 * (row + 0.5) + 16);
            mainWindow.draw(digitsMineCount[temp1]);
            digitsMineCount[temp2].setPosition(54, 32 * (row + 0.5) + 16);
            mainWindow.draw(digitsMineCount[temp2]);
            start_time = chrono::high_resolution_clock::now();
            pauseTime = chrono::high_resolution_clock::now();
            elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start_time).count();
        }

        if(!paused) {
            //enters if the game is NOT paused. This is the condition that keeps the timer from incrementing when paused.
            total_time =  total_time - elapsed_paused_time; //
            minutes = total_time / 60;
            seconds = total_time % 60;
        }

        //"separating" the integers. So.... 68 -> seconds0 = 6 and seconds1 = 8

        if(paused){
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                    sf::Sprite& currentTile = pausedTiles[i][j];
                    currentTile.setPosition(static_cast<float>(j) * tileWidth, static_cast<float>(i)  * tileHeight);
                    mainWindow.draw(currentTile);
                }
            }
            mainWindow.draw(playButtonSprite);
        }
        mainWindow.display();


    }

    return 0;
}