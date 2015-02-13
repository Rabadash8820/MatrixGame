#include <iostream>
#include <string>
#include <cstdlib>	// For rand()
#include <ctime>	// To seed rand()
#include <conio.h>	// For getch()
using namespace std;

// ABSTRACT DATA TYPES
struct Coordinates {
	short r, c;
	bool operator==(Coordinates that) {
		return (r == that.r && c == that.c);
	}
};
enum Entity : char {
	AIR = ' ', WALL = '#', TELEPORT = '@', COIN = '$', PIT = 'O', FART = '&'
};
char PLAYER = 'V';
char ENEMY = '!';
enum Result { NONE, PLAYER_QUIT, COINS_COLLECTED, ENEMY_DEATH, PIT_DEATH, FART_DEATH };

// FUNCTION DECLARATIONS
void initialize();
Coordinates randomCoords();
char getSelection();
Result moveEnemies();
void dispMatrix();
Result movePlayer(char);
Result result();
string endMessage();

// GLOBAL VARIABLES/CONSTANTS
const short ROWS = 20, COLUMNS = 20;
const int NUM_TELEPORTS = 6, NUM_COINS = 20, NUM_PITS = 6, NUM_ENEMIES = 10, NUM_FARTS=5;
Entity matrix[ROWS][COLUMNS];
Coordinates teleportCoords[NUM_TELEPORTS];
Coordinates enemyCoords[NUM_ENEMIES];
Coordinates playerCoords;
int coinsCollected;
Result gameResult;

// MAIN
void main() {
	char sel;

	// Create the matrix and populate it
	initialize();

	// Loop until Player wants to quit
	bool keepPlaying = true;
	do {
		// Move the enemies
		gameResult = moveEnemies();
		keepPlaying = (gameResult == Result::NONE);

		// Quit or continue, according to whether an Enemy hit the Player
		if (keepPlaying) {
			// Display the matrix
			system("cls");
			dispMatrix();

			// Get the Player's move selection
			cout << "Coins collected: " << coinsCollected << "/" << NUM_COINS << endl;
			sel = getSelection();

			// Quit or move the Player, according to their selection
			if (sel == 'Q') {
				gameResult = Result::PLAYER_QUIT;
				keepPlaying = false;
			}
			else {
				gameResult = movePlayer(sel);
				keepPlaying = (gameResult == Result::NONE);
			}
		}

	} while (keepPlaying);

	// Display the outcome and exit
	system("cls");
	cout << endMessage() << endl
		 << endl
		 << "Press Q to quit." << endl;
	do sel = _getch();
	while (sel != 'q' && sel != 'Q');
}

