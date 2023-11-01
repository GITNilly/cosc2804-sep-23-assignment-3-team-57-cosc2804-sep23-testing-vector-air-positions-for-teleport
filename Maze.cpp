#include "Maze.h"
#include <random>


mcpp::MinecraftConnection mc;

string toString(mcpp::Coordinate coordinate);

string toString(mcpp::Coordinate coordinate) {
    stringstream ss;
    ss << coordinate;
    return ss.str();
}

//------------------------ declaration of global vectors ------------------------

std::vector<mcpp::Coordinate> wallPositions;
std::vector<mcpp::Coordinate> airPositions;

//------------------------ end declaration of global vectors ------------------------

Maze::Maze(mcpp::Coordinate basePoint, unsigned int xlen, unsigned int zlen, vector <string>* mazeVec, bool terrainFlatten) {
    

    basePoint.y += 10; // increase 10 so the player can be teleported 10 units up
    mc.setPlayerPosition(basePoint); // teleport player
    basePoint.y -= 10; // reduce back down 10 so the blocks can be built

   vector<string> &localVec = *mazeVec; // dereference the vector

    int baseX, baseY, baseZ;  // Set these to the starting position in Minecraft
    baseX = basePoint.x;
    baseY = basePoint.y;
    baseZ = basePoint.z;

    if (terrainFlatten == true) {
        flattenTerrain(baseX, baseY, baseZ, xlen, zlen);
    } 

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    for (int row = 0; row < localVec.size(); ++row) {
        for (int col = 0; col < localVec[row].size(); ++col) {
            int minecraftX = baseX + col;
            int minecraftY = baseY;  // Assuming the maze is on the same level
            int minecraftZ = baseZ + row;

            
            if (localVec[row][col] == 'x') {
                // 'x' represents a wall block
                for (int i = 0; i < 3; ++i) {
                   // setBlock(minecraftX, minecraftY + i, minecraftZ, "wall_block_type");
                    mc.setBlock(mcpp::Coordinate(minecraftX, minecraftY + i, minecraftZ), mcpp::Blocks::ACACIA_WOOD_PLANK);
                }
            } else if (localVec[row][col] == '.') {
                // '.' represents an open space
                mc.setBlock(mcpp::Coordinate(minecraftX, minecraftY, minecraftZ), mcpp::Blocks::AIR);
            }
        }
    }

    
    

}

Maze::~Maze()
{
}

Maze::Maze() {
    // default constructor
}

void flattenTerrain(int baseX, int baseY, int baseZ, unsigned int length, unsigned int width) {

    for (int x = baseX; x < baseX + length; ++x) {
        for (int z = baseZ; z < baseZ + width; ++z) {
            for (int y = baseY + 1; y < baseY + 4; ++y) {
                mcpp::Coordinate loc{x, y, z};
                mc.setBlock(loc, mcpp::Blocks::AIR);
            }
        }
    }
}
     
bool Maze::playerInitializationState() {
    return playerInitialized;
}

void Maze::populateAirPositions() {
    for(unsigned int i = 0; i < mazeVec.size(); i++) {
        for(unsigned int j = 0; j < mazeVec[i].length(); j++) {
            if(mazeVec[i][j] == '.') {
                mcpp::Coordinate airPosition;
                airPosition.x = basePoint.x + i; // assuming x corresponds to the row
                airPosition.y = basePoint.y;     // y-coordinate remains the same
                airPosition.z = basePoint.z + j; // assuming z corresponds to the column
                airPositions.push_back(airPosition);
            }
        }
    }
}


}

void Maze::placePlayerRandomly() {
    
    srand(static_cast<unsigned int>(time(nullptr)));

    int randomIndex = rand() % airPositions.size();

    mc.setPlayerPosition(airPositions[randomIndex]);

    playerInitialized = true;
}



// void Maze::WallFollowerAlgorithm() { // must solve the maze, place each lime carpet block one after another while removing the previous one and also output the coordinate of each lime carpet block to terminal

//     //bool exitLoop = false; // bool for while loop

//      mcpp::Coordinate currentPosition = mc.getPlayerPosition();
//     //  currentPosition.x += 1;
//     //  currentPosition.z += 1;
    
//     //int count = 0;
//     // Keep turning until an open direction is found
//     while (true) {
//         // Check the block to the right

//         mcpp::Coordinate rightCoord(currentPosition.x + 1, currentPosition.y, currentPosition.z);
//         mcpp::BlockType rightBlock = mc.getBlock(rightCoord);

//         // Check the block in front
//         mcpp::Coordinate frontCoord(currentPosition.x, currentPosition.y, currentPosition.z - 1);
//         mcpp::BlockType frontBlock = mc.getBlock(frontCoord);

