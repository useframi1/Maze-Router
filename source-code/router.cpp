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
        sscanf(line.c_str(), "%d, %d, %d, %d", &width, &length, &bend_pen, &via_pen);
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
    int layer, row, col;
    sscanf(line.c_str(), "OBS (%d, %d, %d)", &layer, &col, &row);
    if (layer >= 0 && layer < NUM_LAYERS && row >= 0 && row < length && col >= 0 && col < width)
    {
        grid[layer][row][col].type = Type::Obstruction;
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

        int layer, row, col;
        sscanf(coords.substr(start + 1, end - start - 1).c_str(), "%d, %d, %d", &layer, &col, &row);
        if (layer >= 0 && layer < NUM_LAYERS && row >= 0 && row < length && col >= 0 && col < width)
        {
            netCoordinates.push_back({row, col, layer});
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

void MazeRouter::orderNets()
{

    if (nets.size() == 0 || nets.size() == 1)
    {
        return;
    }

    vector<int> total_manhattan_distances;
    for (int i = 0; i < nets.size(); i++)
    {
        int total_manhattan_distance = 0;
        for (int j = 0; j < nets[i].size() - 1; j++)
        {
            total_manhattan_distance += (abs(nets[i][0].layer - nets[i][j + 1].layer) +
                                         abs(nets[i][0].row - nets[i][j + 1].row) +
                                         abs(nets[i][0].col - nets[i][j + 1].col));
        }
        cout << "Net " << i << " Manhattan Distance: " << total_manhattan_distance << endl;
        total_manhattan_distances.push_back(total_manhattan_distance);
    }

    // Sort the nets based on the total manhattan distance
    for (int i = 0; i < nets.size(); i++)
    {
        for (int j = i + 1; j < nets.size(); j++)
        {
            if (total_manhattan_distances[i] > total_manhattan_distances[j])
            {
                swap(nets[i], nets[j]);
                swap(total_manhattan_distances[i], total_manhattan_distances[j]);
            }
        }
    }

    return;
}

void MazeRouter::mapNetToGrid(vector<Coordinate> net)
{
    for (int i = 0; i < net.size(); i++)
    {
        grid[net[i].layer][net[i].row][net[i].col].type = i == 0 ? Type::Source : Type::Target;
    }
}

void MazeRouter::pushback(vector<Coordinate> &sources, Coordinate value)
{
    if (find(sources.begin(), sources.end(), value) == sources.end())
    {
        sources.push_back(value); // Add the value only if it's not found
    }
}

bool MazeRouter::updateCells(int row, int col, int layer, vector<Coordinate> &sources, Coordinate &target)
{
    Coordinate temp;

    if (row + 1 < length)
    { // 1 down
        if (grid[layer][row + 1][col].type == Type::Target)
        {
            target = {row + 1, col, layer};
            return 1;
        }
        if (grid[layer][row + 1][col].type != Type::Obstruction && grid[layer][row + 1][col].type != Type::Source)
        {
            temp.row = row + 1;
            temp.col = col;
            temp.layer = layer;
            if (layer == 0)
            { // Metal layer 1 (Horizontal)
                if ((grid[layer][row + 1][col].cost > grid[layer][row][col].cost + bend_pen) || (grid[layer][row + 1][col].cost == 0))
                {
                    grid[layer][row + 1][col].cost = grid[layer][row][col].cost + bend_pen;
                    grid[layer][row + 1][col].type = Type::Empty;
                    pushback(sources, temp);
                }
            }
            else
            { // Metal layer 2 (Vertical)
                if ((grid[layer][row + 1][col].cost > grid[layer][row][col].cost + 1) || (grid[layer][row + 1][col].cost == 0))
                {
                    grid[layer][row + 1][col].cost = grid[layer][row][col].cost + 1;
                    grid[layer][row + 1][col].type = Type::Empty;
                    pushback(sources, temp);
                }
            }
        }
    }

    if (row - 1 >= 0)
    { // 1 up
        if (grid[layer][row - 1][col].type == Type::Target)
        {
            target = {row - 1, col, layer};
            return 1;
        }
        if (grid[layer][row - 1][col].type != Type::Obstruction && grid[layer][row - 1][col].type != Type::Source)
        {
            temp.row = row - 1;
            temp.col = col;
            temp.layer = layer;
            if (layer == 0)
            { // Metal layer 1 (Horizontal)
                if ((grid[layer][row - 1][col].cost > grid[layer][row][col].cost + bend_pen) || (grid[layer][row - 1][col].cost == 0))
                {
                    grid[layer][row - 1][col].cost = grid[layer][row][col].cost + bend_pen;
                    grid[layer][row - 1][col].type = Type::Empty;
                    pushback(sources, temp);
                }
            }
            else
            { // Metal layer 2 (Vertical)
                if ((grid[layer][row - 1][col].cost > grid[layer][row][col].cost + 1) || (grid[layer][row - 1][col].cost == 0))
                {
                    grid[layer][row - 1][col].cost = grid[layer][row][col].cost + 1;
                    grid[layer][row - 1][col].type = Type::Empty;
                    pushback(sources, temp);
                }
            }
        }
    }

    if (col + 1 < width)
    { // 1 to the right
        if (grid[layer][row][col + 1].type == Type::Target)
        {
            target = {row, col + 1, layer};
            return 1;
        }
        if (grid[layer][row][col + 1].type != Type::Obstruction && grid[layer][row][col + 1].type != Type::Source)
        {
            temp.row = row;
            temp.col = col + 1;
            temp.layer = layer;
            if (layer == 0)
            { // Metal layer 1 (Horizontal)
                if ((grid[layer][row][col + 1].cost > grid[layer][row][col].cost + 1) || (grid[layer][row][col + 1].cost == 0))
                {
                    grid[layer][row][col + 1].cost = grid[layer][row][col].cost + 1;
                    grid[layer][row][col + 1].type = Type::Empty;
                    pushback(sources, temp);
                }
            }
            else
            { // Metal layer 2 (Vertical)
                if ((grid[layer][row][col + 1].cost > grid[layer][row][col].cost + bend_pen) || (grid[layer][row][col + 1].cost == 0))
                {
                    grid[layer][row][col + 1].cost = grid[layer][row][col].cost + bend_pen;
                    grid[layer][row][col + 1].type = Type::Empty;
                    pushback(sources, temp);
                }
            }
        }
    }

    if (col - 1 >= 0)
    { // 1 to the left
        if (grid[layer][row][col - 1].type == Type::Target)
        {
            target = {row, col - 1, layer};
            return 1;
        }
        if (grid[layer][row][col - 1].type != Type::Obstruction && grid[layer][row][col - 1].type != Type::Source)
        {
            temp.row = row;
            temp.col = col - 1;
            temp.layer = layer;
            if (layer == 0)
            { // Metal layer 1 (Horizontal)
                if ((grid[layer][row][col - 1].cost > grid[layer][row][col].cost + 1) || (grid[layer][row][col - 1].cost == 0))
                {
                    grid[layer][row][col - 1].cost = grid[layer][row][col].cost + 1;
                    grid[layer][row][col - 1].type = Type::Empty;
                    pushback(sources, temp);
                }
            }
            else
            { // Metal layer 2 (Vertical)
                if ((grid[layer][row][col - 1].cost > grid[layer][row][col].cost + bend_pen) || (grid[layer][row][col - 1].cost == 0))
                {
                    grid[layer][row][col - 1].cost = grid[layer][row][col].cost + bend_pen;
                    grid[layer][row][col - 1].type = Type::Empty;
                    pushback(sources, temp);
                }
            }
        }
    }

    if (grid[(layer + 1) % 2][row][col].type == Type::Target) // 1 across layers
    {
        target = {row, col, (layer + 1) % 2};
        return 1;
    }
    if (grid[(layer + 1) % 2][row][col].type != Type::Obstruction && grid[(layer + 1) % 2][row][col].type != Type::Source)
    {
        temp.row = row;
        temp.col = col;
        temp.layer = (layer + 1) % 2;
        if ((grid[(layer + 1) % 2][row][col].cost > grid[layer][row][col].cost + via_pen) || (grid[(layer + 1) % 2][row][col].cost == 0))
        {
            grid[(layer + 1) % 2][row][col].cost = grid[layer][row][col].cost + via_pen;
            grid[(layer + 1) % 2][row][col].type = Type::Via;
            pushback(sources, temp);
        }
    }
    return 0;
}

Coordinate MazeRouter::fill(vector<Coordinate> sources)
{
    bool hit = false;
    int size;
    int k = 0;
    Coordinate target = {-1, -1, -1};
    while (!hit && !sources.empty())
    {
        size = sources.size();
        for (int i = 0; i < size; i++)
        {
            if (!hit)
                hit = updateCells(sources[0].row, sources[0].col, sources[0].layer, sources, target);
            sources.erase(sources.begin());
        }
        // cout << "Wave" << ++k << ":\n";
        // printGridTypes();
        // printGridCosts();
    }

    size = sources.size();
    for (int i = 0; i < size; i++)
    {
        updateCells(sources[0].row, sources[0].col, sources[0].layer, sources, target);
    }

    return target;
}

vector<Coordinate> MazeRouter::back_propagate(Coordinate target)
{
    Coordinate next_cell = {target.row, target.col, target.layer};
    int min_cost;
    bool via_switch = false;
    char direction = '\0';
    int num_interations = 0;
    vector<Coordinate> route = {target};

    cout << "Target: (" << target.layer << ", " << target.col << ", " << target.row << ")\n";

    while (grid[next_cell.layer][next_cell.row][next_cell.col].type != Type::Source)
    {
        cout << (num_interations);
        if (num_interations++ > 1000)
        {
            cout << "Infinite loop detected during back propogation: No Route Found\n";
            break;
        }

        min_cost = INT_MAX;
        // cout << "Current cell: (" << next_cell.layer << ", " << next_cell.col << ", " << next_cell.row << ")\n";
        if (grid[next_cell.layer][next_cell.row][next_cell.col].type == Type::Via && !via_switch)
        {
            via_switch = true;
            grid[next_cell.layer][next_cell.row][next_cell.col].type = Type::Empty;
            next_cell.layer = (next_cell.layer + 1) % 2;
            route.insert(route.begin(), next_cell);
            continue;
        }
        else
        {
            via_switch = false;
        }

        if (next_cell.row - 1 >= 0)
        {
            if (grid[next_cell.layer][next_cell.row - 1][next_cell.col].type == Type::Source)
            {
                next_cell.row = next_cell.row - 1;
                route.insert(route.begin(), next_cell);
                return route;
            }

            if (grid[next_cell.layer][next_cell.row - 1][next_cell.col].type != Type::Obstruction && grid[next_cell.layer][next_cell.row - 1][next_cell.col].cost != 0)
            {
                if (grid[next_cell.layer][next_cell.row - 1][next_cell.col].cost < min_cost)
                {
                    min_cost = grid[next_cell.layer][next_cell.row - 1][next_cell.col].cost;
                    direction = 'u';
                }
            }
        }
        if (next_cell.row + 1 < length)
        {
            if (grid[next_cell.layer][next_cell.row + 1][next_cell.col].type == Type::Source)
            {
                next_cell.row = next_cell.row + 1;
                route.insert(route.begin(), next_cell);
                return route;
            }

            if (grid[next_cell.layer][next_cell.row + 1][next_cell.col].type != Type::Obstruction && grid[next_cell.layer][next_cell.row + 1][next_cell.col].cost != 0)
            {
                if (grid[next_cell.layer][next_cell.row + 1][next_cell.col].cost < min_cost)
                {
                    min_cost = grid[next_cell.layer][next_cell.row + 1][next_cell.col].cost;
                    direction = 'd';
                }
            }
        }
        if (next_cell.col - 1 >= 0)
        {
            if (grid[next_cell.layer][next_cell.row][next_cell.col - 1].type == Type::Source)
            {
                next_cell.col = next_cell.col - 1;
                route.insert(route.begin(), next_cell);
                return route;
            }

            if (grid[next_cell.layer][next_cell.row][next_cell.col - 1].type != Type::Obstruction && grid[next_cell.layer][next_cell.row][next_cell.col - 1].cost != 0)
            {
                if (grid[next_cell.layer][next_cell.row][next_cell.col - 1].cost < min_cost)
                {
                    min_cost = grid[next_cell.layer][next_cell.row][next_cell.col - 1].cost;
                    direction = 'l';
                }
            }
        }
        if (next_cell.col + 1 < length)
        {
            if (grid[next_cell.layer][next_cell.row][next_cell.col + 1].type == Type::Source)
            {
                next_cell.col = next_cell.col + 1;
                route.insert(route.begin(), next_cell);
                return route;
            }

            if (grid[next_cell.layer][next_cell.row][next_cell.col + 1].type != Type::Obstruction && grid[next_cell.layer][next_cell.row][next_cell.col + 1].cost != 0)
            {
                if (grid[next_cell.layer][next_cell.row][next_cell.col + 1].cost < min_cost)
                {
                    min_cost = grid[next_cell.layer][next_cell.row][next_cell.col + 1].cost;
                    direction = 'r';
                }
            }
        }

        switch (direction)
        {
        case 'u':
            next_cell.row--;
            break;
        case 'd':
            next_cell.row++;
            break;
        case 'l':
            next_cell.col--;
            break;
        case 'r':
            next_cell.col++;
            break;
        default:
            break;
        }

        route.insert(route.begin(), next_cell);
    }

    return route;
}

void MazeRouter::writeRoute(vector<Coordinate> route)
{
    for (int i = 0; i < route.size(); i++)
    {
        cout << "(" << route[i].layer << " " << route[i].col << " " << route[i].row << ") -> ";
    }
    cout << "END\n";
}

void MazeRouter::resetGridCosts()
{
    for (int i = 0; i < NUM_LAYERS; i++)
    {
        for (int j = 0; j < length; j++)
        {
            for (int k = 0; k < width; k++)
            {
                grid[i][j][k].cost = 0;
                grid[i][j][k].type = grid[i][j][k].type == Type::Via ? Type::Empty : grid[i][j][k].type;
            }
        }
    }
}

void MazeRouter::printGridCosts()
{
    for (int i = 0; i < NUM_LAYERS; i++)
    {
        cout << "M" << i + 1 << ":\n";
        cout << "========\n";
        for (int j = 0; j < length; j++)
        {
            for (int k = 0; k < width; k++)
            {
                cout << setw(3) << grid[i][j][k].cost << " ";
            }
            cout << endl;
        }
    }
}

void MazeRouter::printGridTypes()
{
    for (int i = 0; i < NUM_LAYERS; i++)
    {
        cout << "M" << i + 1 << ":\n";
        cout << "========\n";
        for (int j = 0; j < length; j++)
        {
            for (int k = 0; k < width; k++)
            {
                cout << setw(3) << grid[i][j][k].type << " ";
            }
            cout << endl;
        }
    }
}

void MazeRouter::printNets()
{
    for (int i = 0; i < nets.size(); i++)
    {
        cout << "Net " << i << ": ";
        for (int j = 0; j < nets[i].size(); j++)
        {
            cout << "(" << nets[i][j].layer << ", " << nets[i][j].col << ", " << nets[i][j].row << ") ";
        }
        cout << endl;
    }
}

void MazeRouter::route()
{
    orderNets();

    for (int i = 0; i < nets.size(); i++)
    {
        cout << "\n------------------------------------\n";
        cout << "Routing Net " << i << endl;
        mapNetToGrid(nets[i]);
        vector<Coordinate> sources = {nets[i][0]};
        vector<Coordinate> route;
        for (int j = 0; j < nets[i].size() - 1; j++)
        {
            resetGridCosts();
            Coordinate target = fill(sources);
            printGridCosts();

            if (target.row == -1 && target.col == -1 && target.layer == -1)
            {
                cout << "\n\nNo Route Found for Net " << i << endl;
                break;
            }

            vector<Coordinate> temp_route = back_propagate(target);
            for (int k = 0; k < temp_route.size(); k++)
            {
                route.push_back(temp_route[k]);
            }

            cout << "\nRoute for target " << j << ":\n";
            writeRoute(temp_route);
            cout << endl;

            sources.clear();
            for (int k = 0; k < route.size(); k++)
            {
                grid[route[k].layer][route[k].row][route[k].col].type = Type::Source;
                sources.push_back(route[k]);
            }
        }

        for (int j = 0; j < route.size(); j++)
        {
            grid[route[j].layer][route[j].row][route[j].col].type = Type::Obstruction;
        }

        // cout << "\nFinal Route for Net " << i << ":\n";
        // writeRoute(route);
        cout << "\n------------------------------------\n";
    }
}