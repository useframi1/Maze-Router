#ifndef __ROUTER_H__
#define __ROUTER_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <climits>
#include <iomanip>
#include <algorithm>

using namespace std;

#define NUM_LAYERS 2

enum Type
{
    Obstruction, // 0
    Source,      // 1
    Target,      // 2
    Empty,       // 3
    Via,         // 4
};

struct Cell
{
    Type type;
    int cost;
};

struct Coordinate
{
    int row;
    int col;
    int layer;
    bool operator==(const Coordinate &rhs) const
    {
        return row == rhs.row && col == rhs.col && layer == rhs.layer;
    }
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

    int via_count = 0;
    int max_length = 0;
    int total_wire_l = 0;

    const string ui_route_path = "../labyrinth/src/app/input.txt";
    const string ui_dimensions_path = "../labyrinth/src/app/grid_dimensions.txt";

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

    /// @brief Reserves source and targets for the nets
    void reserveCells();

    /// @brief Orders the nets based on the total manhattan distance of the net
    void orderNets();

    /// @brief Maps the net to the grid
    void mapNetToGrid(vector<Coordinate> net);

    /// @brief Add the value in a vector only if it's not found
    void pushback(vector<Coordinate> &sources, Coordinate value);

    /// @brief Updates the costs of the cells in the grid
    bool updateCells(int row, int col, int layer, vector<Coordinate> &sources, Coordinate &target);

    /// @brief Called per net. Does cost assignment till target is reached
    Coordinate fill(vector<Coordinate> sources);

    /// @brief Called per net. Propagates to determine path and returns path sequence while marking it as an obstacle for next nets.
    vector<Coordinate> back_propagate(Coordinate target);

    /// @brief Saves progress in file.
    void writeRoute(int net, vector<Coordinate> route);

    /// @brief Truncates the file
    void truncateUIRouteFile();

    /// @brief Writes the route to the UI directory
    void writeRouteToUI(int net, vector<Coordinate> route);

    /// @brief Writes the dimensions to the UI directory
    void writeDimensionsToUI();

    /// @brief Re-Initializes all cell costs to 0
    void resetGridCosts();

    /// @brief Prints the grid costs
    void printGridCosts();

    /// @brief Prints the grid types
    void printGridTypes();

    /// @brief Prints the nets
    void printNets();

    /// @brief Puts all functions above together. This will be the only function called in main.
    void route();
};

#endif // __ROUTER_H__