//         // Check the block to the left
//         mcpp::Coordinate leftCoord(currentPosition.x - 1, currentPosition.y, currentPosition.z);
//         mcpp::BlockType leftBlock = mc.getBlock(leftCoord);

//         // Check the block behind
//         mcpp::Coordinate backCoord(currentPosition.x, currentPosition.y, currentPosition.z + 1);
//         mcpp::BlockType backBlock = mc.getBlock(backCoord);

//         // Store the positions of walls and air blocks
//         if (rightBlock == mcpp::Blocks::ACACIA_WOOD_PLANK) {
//             wallPositions.push_back(rightCoord);
//             //mc.postToChat("right block wall at: " + toString(currentPosition));
//         } else if (rightBlock == mcpp::Blocks::AIR) {
//             airPositions.push_back(rightCoord); 
//             //count++;
//             mc.postToChat("right block air at: " + toString(currentPosition));
//              mc.setBlock(rightCoord, mcpp::Blocks::LIME_CARPET);
//             //mc.setBlock(mcpp::Coordinate(rightCoord.x + 1, rightCoord.y, rightCoord.z), mcpp::Blocks::LIME_CARPET);

//         }

//         if (frontBlock == mcpp::Blocks::ACACIA_WOOD_PLANK) {
//             wallPositions.push_back(frontCoord);
//             //mc.postToChat("front block wall at: " + toString(currentPosition));
//         } else if (frontBlock == mcpp::Blocks::AIR) {
//             airPositions.push_back(frontCoord);
//            // count++;
//             mc.postToChat("front block air at: " + toString(currentPosition));
//              mc.setBlock(frontCoord, mcpp::Blocks::LIME_CARPET);
//             //mc.setBlock(mcpp::Coordinate(rightCoord.x + 1, rightCoord.y, rightCoord.z), mcpp::Blocks::LIME_CARPET);

//         }

//         if (leftBlock == mcpp::Blocks::ACACIA_WOOD_PLANK) {
//             wallPositions.push_back(leftCoord);
//             //mc.postToChat("left block wall at: " + toString(currentPosition));
//         } else if (leftBlock == mcpp::Blocks::AIR) {
//             airPositions.push_back(leftCoord);
//             //count++;
//              mc.postToChat("left block air at: " + toString(currentPosition));
//              mc.setBlock(leftCoord, mcpp::Blocks::LIME_CARPET);
//             //mc.setBlock(mcpp::Coordinate(rightCoord.x + 1, rightCoord.y, rightCoord.z), mcpp::Blocks::LIME_CARPET);

//         }

//         if (backBlock == mcpp::Blocks::ACACIA_WOOD_PLANK) {
//             wallPositions.push_back(backCoord);
//             //mc.postToChat("back block wall at: " + toString(currentPosition));
//         } else if (backBlock == mcpp::Blocks::AIR) {
//             airPositions.push_back(backCoord);
//            // count++;
//              mc.postToChat("back block air at: " + toString(currentPosition));
//             mc.setBlock(backCoord, mcpp::Blocks::LIME_CARPET);
//            // mc.setBlock(mcpp::Coordinate(rightCoord.x + 1, rightCoord.y, rightCoord.z), mcpp::Blocks::LIME_CARPET);

//         }

//         // If there is an open direction, break out of the loop
//         if (rightBlock == mcpp::Blocks::AIR) {
//             // Turn right
//             mc.postToChat("Turned right!");
//             currentPosition = rightCoord;
//         } else if (frontBlock == mcpp::Blocks::AIR) {
//             // Go straight
//             mc.postToChat("Go straight!");
//             currentPosition = frontCoord;
//         } else if (leftBlock == mcpp::Blocks::AIR) {
//             // Turn left
//             mc.postToChat("Turned left!");
//             currentPosition = leftCoord;
//         } else if (backBlock == mcpp::Blocks::AIR) {
//             // Turn back (180 degrees)
//            mc.postToChat("Turned back!");
//             currentPosition = backCoord;
//         }

//         // If no open direction found, keep turning
//         // Add a delay between turns (adjust as needed)
//         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            

//            stringstream out;
//            out << airPositions[airPositions.size()] << std::endl;
//           string block = out.str();
//           mc.postToChat(block);

            
//         }

//     }

    void Maze::Solve() {

        for (unsigned int i = 0; i < airPositions.size(); i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
            mc.setBlock(airPositions[i], mcpp::Blocks::LIME_CARPET);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            mc.setBlock(airPositions[i], mcpp::Blocks::AIR);
        }   
    }

