#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;

//our classes will form the absoilute backbone of the game structure and make the program more concise, helps us avoid using a ridiculous amount of if statements
class Room {
    public:
    string name;
    string description;
    map<string, string> exits;
    string item;
    bool visited;

    Room() {
        name = "";
        description = "";
        item = "";
        visited = false;
    }

    Room(string roomName, string roomDescription, string roomItem = "") {
        name = roomName;
        description = roomDescription;
        item = roomItem;
        visited = false;
    }
};

class Player {
public:
    string location;
    vector<string> inventory;
    int energy;
    int stress;
    int timeLeft;
    int clues;
    bool hasLaptop;

    Player() {
        location = "NAC Lobby";
        energy = 10;
        stress = 0;
        timeLeft = 24;
        clues = 0;
        hasLaptop = false;
    }
};

string lowerText(string text);
int keepAtLeastZero(int number);
void printDivider();
void printHeader(string title);
void printCommandPrompt();
void printIntro();
void printHelp();
void printMap();
void buildWorld(map<string, Room>& rooms);
void connectRooms(map<string, Room>& rooms);
void describeRoom(map<string, Room>& rooms, Player& player);
void showExits(const Room& room);
void showInventory(const Player& player);
void showStatus(const Player& player);
bool hasItem(const Player& player, string itemName);
bool isDirection(string command);
void addItem(Player& player, string itemName);
void takeItem(map<string, Room>& rooms, Player& player);
void movePlayer(map<string, Room>& rooms, Player& player, string direction);
void applyRoomEffect(Player& player, const string& roomName);
void handleSpecialRoom(map<string, Room>& rooms, Player& player);
bool playSecurityCodeGame(Player& player);
bool playCopyCenterGame(Player& player);
void searchLostAndFound(Player& player);
void restPlayer(Player& player);
void printEnding(const Player& player);
bool isGameOver(const Player& player);
int calculateScore(const Player& player);
void processCommand(map<string, Room>& rooms, Player& player, string command, bool& running);

int main() {
    srand(time(0));
    map<string, Room> rooms;
    Player player;
    bool running = true;

    buildWorld(rooms);
    printIntro();
    describeRoom(rooms, player);

    while (running && !isGameOver(player)) {
        string command;
        printCommandPrompt();
        getline(cin, command);
        processCommand(rooms, player, lowerText(command), running);
    }
    printEnding(player);
    return 0;
}

string lowerText(string text) {
    for (int i = 0; i < (int)text.length(); i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = text[i] + 32;
        }
    }
    return text;
}