// FUNCTION DEFINITIONS
void initialize() {
	srand(time(0));
	Coordinates coords;

	// Create a temporary matrix of Entities
	Entity temp[ROWS][COLUMNS] =
	{ { AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  WALL, WALL, AIR,  AIR,  AIR,  WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL },
	  { AIR,  WALL, WALL, WALL, AIR,  WALL, WALL, WALL, WALL, AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  WALL, WALL, WALL, WALL, AIR },
	  { WALL, WALL, AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  WALL, AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR },
	  { AIR,  WALL, AIR,  WALL, AIR,  AIR,  WALL, AIR,  AIR,  AIR,  WALL, AIR, WALL, AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR },
	  { AIR,  WALL, AIR,  WALL, WALL, WALL, WALL, AIR,  AIR,  AIR,  WALL, WALL, AIR,  AIR,  AIR,  AIR,  AIR,  WALL, AIR,  AIR },
	  { AIR,  WALL, AIR,  WALL, WALL, WALL, AIR,  AIR,  AIR,  WALL, WALL, WALL, AIR,  AIR,  AIR,  WALL, WALL, WALL, WALL, WALL },
	  { AIR,  WALL, AIR,  AIR,  WALL, WALL, AIR,  AIR,  AIR,  WALL, WALL, AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  WALL, AIR,  AIR },
	  { AIR,  WALL, AIR,  AIR,  WALL, AIR,  AIR,  AIR,  WALL, WALL, WALL, WALL, AIR,  WALL, AIR,  AIR,  AIR,  AIR,  AIR,  AIR },
	  { AIR,  WALL, AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  WALL, WALL, WALL, AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  WALL },
	  { AIR,  AIR,  AIR,  AIR,  AIR,  WALL, AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  WALL, AIR,  WALL, WALL, AIR,  AIR,  AIR,  WALL },
	  { WALL, AIR,  AIR,  AIR,  AIR,  WALL, WALL, AIR,  AIR,  AIR,  AIR,  WALL, WALL, WALL, WALL, WALL, WALL, AIR,  AIR,  WALL },
	  { AIR,  WALL, WALL, WALL, AIR,  AIR,  WALL, WALL, WALL, WALL, AIR,  AIR,  AIR,  WALL, AIR,  AIR,  AIR,  WALL, AIR,  AIR },
	  { AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  WALL, AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  WALL, AIR,  AIR },
	  { AIR,  AIR,  AIR,  WALL, AIR,  AIR,  AIR,  AIR,  WALL, AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  WALL, AIR,  AIR },
	  { AIR,  AIR,  WALL, WALL, AIR,  AIR,  WALL, AIR,  WALL, WALL, WALL, AIR,  AIR,  WALL, AIR,  AIR,  WALL, AIR,  AIR,  AIR },
	  { AIR,  AIR,  AIR,  WALL, AIR,  AIR,  WALL, AIR,  AIR,  WALL, AIR,  AIR,  AIR,  WALL, AIR,  AIR,  AIR,  AIR,  AIR,  AIR },
	  { AIR,  AIR,  AIR,  WALL, AIR,  AIR,  WALL, AIR,  AIR,  WALL, AIR,  AIR,  WALL, WALL, WALL, AIR,  AIR,  AIR,  AIR,  AIR },
	  { AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  WALL, WALL, AIR,  WALL, AIR,  AIR,  AIR,  AIR,  WALL },
	  { WALL, WALL, WALL, AIR,  AIR,  AIR,  AIR,  WALL, AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  AIR,  WALL, AIR,  AIR,  WALL },
	  { AIR,  AIR,  AIR,  AIR,  AIR,  WALL, WALL, WALL, WALL, AIR,  AIR,  WALL, AIR,  WALL, WALL, WALL, WALL, AIR,  WALL, WALL } };

	// Copy the temporary matrix to THE matrix
	for (short r=0; r<ROWS; ++r) {
		for (short c=0; c<COLUMNS; ++c)
			matrix[r][c] = temp[r][c];
	}

	// Randomly place Teleports
	for (int t=0; t<NUM_TELEPORTS; ++t) {
		coords = randomCoords();
		matrix[coords.r][coords.c] = Entity::TELEPORT;
		teleportCoords[t] = coords;
	}

	// Randomly place Coins
	for (int c=0; c<NUM_COINS; ++c) {
		coords = randomCoords();
		matrix[coords.r][coords.c] = Entity::COIN;
	}

	// Randomly place Pits
	for (int p=0; p<NUM_PITS; ++p) {
		coords = randomCoords();
		matrix[coords.r][coords.c] = Entity::PIT;
	}

	// Randomly place Farts
	for (int p=0; p<NUM_FARTS; ++p) {
		coords = randomCoords();
		matrix[coords.r][coords.c] = Entity::FART;
	}	
	
	// Randomly place the Player
	playerCoords = randomCoords();
	coinsCollected = 0;

	// Randomly place Enemeies
	for (int e=0; e<NUM_ENEMIES; ++e)
		enemyCoords[e] = randomCoords();
}
Coordinates randomCoords() {
	Coordinates coords;

	// Randomly choose coordinates until an empty one (AIR) is found
	do {
		coords.r = rand() % ROWS;
		coords.c = rand() % COLUMNS;
	} while (matrix[coords.r][coords.c] != Entity::AIR);

	return coords;
}
void dispMatrix() {
	// Draw top border
	cout << "\t+";
	for (short c=0; c<COLUMNS; ++c)
		cout << "--";
	cout << "-+";

	// Draw each row of the map
	// Render the matrix element first, then an Enemy if one is there, then the Player
	char ch;
	for (short r=0; r<ROWS; ++r) {
		cout << "\n\t| ";
		for (short c=0; c<COLUMNS; ++c) {
			ch = static_cast<char>(matrix[r][c]);
			for (int e=0; e<NUM_ENEMIES; ++e) {
				if (r == enemyCoords[e].r && c == enemyCoords[e].c) {
					ch = ENEMY;
					break;
				}
			}
			if (r == playerCoords.r && c == playerCoords.c)
				ch = PLAYER;
			cout << ch << ' ';
		}
		cout << "|";
	}

	// Draw the bottom border
	cout << "\n\t+";
	for (short c=0; c<COLUMNS; ++c)
		cout << "--";
	cout << "-+\n";
}
char getSelection() {
	int sel;
	Coordinates coords;
	
	// Get user selection as a string
	cout << "Type W, A, S, D to move, or type Q to quit.  (You are the " << PLAYER << ")" << endl
		 << ">";
	sel = _getch();

	// If the user wants to quit, create a sentinel coordinate of {-1, -1}
	while (!(sel == 'Q' || sel == 'q') && !(sel == 'W' || sel == 'w') && !(sel == 'A' || sel == 'a') && !(sel == 'S' || sel == 's') && !(sel == 'D' || sel == 'd')) {
		cout << "Please enter a 'W', 'A', 'S', 'D' or 'Q': ";
		sel = _getch();
	}
	
	return toupper(sel);
}
Result moveEnemies() {
	Coordinates coords;
	bool goodCoords = true;
	// Move each enemy in a random direction (up, down, left, or right)
	// as long as that space contains nothing but AIR
	for (int e=0; e<NUM_ENEMIES; ++e) {
		// Store the coordinates that are a random direction from this Enemy
		coords = enemyCoords[e];
		switch (rand() % 4) {
			case 0:
				if (coords.r > 0 && matrix[coords.r - 1][coords.c] == Entity::AIR)
					--coords.r;
				break;
			case 1:
				if (coords.r < ROWS - 1 && matrix[coords.r + 1][coords.c] == Entity::AIR)
					++coords.r;
				break;
			case 2:
				if (coords.c > 0 && matrix[coords.r][coords.c - 1] == Entity::AIR)
					--coords.c;
				break;
			case 3:
				if (coords.c < COLUMNS - 1 && matrix[coords.r][coords.c + 1] == Entity::AIR)
					++coords.c;
				break;
		}

		// Check if these coordinates collide with another Enemy or a stationary Entity
		Entity ent = matrix[coords.r][coords.c];
		if (ent == Entity::AIR || ent == Entity::COIN) {
			for (int e2=0; e2<NUM_ENEMIES; ++e2) {
				if (e2 != e && coords == enemyCoords[e2]) {
					goodCoords = false;
					break;
				}
			}

			// If the coordinates are good then move this Enemy
			if (goodCoords) {
				enemyCoords[e].r = coords.r;
				enemyCoords[e].c = coords.c;
			}
		}

		// If this Enemy now collides with the Player, end the game
		if (enemyCoords[e] == playerCoords)
			return ENEMY_DEATH;
	}

	return NONE;
}
Result movePlayer(char dir) {
	// Move the Player
	switch (dir) {
		case 'W':
			if (playerCoords.r > 0)
				--playerCoords.r;
			break;
		case 'A':
			if (playerCoords.c > 0)
				--playerCoords.c;
			break;
		case 'S':
			if (playerCoords.r < ROWS - 1)
				++playerCoords.r;
			break;
		case 'D':
			if (playerCoords.c < COLUMNS - 1)
				++playerCoords.c;
			break;
	}

	// If the Player hit an Enemy...
	for (int e=0; e<NUM_ENEMIES; ++e) {
		if (playerCoords == enemyCoords[e])
			return Result::ENEMY_DEATH;
	}

	// If the Player hit a stationary Entity...
	switch (matrix[playerCoords.r][playerCoords.c]) {
		case TELEPORT:
			playerCoords = teleportCoords[rand() % NUM_TELEPORTS];
			return Result::NONE;
			break;
		case COIN:
			matrix[playerCoords.r][playerCoords.c] = Entity::AIR;
			if (++coinsCollected == NUM_COINS)
				return Result::COINS_COLLECTED;
			else
				return Result::NONE;
			break;
		case PIT:
			return Result::NONE;
			break;
		case FART:
			return Result::FART_DEATH;
			break;
		default:
			return Result::NONE;
	}
}
string endMessage() {
	string message;
	switch (gameResult) {
		case PLAYER_QUIT:
			message = "Ok bye, jerk.";
			break;
		case COINS_COLLECTED:
			message = "Wow.  You collected all the coins.  Yay...";
			break;
		case ENEMY_DEATH:
			message = "You were killed by an exclamation mark.  How does that make you feel?";
			break;
		case PIT_DEATH:
			message = "You fell into a pit and died.  Way to go.";
		case FART_DEATH:
			message = "Ew, dat nasty.";
	}
	return message;
}