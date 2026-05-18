#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;

class Room { // setting up the skeleton of every room, whatit should contain and show
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

    Player() { // our starting point and stats w/ location energy and other values
        location = "NAC Lobby";
        energy = 10;
        stress = 0;
        timeLeft = 24;
        clues = 0;
        hasLaptop = false;
    }
};

string lowerText(string text); // function declarations for what will be used throughout the games very needed
int keepAtLeastZero(int number);
void printDivider();
void printHeader(string title);
void printCommandPrompt();
void printIntro();
void printHelp();
void printCoffeeMugArt();
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

int main() { // game begins here, srand randomizes the mini games and the rest is just setting up the world and running the game loop until the game is over, then it prints the ending
    srand(time(0));
    map<string, Room> rooms;
    Player player;
    bool running = true;

    buildWorld(rooms);
    printIntro();
    describeRoom(rooms, player);

    while (running && !isGameOver(player)) { // main game loop, game will run until the player wins or loses and eventually print end
        string command;
        printCommandPrompt();
        getline(cin, command);
        processCommand(rooms, player, lowerText(command), running);
    }
    printEnding(player);
    return 0;
}

string lowerText(string text) { // this function just converts any text to lowercase, so that you can input commands in any case and it will still work, making the game more user friendly and less strict on how you type commands
    for (int i = 0; i < (int)text.length(); i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = text[i] + 32;
        }
    }
    return text;
}
int keepAtLeastZero(int number) { // this function is used to make sure that certain stats like energy and stress don't go below zero, which would not make sense in the context of the game
    if (number < 0) {
        return 0;
    }
    return number;
}

void printDivider() {
    cout << "\n------------------------------------------------------------\n";
}

void printHeader(string title) {
    printDivider();
    cout << title << "\n";
    printDivider();
}

void printCommandPrompt() {
    cout << "\nCommand > ";
}

void printLaptopArt() { // ascii art 
    cout << "      ______________________\n";
    cout << "     |  CCNY LOGIN SCREEN   |\n";
    cout << "     |  laptop missing...   |\n";
    cout << "     |______________________|\n";
    cout << "        /________________\\\n";
}

void printCoffeeMugArt() { // more ascii art to add some atmosphere
    cout << "          (  )   (   )  )\n";
    cout << "           ) (   )  (  (\n";
    cout << "           ( )  (    ) )\n";
    cout << "        __________________\n";
    cout << "       |                  |__\n";
    cout << "       |      COFFEE      |  )\n";
    cout << "       |__________________|--\n";
}

void printIntro() { // the intro sets the scene for the game and gives the player a brief overview of the story and what they need to do, as well as how to access help and the map
    printHeader("CCNY Laptop Adventure");
    printLaptopArt();
    cout << "Your laptop vanished somewhere in the NAC before a major deadline.\n";
    cout << "Find clues, manage your stress, and recover it before campus closes.\n";
    cout << "\nType 'help' for commands or 'map' to view campus.\n";
}

