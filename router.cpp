#include "router.h"

MazeRouter::MazeRouter(string input_file, string output_file)
{
    this->input_file = input_file;
    this->output_file = output_file;

    fstream file;
    file.open(input_file, ios::in);
    initializeRouterInfo(file);
    initializeNets(file);
    file.close();
}

void MazeRouter::initializeRouterInfo(fstream &file)
{
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file.\n";
        return;
    }

    string line;

    // Read the first line to get the grid dimensions and penalties
    if (getline(file, line))
    {
        sscanf(line.c_str(), "%d, %d, %d, %d", &length, &width, &bend_pen, &via_pen);
    }

    // Initialize the grid with empty cells
    for (int i = 0; i < NUM_LAYERS; ++i)
    {
        grid[i] = vector<vector<Cell>>(length, vector<Cell>(width, {Type::Empty, 0}));
    }

    // Read obstruction lines (2nd and 3rd lines)
    while (getline(file, line) && line.substr(0, 3) == "OBS")
    {
        int layer, x, y;
        sscanf(line.c_str(), "OBS (%d, %d, %d)", &layer, &x, &y);
        if (layer >= 0 && layer < NUM_LAYERS && x >= 0 && x < length && y >= 0 && y < width)
        {
            grid[layer][x][y].type = Type::Obstruction;
        }
        else
        {
            cerr << "Error: Invalid obstruction coordinates.\n";
        }
    }
}

void MazeRouter::initializeNets(fstream &file)
{
}

void MazeRouter::initializeGrid()
{
}

void MazeRouter::fill(vector<Coordinate> net)
{
}

void MazeRouter::propagate()
{
}

void MazeRouter::writeRoute()
{
}

void MazeRouter::resetGridCosts()
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            for (int k = 0; k < grid[i][j].size(); k++)
            {
                grid[i][j][k].cost = 0;
            }
        }
    }
}

void MazeRouter::route()
{
}