#include "router.h"

MazeRouter::MazeRouter(string input_file, string output_file)
{
    this->input_file = input_file;
    this->output_file = output_file;

    initialize();
}

void MazeRouter::initialize()
{
    fstream file;
    file.open(input_file, ios::in);

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
    while (getline(file, line))
    {
        if (line.substr(0, 3) == "OBS")
        {
            addObstruction(line);
        }
        else if (line.substr(0, 3) == "net")
        {
            addNet(line);
        }
    }

    file.close();
}

void MazeRouter::addObstruction(string line)
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

void MazeRouter::addNet(string line)
{
    // Check if the line defines a net
    vector<Coordinate> netCoordinates;
    size_t pos = line.find(' ');
    if (pos == string::npos)
    {
        cerr << "Error: Invalid net format.\n";
        return;
    }

    // Extract coordinates from the line
    string coords = line.substr(pos + 1); // Get everything after the net name
    size_t start = 0, end = 0;
    while ((start = coords.find('(', end)) != string::npos)
    {
        end = coords.find(')', start);
        if (end == string::npos)
        {
            cerr << "Error: Invalid coordinate format in net.\n";
            break;
        }

        int layer, x, y;
        sscanf(coords.substr(start + 1, end - start - 1).c_str(), "%d, %d, %d", &layer, &x, &y);
        if (layer >= 0 && layer < NUM_LAYERS && x >= 0 && x < length && y >= 0 && y < width)
        {
            netCoordinates.push_back({x, y, layer});
        }
        else
        {
            cerr << "Error: Invalid net coordinates.\n";
            break;
        }
    }

    // Add the net to the nets vector
    nets.push_back(netCoordinates);
}

void MazeRouter::mapNetToGrid(vector<Coordinate> net)
{
    for (int i = 0; i < net.size(); i++)
    {
        grid[net[i].layer][net[i].x][net[i].y].type = i == 0 ? Type::Source : Type::Target;
    }
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

void MazeRouter::printGrid()
{
    for (int i = 0; i < grid.size(); i++)
    {
        cout << "Layer " << i << endl;
        for (int j = 0; j < grid[i].size(); j++)
        {
            for (int k = 0; k < grid[i][j].size(); k++)
            {
                cout << grid[i][j][k].type << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

void MazeRouter::printNets()
{
    for (int i = 0; i < nets.size(); i++)
    {
        cout << "Net " << i << ": ";
        for (int j = 0; j < nets[i].size(); j++)
        {
            cout << "(" << nets[i][j].layer << ", " << nets[i][j].x << ", " << nets[i][j].y << ") ";
        }
        cout << endl;
    }
}

void MazeRouter::route()
{
}