void printHelp() { // this will list all the commands the player can use, including movement, actions, info, and how to exit the game, so that they can refer back to it whenever they need a reminder on what they can do in the game
    printHeader("Commands");
    cout << "Movement:  north | south | east | west | up | down\n";
    cout << "           You can also type go north, go south, etc.\n";
    cout << "Actions:   look | take | rest\n";
    cout << "Info:      inventory | status | map | help\n";
    cout << "Exit:      quit\n";
}
void printMap() {
    printHeader("Campus Map");
    cout << "Rooftop -- Rooftop Stairwell\n";
    cout << "   |             |\n";
    cout << "Library Second Floor -- Quiet Study Room -- Computer Lab\n";
    cout << "   |                                      |\n";
    cout << "Library Entrance -- NAC Lobby -- Cafeteria -- Student Lounge\n";
    cout << "   |              |            |              |\n";
    cout << "Security Desk -- Elevator Bank -- Basement Hallway -- Copy Center\n";
    cout << "   |              |            |              |\n";
    cout << "Lost and Found -- Lecture Halls -- Professor's Office -- Auditorium Hall\n";
    cout << "                  |            |\n";
    cout << "Bathroom -- Rear Exit Staircase -- Outdoor Plaza\n";
}
void buildWorld(map<string, Room>& rooms) { // this function creates all rooms and their descriptions, items, and then calls connectRooms to set up how they are connected to each other, this is the foundation of the game world and everything else builds off of this
    rooms["NAC Lobby"] = Room("NAC Lobby", "Students walk around casually, friend groups laughing helplessly. Nothing is funny to you, however, as your laptop is missing.", "student ID");
    rooms["Library Entrance"] = Room("Library Entrance", "Security stands behind the counter and also mentions he thinks he saw a laptop around earlier.", "library note");
    rooms["Library Second Floor"] = Room("Library Second Floor", "The shelves in this room always make it feel like a maze; unless you're 7ft and can see over everything.");
    rooms["Quiet Study Room"] = Room("Quiet Study Room", "The whiteboards here still have traces of previous students' panic sessions, including horrific physics equations.", "charger");
    rooms["Computer Lab"] = Room("Computer Lab", "Rows of desktops glow quietly. A login screen waits for the right clue.");
    rooms["Cafeteria"] = Room("Cafeteria", "The smell of cofee, chicken, and pizza fills the air while you overhear a million conversations.", "coffee");
    rooms["Student Lounge"] = Room("Student Lounge", "A couch, a vending machine, and several students trying their hardest to stay awake.", "receipt");
    rooms["Security Desk"] = Room("Security Desk", "A guard watches the camera wall and asks for proof before sharing details.");
    rooms["Elevator Bank"] = Room("Elevator Bank", "Why bother, you already know these elevators are broken most of the time.");
    rooms["Basement Hallway"] = Room("Basement Hallway", "The hallway hums with pipes and flourescent lights that look older than your parents.");
    rooms["Lecture Halls"] = Room("Lecture Halls", "A lecture just ended, leaving behind chalk dust and abandoned handouts.", "class schedule");
    rooms["Professor's Office"] = Room("Professor's Office", "Your professor has seen your panic and offers a sympathetic gesture.", "extension email");
    rooms["Bathroom"] = Room("Bathroom", "The best place to recover and gather your thoughts.");
    rooms["Rear Exit Staircase"] = Room("Rear Exit Staircase", "The concrete stairs seem to go on forever alongside the metal grates stopping anything from falling down the middle.");
    rooms["Outdoor Plaza"] = Room("Outdoor Plaza", "The lights are bright out here and the wind is getting stronger.", "umbrella");
    rooms["Rooftop"] = Room("Rooftop", "The skyline looks calm, which feels personally rude right now.");
    rooms["Copy Center"] = Room("Copy Center", "Printers chatter like tiny engines and a staff member offers to help if you solve a jam.");
    rooms["Lost and Found"] = Room("Lost and Found", "Shelves of forgotten bottles, scarves, notebooks, and maybe your future.");
    rooms["Rooftop Stairwell"] = Room("Rooftop Stairwell", "A narrow stairwell connects the roof to the quieter upper floors.", "keycard");
    rooms["Auditorium Hall"] = Room("Auditorium Hall", "Posters advertise events you would enjoy if your laptop were not missing.");

    connectRooms(rooms);
}
void connectRooms(map<string, Room>& rooms) { // This is critical; it connects rooms together and creates the world. Each room has exits that lead to other rooms, and this function sets up all those connections.
    rooms["NAC Lobby"].exits["north"] = "Library Entrance";
    rooms["NAC Lobby"].exits["east"] = "Cafeteria";
    rooms["NAC Lobby"].exits["south"] = "Elevator Bank";

    rooms["Library Entrance"].exits["south"] = "NAC Lobby";
    rooms["Library Entrance"].exits["up"] = "Library Second Floor";
    rooms["Library Entrance"].exits["west"] = "Security Desk";

    rooms["Library Second Floor"].exits["down"] = "Library Entrance";
    rooms["Library Second Floor"].exits["east"] = "Quiet Study Room";
    rooms["Library Second Floor"].exits["up"] = "Rooftop Stairwell";

    rooms["Quiet Study Room"].exits["west"] = "Library Second Floor";
    rooms["Quiet Study Room"].exits["east"] = "Computer Lab";

    rooms["Computer Lab"].exits["west"] = "Quiet Study Room";
    rooms["Computer Lab"].exits["south"] = "Student Lounge";

    rooms["Cafeteria"].exits["west"] = "NAC Lobby";
    rooms["Cafeteria"].exits["east"] = "Student Lounge";
    rooms["Cafeteria"].exits["south"] = "Basement Hallway";

    rooms["Student Lounge"].exits["west"] = "Cafeteria";
    rooms["Student Lounge"].exits["north"] = "Computer Lab";
    rooms["Student Lounge"].exits["south"] = "Copy Center";

    rooms["Security Desk"].exits["east"] = "Library Entrance";
    rooms["Security Desk"].exits["south"] = "Lost and Found";

    rooms["Elevator Bank"].exits["north"] = "NAC Lobby";
    rooms["Elevator Bank"].exits["east"] = "Basement Hallway";
    rooms["Elevator Bank"].exits["south"] = "Lecture Halls";

    rooms["Basement Hallway"].exits["west"] = "Elevator Bank";
    rooms["Basement Hallway"].exits["north"] = "Cafeteria";
    rooms["Basement Hallway"].exits["east"] = "Copy Center";

    rooms["Lecture Halls"].exits["north"] = "Elevator Bank";
    rooms["Lecture Halls"].exits["east"] = "Professor's Office";
    rooms["Lecture Halls"].exits["south"] = "Rear Exit Staircase";

    rooms["Professor's Office"].exits["west"] = "Lecture Halls";
    rooms["Professor's Office"].exits["south"] = "Outdoor Plaza";
    rooms["Professor's Office"].exits["east"] = "Auditorium Hall";

    rooms["Bathroom"].exits["east"] = "Rear Exit Staircase";
    rooms["Rear Exit Staircase"].exits["west"] = "Bathroom";
    rooms["Rear Exit Staircase"].exits["north"] = "Lecture Halls";
    rooms["Rear Exit Staircase"].exits["east"] = "Outdoor Plaza";

    rooms["Outdoor Plaza"].exits["west"] = "Rear Exit Staircase";
    rooms["Outdoor Plaza"].exits["north"] = "Professor's Office";

    rooms["Rooftop"].exits["east"] = "Rooftop Stairwell";
    rooms["Rooftop Stairwell"].exits["west"] = "Rooftop";
    rooms["Rooftop Stairwell"].exits["down"] = "Library Second Floor";

    rooms["Copy Center"].exits["west"] = "Basement Hallway";
    rooms["Copy Center"].exits["north"] = "Student Lounge";
    rooms["Copy Center"].exits["south"] = "Auditorium Hall";

    rooms["Lost and Found"].exits["north"] = "Security Desk";
    rooms["Lost and Found"].exits["east"] = "Lecture Halls";

    rooms["Auditorium Hall"].exits["west"] = "Professor's Office";
    rooms["Auditorium Hall"].exits["north"] = "Copy Center";
}
void describeRoom(map<string, Room>& rooms, Player& player) {
    Room& room = rooms[player.location];
    printHeader(room.name);
    if (room.name == "Cafeteria") {
        printCoffeeMugArt();
    }
    cout << room.description << "\n";

    if (!room.item.empty()) {
        cout << "\nItem here: " << room.item << "\n";
    }

    showExits(room);
    room.visited = true;
}

