
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <stack>
#include <cstdlib> // for random functions
#include <ctime>
#include "menuUtils.h"
#include <climits>
#include <random>
#include <algorithm>
#include "Maze.h"
#include "Agent.h"
#include <mcpp/mcpp.h>

mcpp::MinecraftConnection mcConnection;

using namespace std;

enum Direction { UP, RIGHT, DOWN, LEFT };

struct Cell {
    int x, z;
    vector<Direction> unvisitedDirs;
    
    Cell(int x, int z) : x(x), z(z) {
        unvisitedDirs = {UP, RIGHT, DOWN, LEFT};
    }
};

// ------------------------------------------- declaration of functions -------------------------------------
void print_maze(const vector<string>& maze, const tuple<int, int, int>& base_point);
void read_maze();
void generate_random_maze();
void recursive_backtracking_maze(vector<string>& maze, int length, int width, bool testing_mode = false);
void main_menu();
void generate_maze_menu(); 
void generate_solve_maze_menu();
void checkCommandLineArgs (int argc, char* argv[]);

// ------------------------------------------- end declaration of functions ---------------------------------


// ------------------------------------------- declaration of global variables ------------------------------

int x, y, z; // x, y, z are for the "basePoint" & length and width - self-explanatory
unsigned int length, width; 
static vector <string> mazeVec; // globally declared vector
bool terrainFlatten = true;  // by default, terrain will be flattened when maze is built, for use in checkCommandLineArgs()
bool testingMode = false; // by default

// ------------------------------------------- end declaration of global variables --------------------------


// ------------------------------------------- declaration of global objects --------------------------------

Maze m; // an object of type Maze to access maze functions

// ------------------------------------------- end declaration of global objects ----------------------------


int main(int argc, char* argv[]) {
    srand(time(nullptr));
    printStartText();
    main_menu();
    printExitMessage();
    checkCommandLineArgs(argc, argv);
    return 0;
}

int myrandom (int i) { return rand() % i; }

void checkCommandLineArgs(int argc, char* argv[]) {
    
    for (int i = 1; i < argc; i++) { 
        if (string(argv[i]) == "--noFlattenTerrain") {
            terrainFlatten = false;

        }
    }

    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-testmode") {
            testingMode = true;

        }
    }

}

void main_menu() {
    mcConnection.doCommand("time set day");
    bool exitMainMenu = false;
    while (!exitMainMenu) {
        printMainMenu();

        int choice;
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 5) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "Input Error: Enter a number between 1 and 5 ...." << endl;
            continue;
        }
        
        if (choice == 1) {
            generate_maze_menu();
        } else if (choice == 4) {
            printTeamInfo();
            exitMainMenu = true; // returns user to main menu
        } else if (choice == 5) {
            exitMainMenu = true;
        } else if (choice == 2) {
            
            // validation to check whether or not mazeVec is empty
            if (!mazeVec.empty()) {
                mcpp::Coordinate basePoint(x, y, z); // created coordinate object that will then be passed to a maze object
                vector<string>* vecPtr = &mazeVec;
                Maze maze (basePoint, length, width, vecPtr, terrainFlatten);
            } else {

                cout << "Maze not defined! Generate maze before Building." << endl;
            }
        } else if (choice == 3) {
            generate_solve_maze_menu();
        }
}

}

void generate_maze_menu() {
    bool continueSubMenu = true;
    while (continueSubMenu) {
        printGenerateMazeMenu();
        
        int choice;
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 3) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "Input Error: Enter a number between 1 and 3 ...." << endl;
            continue;
        }
        
        if (choice == 1) {
            read_maze();
        } else if (choice == 2) {
            generate_random_maze();
            continueSubMenu = false; // returns user to main screen
        } else if (choice == 3) {
            continueSubMenu = false;
        } else {
            cout << "Option not implemented yet" << endl;
        }
    }
}

void generate_random_maze() {
    cout << "Enter the basePoint of maze:" << endl;
    cin >> x >> y >> z;

    int length, width;
    cout << "Enter the length and width of maze:" << endl;
    cin >> length >> width;

    if(length % 2 == 0 || width % 2 == 0) {
        cout << "Error: Length and width should be odd numbers." << endl;
        return;
    }

   // vector<string> maze(length, string(width, 'x'));

    // recursive_backtracking_maze(maze, length, width);
    mazeVec.assign(length, string(width, 'x'));
    recursive_backtracking_maze(mazeVec, length, width);

     
    // Print generated maze
    cout << "Maze generated successfully" << endl;
    cout << "**Printing Maze**" << endl;
   // print_maze(maze, make_tuple(x, y, z));
    print_maze(mazeVec, make_tuple(x, y, z));
    cout << "**End Printing Maze**" << endl;
   

}