void Maze::WallFollowerAlgorithm() { // must solve the maze, place each lime carpet block one after another while removing the previous one and also output the coordinate of each lime carpet block to terminal

    mcpp::Coordinate currentPosition = mc.getPlayerPosition();

    // Keep turning until an open direction is found
    while (true) {
        // Check the block to the right

        mcpp::Coordinate rightCoord(currentPosition.x + 1, currentPosition.y, currentPosition.z);
        mcpp::BlockType rightBlock = mc.getBlock(rightCoord);

        // Check the block in front
        mcpp::Coordinate frontCoord(currentPosition.x, currentPosition.y, currentPosition.z - 1);
        mcpp::BlockType frontBlock = mc.getBlock(frontCoord);

        // Check the block to the left
        mcpp::Coordinate leftCoord(currentPosition.x - 1, currentPosition.y, currentPosition.z);
        mcpp::BlockType leftBlock = mc.getBlock(leftCoord);

        // Check the block behind
        mcpp::Coordinate backCoord(currentPosition.x, currentPosition.y, currentPosition.z + 1);
        mcpp::BlockType backBlock = mc.getBlock(backCoord);

        // Store the positions of walls and air blocks
        if (rightBlock == mcpp::Blocks::ACACIA_WOOD_PLANK) {
            wallPositions.push_back(rightCoord);
            //mc.postToChat("right block wall at: " + toString(currentPosition));
        } else if (rightBlock == mcpp::Blocks::AIR) {
            airPositions.push_back(rightCoord);
            mc.postToChat("right block air at: " + toString(currentPosition));
            mc.setBlock(rightCoord, mcpp::Blocks::LIME_CARPET);
            //mc.setBlock(mcpp::Coordinate(rightCoord.x + 1, rightCoord.y, rightCoord.z), mcpp::Blocks::LIME_CARPET);

        }

        if (frontBlock == mcpp::Blocks::ACACIA_WOOD_PLANK) {
            wallPositions.push_back(frontCoord);
            //mc.postToChat("front block wall at: " + toString(currentPosition));
        } else if (frontBlock == mcpp::Blocks::AIR) {
            airPositions.push_back(frontCoord);
            mc.postToChat("front block air at: " + toString(currentPosition));
            mc.setBlock(frontCoord, mcpp::Blocks::LIME_CARPET);
            //mc.setBlock(mcpp::Coordinate(rightCoord.x + 1, rightCoord.y, rightCoord.z), mcpp::Blocks::LIME_CARPET);

        }

        if (leftBlock == mcpp::Blocks::ACACIA_WOOD_PLANK) {
            wallPositions.push_back(leftCoord);
            //mc.postToChat("left block wall at: " + toString(currentPosition));
        } else if (leftBlock == mcpp::Blocks::AIR) {
            airPositions.push_back(leftCoord);
            mc.postToChat("left block air at: " + toString(currentPosition));
            mc.setBlock(leftCoord, mcpp::Blocks::LIME_CARPET);
            //mc.setBlock(mcpp::Coordinate(rightCoord.x + 1, rightCoord.y, rightCoord.z), mcpp::Blocks::LIME_CARPET);

        }

        if (backBlock == mcpp::Blocks::ACACIA_WOOD_PLANK) {
            wallPositions.push_back(backCoord);
           // mc.postToChat("back block wall at: " + toString(currentPosition));
        } else if (backBlock == mcpp::Blocks::AIR) {
            airPositions.push_back(backCoord);
            mc.postToChat("back block air at: " + toString(currentPosition));
            mc.setBlock(backCoord, mcpp::Blocks::LIME_CARPET);
           // mc.setBlock(mcpp::Coordinate(rightCoord.x + 1, rightCoord.y, rightCoord.z), mcpp::Blocks::LIME_CARPET);

        }

        // If there is an open direction, break out of the loop
        if (rightBlock == mcpp::Blocks::AIR) {
            // Turn right
            mc.postToChat("Turned right!");
            currentPosition = rightCoord;
        } else if (frontBlock == mcpp::Blocks::AIR) {
            // Go straight
            mc.postToChat("Go straight!");
            currentPosition = frontCoord;
        } else if (leftBlock == mcpp::Blocks::AIR) {
            // Turn left
            mc.postToChat("Turned left!");
            currentPosition = leftCoord;
        } else if (backBlock == mcpp::Blocks::AIR) {
            // Turn back (180 degrees)
            mc.postToChat("Turned back!");
            currentPosition = backCoord;
        }

        // If no open direction found, keep turning
        // Add a delay between turns (adjust as needed)
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        
    }

}

   