void showExits(const Room& room) { // this function lists the exits available in the current room, so that the player knows where they can go from there, and it formats it nicely with separators
    cout << "\nExits: ";
    for (map<string, string>::const_iterator exit = room.exits.begin(); exit != room.exits.end(); exit++) {
        cout << exit->first;
        map<string, string>::const_iterator nextExit = exit;
        nextExit++;
        if (nextExit != room.exits.end()) {
            cout << " | ";
        }
    }
    cout << "\n";
}

void showInventory(const Player& player) { // this handles the inventory command 
    printHeader("Inventory");

    if (player.inventory.empty()) {
        cout << "You are not carrying anything yet.\n";
        return;
    }

    cout << "You are carrying:\n";
    for (int i = 0; i < (int)player.inventory.size(); i++) {
        cout << "  " << i + 1 << ". " << player.inventory[i] << "\n";
    }
}

void showStatus(const Player& player) { // similarly this handles status command and includes stats such as energy, stress, time left, clues, and score, 
    printHeader("Status");
    cout << "Energy:   " << player.energy << "\n";
    cout << "Stress:   " << player.stress << "\n";
    cout << "Time:     " << player.timeLeft << "\n";
    cout << "Clues:    " << player.clues << "\n";
    cout << "Score:    " << calculateScore(player) << "\n";
}