void recursive_backtracking_maze(vector<string>& maze, int length, int width, bool testing_mode) {
    stack<Cell> stack;

    //initialize RNG to prevent random_shuffle error
    // initalise RNG to prevent random shuffle error
    std::default_random_engine rng(time(nullptr));

    // Random starting point
    int startX = 1;
    int startZ = 1;

    if(testing_mode) {
        maze[startX][startZ-1] = '.';  // set entrance for testing mode
    } else { 
        startX = rand() % (length - 2);
        startZ = rand() % (width - 2);
        if (startX % 2 == 0){
            startX += 1; 
        }else{
            startX += 2; 
        }
        if (startZ % 2 == 0){
            startZ += 1; 
        }else{
            startZ += 2; 
        }
        maze[startX][startZ] = '.';
    }

    Cell start(startX, startZ);
    stack.push(start);

    const int dx[] = {-1, 0, 1, 0};
    const int dz[] = {0, 1, 0, -1};
    
    while(!stack.empty()) {
        Cell current = stack.top();
        stack.pop();

        // For testing mode - U, D , L, R order 
        if (!testing_mode) {
            std::shuffle(current.unvisitedDirs.begin(), current.unvisitedDirs.end(), rng);
        }

        for(Direction dir : current.unvisitedDirs) {
            int newX = current.x + 2 * dx[dir];
            int newZ = current.z + 2 * dz[dir];
            if(newX > 0 && newX < length-1 && newZ > 0 && newZ < width-1 && maze[newX][newZ] == 'x') {
                maze[newX][newZ] = '.';
                maze[current.x + dx[dir]][current.z + dz[dir]] = '.'; 
                stack.push(Cell(newX, newZ));
            }
        }
    }
    int endX = 1;
    int endZ = 1;
    bool ExitPlaced = false;
    while(!ExitPlaced){
        int ExitSide = rand() % 4;
        if(ExitSide == 0){
            endX = 0;
            endZ = rand() % (width - 2) + 1;
            if (maze[endX+1][endZ] == '.'){
                maze[endX][endZ] = '.';
                ExitPlaced = true;
            }
        }else if(ExitSide == 1){
            endX = length-1;
            endZ = rand() % (width - 2) + 1;
            if (maze[endX-1][endZ] == '.'){
                maze[endX][endZ] = '.';
                ExitPlaced = true;
            }
        }else if(ExitSide == 2){
            endZ = 0;
            endX = rand() % (length - 2) + 1;
            if (maze[endX][endZ+1] == '.'){
                maze[endX][endZ] = '.';
                ExitPlaced = true;
            }
        }else{
            endZ = length-1;
            endX = rand() % (length - 2) + 1;
            if (maze[endX][endZ-1] == '.'){
                maze[endX][endZ] = '.';
                ExitPlaced = true;
            }
        }
    }
}

void read_maze() {

    cout << "Enter the basePoint of maze:" << endl;
    cin >> x >> y >> z;

    int length, width;
    cout << "Enter the length and width of maze:" << endl;
    cin >> length >> width;

    if(length % 2 == 0 || width % 2 == 0) {
        cout << "Error: Length and width should be odd numbers." << endl;
        return;
    }


    cout << "Enter the maze structure:" << endl;
    cin.ignore(); 
    for (int i = 0; i < length; i++) { 
        string row;
        getline(cin, row); 
        mazeVec.push_back(row);
    }

    // Validate if maze was read successfully and print
    cout << "Maze read successfully" << endl;
    cout << "**Printing Maze**" << endl;
    print_maze(mazeVec, make_tuple(x, y, z));
    cout << "**End Printing Maze**" << endl;
    main_menu();
}

void print_maze(const vector<string>& maze, const tuple<int, int, int>& base_point) {
    cout << "BasePoint: (" << get<0>(base_point) << ", " << get<1>(base_point) << ", " << get<2>(base_point) << ")" << endl;
    cout << "Structure:" << endl;
    for (const auto& row : maze) {
        cout << row << endl;
    }

}


void generate_solve_maze_menu() {

    printSolveMazeMenu();
    
    int choice;
    cin >> choice;


    if (choice == 2) {
       
        if (m.playerInitializationState() == false) {
            cout << "Initialize player using Solve manually." << endl;
            generate_solve_maze_menu();
        } else {
            m.Solve();
        }
        
    } else if (choice == 1) {
        cout << "Maze ready to Solve." << endl;

        //Randomly place player within the maze boundaries 
        m.placePlayerRandomly();

        m.WallFollowerAlgorithm();
        generate_solve_maze_menu();


    } else if (choice == 3) {
        generate_maze_menu();
    }
}

