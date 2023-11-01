#ifndef ASSIGN_MAZE_H
#define ASSIGN_MAZE_H

#include <vector>
#include <mcpp/mcpp.h>
#include <string>
#include <chrono>
#include <thread>
// for placePlayerRandomly()
#include <cstdlib>
#include <ctime>

using namespace std;

class Maze
{

public:
    Maze(mcpp::Coordinate basePoint, unsigned int xlen, 
                                        unsigned int zlen, vector <string>* mazeVec, bool terrainFlatten);
    ~Maze();

    Maze(); // default constructor
     bool playerInitializationState(); // returns playerInitalisation state
     void placePlayerRandomly(); // places player randomly in maze
     void WallFollowerAlgorithm(); // wall follower algorithm
     void Solve();
     void populateAirPositions(); //We can try store the vector air positions before the maze is built
private:
    /* data */
    bool playerInitialized = false; // variable that will be passed back to mazeRunner.cpp for solve_maze_menu function
    vector<string> mazeVec; 
    mcpp::Coordinate basePoint; 
    vector<mcpp::Coordinate> airPositions;
};


    void flattenTerrain(int baseX, int baseY, int baseZ, unsigned int length, unsigned int width);

#endif //ASSIGN_MAZE_H