bool hasItem(const Player& player, string itemName) {
    for (int i = 0; i < (int)player.inventory.size(); i++) {
        if (player.inventory[i] == itemName) {
            return true;
        }
    }
    return false;
}
bool isDirection(string command) {
    return command == "north" || command == "south" || command == "east" || command == "west" || command == "up" || command == "down";
}

void addItem(Player& player, string itemName) {
    if (!hasItem(player, itemName)) {
        player.inventory.push_back(itemName);
    }
}

void takeItem(map<string, Room>& rooms, Player& player) { // this function allows the player to take an item from the room they are currently in, if there is one, and adds it to their inventory, while also increasing their clues and removing the item from the room
    Room& room = rooms[player.location];

    if (room.item.empty()) {
        cout << "\nThere is nothing useful to take here.\n";
        return;
    }

    cout << "\nYou take the " << room.item << ".\n";
    addItem(player, room.item);
    player.clues++;
    room.item = "";
}

void movePlayer(map<string, Room>& rooms, Player& player, string direction) { // handles player movement 
    Room& room = rooms[player.location];

    if (room.exits.count(direction) == 0) {
        cout << "\nYou cannot go that way. Check the listed exits.\n";
        return;
    }

    player.location = room.exits[direction];
    player.energy--;
    player.timeLeft--;
    applyRoomEffect(player, player.location);
    describeRoom(rooms, player);
    handleSpecialRoom(rooms, player);
}
void applyRoomEffect(Player& player, const string& roomName) {
    if (roomName == "Cafeteria" && hasItem(player, "coffee")) {
        player.energy++;
    }
    if (roomName == "Bathroom") {
        player.stress = keepAtLeastZero(player.stress - 2);
    }
    if (roomName == "Basement Hallway") {
        player.stress++; // basements are creepy so it increases stress just by being there, adding a bit of challenge and atmosphere to that part of the game
    }
}

void handleSpecialRoom(map<string, Room>& rooms, Player& player) { // this function checks if player is in a special room that has a mini game or special interaction, and if so, it triggers that interaction, such as the security code game in the computer lab, the printer jam game in the copy center, and searching lost and found for the laptop
    if (player.location == "Computer Lab" && !hasItem(player, "camera clue")) {
        if (playSecurityCodeGame(player)) {
            addItem(player, "camera clue");
            player.clues++;
        }
    }

    if (player.location == "Copy Center" && !hasItem(player, "printed flyer")) { // checks if the player is in the right room to trigger the printer jam mini game
        if (playCopyCenterGame(player)) {
            addItem(player, "printed flyer");
            player.clues++;
        }
    }

    if (player.location == "Lost and Found") {
        searchLostAndFound(player);
    }
}
bool playSecurityCodeGame(Player& player) { // randomizer creates the code for security terminal game, player can guess and stress will go up if its wrong
    int code = rand() % 5 + 1;
    int guess;

    printHeader("Mini-Game");
    cout << "Guess the security terminal code from 1 to 5.\n";
    cout << "Guess: ";
    cin >> guess;
    cin.ignore();

    if (guess == code) {
        cout << "\nCorrect. The camera log shows someone carrying your laptop toward Lost and Found.\n";
        return true;
    }

    cout << "\nWrong code. The correct code was " << code << ". Your stress rises.\n";
    player.stress += 2;
    return false;
}
bool playCopyCenterGame(Player& player) {
    int jamSide = rand() % 2;
    string choice;

    printHeader("Mini-Game");
    cout << "The printer is jammed. Pull from the left or right tray? ";
    getline(cin, choice);
    choice = lowerText(choice);

    if ((jamSide == 0 && choice == "left") || (jamSide == 1 && choice == "right")) {
        cout << "\nThe jam clears. You print a flyer with your laptop description.\n";
        return true;
    }

    cout << "\nThe paper tears and everyone hears it. Stress goes up.\n";
    player.stress++; // penalizing the player for a wrong choice in the mini game by increasing stress, too much stress will end game
    return false;
}
void searchLostAndFound(Player& player) {
    if (player.hasLaptop) {
        return;
    }

    if (player.clues >= 4 && hasItem(player, "student ID")) {
        cout << "\nWith enough clues and your ID, Lost and Found releases your laptop.\n";
        player.hasLaptop = true;
        return;
    }

    cout << "\nThe staff needs your ID and at least four clues before handing over any laptop.\n";
}

