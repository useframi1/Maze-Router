#ifndef __ROUTER_H__
#define __ROUTER_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
using namespace std;

#define NUM_LAYERS 2

enum Type
{
    Obstruction, // 0
    Source,      // 1
    Target,      // 2
    Empty,       // 3
    Via          // 4
};

struct Cell
{
    Type type;
    int cost;
};

struct Coordinate
{
    int x;
    int y;
    int layer;
};

class MazeRouter
{
    // private:

public:
    unordered_map<int, vector<vector<Cell>>> grid;
    vector<vector<Coordinate>> nets;
    int length;
    int width;
    int bend_pen;
    int via_pen;
    string input_file;
    string output_file;
    /// @brief The constructor for the maze router
    /// @param input_file The path of the input file having all the router information
    /// @param output_file The path of the output file that will hold the routes for all the nets
    MazeRouter(string input_file, string output_file);

    /// @brief Initializes the maze router
    void initialize();

    /// @brief Add Obstruction to the grid
    void addObstruction(string line);

    /// @brief Add a net to the nets vector
    void addNet(string line);

    /// @brief Maps the net to the grid
    void mapNetToGrid(vector<Coordinate> net);

    /// @brief Called per net. Does cost assignment till target is reached
    /// @param net The coordinates of all the cells in the net
    void fill(vector<Coordinate> net);

    /// @brief Called per net. Propagates to determine path and returns path sequence while marking it as an obstacle for next nets.
    void propagate();

    /// @brief Saves progress in file.
    void writeRoute();

    /// @brief Re-Initializes all cell costs to 0
    void resetGridCosts();

    /// @brief Prints the grid
    void printGrid();

    /// @brief Prints the nets
    void printNets();

    /// @brief Puts all functions above together. This will be the only function called in main.
    void route();
};

#endif // __ROUTER_H__