void restPlayer(Player& player) {
    if (player.timeLeft <= 2) {
        cout << "\nThere is not enough time to rest.\n";
        return;
    }
     player.energy += 2;
    player.stress = keepAtLeastZero(player.stress - 1);
    player.timeLeft -= 2;
    cout << "\nYou take a short pause and steady yourself.\n";
}

int calculateScore(const Player& player) { // gives the players performance a final score based on clues, energy, time left, and more
    int score = player.clues * 15 + player.energy * 3 + player.timeLeft * 2 - player.stress * 4;

    if (player.hasLaptop) {
        score += 100;
    }
    if (score < 0) {
        score = 0;
    }
    if (score > 100) {
        score = 100;
    }

    return score;
}

bool isGameOver(const Player& player) { // this checks if the game is over, which happens if the player finds the laptop, runs out of time, energy, or gets too stressed, so it covers all the win and lose conditions for the game
    if (player.hasLaptop) {
        return true;
    }
    if (player.timeLeft <= 0) {
        return true;
    }
    if (player.energy <= 0) {
        return true;
    }
    if (player.stress >= 10) {
        return true;
    }

    return false;
}
void printEnding(const Player& player) { // handles all possible endings based on performances and conditions
    printHeader("Ending");

    if (player.hasLaptop) {
        cout << "You recover the laptop and submit your work with a final score of " << calculateScore(player) << ".\n";
    } else if (player.timeLeft <= 0) {
        cout << "Campus closes before you recover the laptop. Final score: " << calculateScore(player) << ".\n";
    } else if (player.energy <= 0) {
        cout << "You run out of energy and have to call it for the night. Final score: " << calculateScore(player) << ".\n";
    } else if (player.stress >= 10) {
        cout << "Your stress gets too high, so you pause the search and ask for help. Final score: " << calculateScore(player) << ".\n";
    } else {
        cout << "You leave the search unfinished. Final score: " << calculateScore(player) << ".\n";
    }
}

void processCommand(map<string, Room>& rooms, Player& player, string command, bool& running) {
    if (command == "help") {
        printHelp();
    } 
    else if (command == "look") {
        describeRoom(rooms, player);
    } 
    else if (command == "take") {
        takeItem(rooms, player);
    } 
    else if (command == "inventory") { // command shows the players inventory, what they have and updates as they get more items 
        showInventory(player);
    } 
    else if (command == "status") { // similarly, this command shows the players current status with energy, stress, time left, clues, and score. It updates as the player takes actions and moves around the world
        showStatus(player);
    } 
    else if (command == "map") {
        printMap();
    } 
    else if (command == "rest") {
        restPlayer(player);
    } 
    else if (command == "quit") {
        running = false;
    } 
    else if (isDirection(command)) {
        movePlayer(rooms, player, command);
    } 
    else if (command.rfind("go ", 0) == 0) {
        movePlayer(rooms, player, command.substr(3));
    } 
    else {
        cout << "\nUnknown command. Type 'help' for options.\n"; // lets our player know if they typed something wrong and also reminds that they can type help to see all the commands they can use
    }
}