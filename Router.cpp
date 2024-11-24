#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <climits>
using namespace std;

enum Type
{
    Obstacle, // 0
    Source,   // 1
    Target,   // 2
    Empty,    // 3
    Via       // 4
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
private:
    unordered_map<int, vector<vector<Cell>>> grid;
    vector<vector<Coordinate>> nets;
    int length;
    int width;
    int bend_pen;
    int via_pen;
    string output_file;

public:
    MazeRouter()
    {
        Cell empty = {Type::Empty, 0};
        Cell obstacle = {Type::Obstacle, 0};
        Cell source = {Type::Source, 0};
        Cell target = {Type::Target, 0};

        vector<vector<Cell>> grid1 =
            {
                {empty, empty, empty, empty, empty, empty, empty, empty},
                {empty, empty, empty, empty, empty, empty, empty, empty},
                {empty, source, empty, empty, empty, empty, empty, empty},
                {obstacle, obstacle, obstacle, obstacle, obstacle, obstacle, obstacle, obstacle},
                {obstacle, obstacle, obstacle, obstacle, obstacle, obstacle, obstacle, obstacle},
                {obstacle, obstacle, obstacle, obstacle, obstacle, obstacle, obstacle, obstacle},
                {empty, empty, empty, empty, empty, empty, target, empty},
                {empty, empty, empty, empty, empty, empty, empty, empty}

            };

        vector<vector<Cell>> grid2 =
            {
                {empty, empty, empty, empty, empty, empty, empty, empty},
                {empty, empty, empty, empty, empty, empty, empty, empty},
                {obstacle, obstacle, obstacle, obstacle, obstacle, empty, empty, empty},
                {empty, empty, empty, empty, empty, empty, empty, empty},
                {empty, empty, empty, empty, empty, empty, empty, empty},
                {empty, empty, empty, empty, empty, empty, empty, empty},
                {empty, empty, empty, empty, empty, empty, empty, empty},
                {empty, empty, empty, empty, empty, empty, empty, empty}

            };

        grid[0] = grid1;
        grid[1] = grid2;

        length = 8;
        width = 8;
        bend_pen = 3;
        via_pen = 5;
    };

    void printgrid()
    {
        cout << "M1:\n";
        cout << "========\n";
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                cout << setw(3) << grid[0][i][j].cost << " "; // setw for uniform spacing
            }
            cout << endl;
        }

        cout << "\nM2:\n";
        cout << "========\n";
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                cout << setw(3) << grid[1][i][j].cost << " ";
            }
            cout << endl;
        }
    }

    void pushback(vector<Coordinate> &border, Coordinate value)
    {
        if (find(border.begin(), border.end(), value) == border.end())
        {
            border.push_back(value); // Add the value only if it's not found
        }
    };

    bool Update(int row, int col, int layer, vector<Coordinate> &sources, Coordinate &target)
    {
        Coordinate temp;

        if (row + 1 < length)
        { // 1 down
            if (grid[layer][row + 1][col].type == Type::Target)
            {
                target = {row + 1, col, layer};
                return 1;
            }
            if (grid[layer][row + 1][col].type != Type::Obstacle && grid[layer][row + 1][col].type != Type::Source)
            {
                temp.row = row + 1;
                temp.col = col;
                temp.layer = layer;
                if (layer == 0)
                { // Metal layer 1 (Horizontal)
                    if ((grid[layer][row + 1][col].cost > grid[layer][row][col].cost + bend_pen) || (grid[layer][row + 1][col].cost == 0))
                        grid[layer][row + 1][col].cost = grid[layer][row][col].cost + bend_pen;
                }
                else
                { // Metal layer 2 (Vertical)
                    if ((grid[layer][row + 1][col].cost > grid[layer][row][col].cost + 1) || (grid[layer][row + 1][col].cost == 0))
                        grid[layer][row + 1][col].cost = grid[layer][row][col].cost + 1;
                }
                pushback(sources, temp);
            }
        }

        if (row - 1 >= 0)
        { // 1 up
            if (grid[layer][row - 1][col].type == Type::Target)
            {
                target = {row - 1, col, layer};
                return 1;
            }
            if (grid[layer][row - 1][col].type != Type::Obstacle && grid[layer][row - 1][col].type != Type::Source)
            {
                temp.row = row - 1;
                temp.col = col;
                temp.layer = layer;
                if (layer == 0)
                { // Metal layer 1 (Horizontal)
                    if ((grid[layer][row - 1][col].cost > grid[layer][row][col].cost + bend_pen) || (grid[layer][row - 1][col].cost == 0))
                        grid[layer][row - 1][col].cost = grid[layer][row][col].cost + bend_pen;
                }
                else
                { // Metal layer 2 (Vertical)
                    if ((grid[layer][row - 1][col].cost > grid[layer][row][col].cost + 1) || (grid[layer][row - 1][col].cost == 0))
                        grid[layer][row - 1][col].cost = grid[layer][row][col].cost + 1;
                }
                pushback(sources, temp);
            }
        }

        if (col + 1 < length)
        { // 1 to the right
            if (grid[layer][row][col + 1].type == Type::Target)
            {
                target = {row, col + 1, layer};
                return 1;
            }
            if (grid[layer][row][col + 1].type != Type::Obstacle && grid[layer][row][col + 1].type != Type::Source)
            {
                temp.row = row;
                temp.col = col + 1;
                temp.layer = layer;
                if (layer == 0)
                { // Metal layer 1 (Horizontal)
                    if ((grid[layer][row][col + 1].cost > grid[layer][row][col].cost + 1) || (grid[layer][row][col + 1].cost == 0))
                        grid[layer][row][col + 1].cost = grid[layer][row][col].cost + 1;
                }
                else
                { // Metal layer 2 (Vertical)
                    if ((grid[layer][row][col + 1].cost > grid[layer][row][col].cost + bend_pen) || (grid[layer][row][col + 1].cost == 0))
                        grid[layer][row][col + 1].cost = grid[layer][row][col].cost + bend_pen;
                }
                pushback(sources, temp);
            }
        }

        if (col - 1 >= 0)
        { // 1 to the left
            if (grid[layer][row][col - 1].type == Type::Target)
            {
                target = {row, col - 1, layer};
                return 1;
            }
            if (grid[layer][row][col - 1].type != Type::Obstacle && grid[layer][row][col - 1].type != Type::Source)
            {
                temp.row = row;
                temp.col = col - 1;
                temp.layer = layer;
                if (layer == 0)
                { // Metal layer 1 (Horizontal)
                    if ((grid[layer][row][col - 1].cost > grid[layer][row][col].cost + 1) || (grid[layer][row][col - 1].cost == 0))
                        grid[layer][row][col - 1].cost = grid[layer][row][col].cost + 1;
                }
                else
                { // Metal layer 2 (Vertical)
                    if ((grid[layer][row][col - 1].cost > grid[layer][row][col].cost + bend_pen) || (grid[layer][row][col - 1].cost == 0))
                        grid[layer][row][col - 1].cost = grid[layer][row][col].cost + bend_pen;
                }
                pushback(sources, temp);
            }
        }

        if (grid[(layer + 1) % 2][row][col].type == Type::Target) // 1 across layers
        {
            target = {row, col, (layer + 1) % 2};
            return 1;
        }
        if (grid[(layer + 1) % 2][row][col].type != Type::Obstacle && grid[(layer + 1) % 2][row][col].type != Type::Source)
        {
            temp.row = row;
            temp.col = col;
            temp.layer = (layer + 1) % 2;
            if ((grid[(layer + 1) % 2][row][col].cost > grid[layer][row][col].cost + via_pen) || (grid[(layer + 1) % 2][row][col].cost == 0))
            {
                grid[(layer + 1) % 2][row][col].cost = grid[layer][row][col].cost + via_pen;
                grid[(layer + 1) % 2][row][col].type = Type::Via;
            }
            pushback(sources, temp);
        }
        return 0;
    }

    Coordinate Fill(vector<Coordinate> sources)
    { // called per net per target. Does cost assignment till target reached.
        bool hit = false;
        int size;
        Coordinate target;
        while (!hit)
        {
            size = sources.size();
            for (int i = 0; i < size; i++)
            {
                hit = Update(sources[0].row, sources[0].col, sources[0].layer, sources, target); // black borow
                sources.erase(sources.begin());
                if (hit)
                {
                    break;
                }
                // now we have the new border
            }

            printgrid();
            cout << "_________________________" << endl;
        }

        return target;
    };

    vector<Coordinate> Propagate(Coordinate target)
    { // called per net per target. Propagates to determine path and returns path sequence while marking it as an obstacle for nerowt nets.
        Coordinate next_cell = {target.row, target.col, target.layer};
        int min_cost;
        char direction = '\0';
        vector<Coordinate> route = {target};

        while (grid[next_cell.layer][next_cell.row][next_cell.col].type != Type::Source)
        {
            min_cost = INT_MAX;
            if (grid[next_cell.layer][next_cell.row][next_cell.col].type == Type::Via)
            {
                next_cell.layer = (next_cell.layer + 1) % 2;
                route.insert(route.begin(), next_cell);
                continue;
            }

            if (next_cell.row - 1 >= 0)
            {
                if (grid[next_cell.layer][next_cell.row - 1][next_cell.col].type == Type::Source)
                {
                    next_cell.row = next_cell.row - 1;
                    cout << "Coordinate" << " " << next_cell.row << " " << next_cell.col << " " << next_cell.layer << " is a source" << endl;
                    route.insert(route.begin(), next_cell);
                    return route;
                }

                if (grid[next_cell.layer][next_cell.row - 1][next_cell.col].type != Type::Obstacle && grid[next_cell.layer][next_cell.row - 1][next_cell.col].cost != 0)
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
                    cout << "Coordinate" << " " << next_cell.row << " " << next_cell.col << " " << next_cell.layer << " is a source" << endl;
                    route.insert(route.begin(), next_cell);
                    return route;
                }

                if (grid[next_cell.layer][next_cell.row + 1][next_cell.col].type != Type::Obstacle && grid[next_cell.layer][next_cell.row + 1][next_cell.col].cost != 0)
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
                    cout << "Coordinate" << " " << next_cell.row << " " << next_cell.col << " " << next_cell.layer << " is a source" << endl;
                    route.insert(route.begin(), next_cell);
                    return route;
                }

                if (grid[next_cell.layer][next_cell.row][next_cell.col - 1].type != Type::Obstacle && grid[next_cell.layer][next_cell.row][next_cell.col - 1].cost != 0)
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
                    cout << "Coordinate" << " " << next_cell.row << " " << next_cell.col << " " << next_cell.layer << " is a source" << endl;
                    route.insert(route.begin(), next_cell);
                    return route;
                }

                if (grid[next_cell.layer][next_cell.row][next_cell.col + 1].type != Type::Obstacle && grid[next_cell.layer][next_cell.row][next_cell.col + 1].cost != 0)
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
    };

    void WriteRoute() { // saves progress in file.

    };

    void EmptyGrid() { // re-initializes all cell costs to 0

        // for(int i=0;i<grid.size();i++){
        //     for(int j=0;j<grid[i].size();j++){
        //         grid[i][j].cost = 0;
        //     }
        // }

    };

    void Route() { // puts all functions above together. This will be the only function called in main.

    };
};

int main()
{

    MazeRouter router;
    Coordinate target = router.Fill({{2, 1, 0}});
    cout << target.row << " " << target.col << " " << target.layer << endl;
    vector<Coordinate> route = router.Propagate(target);

    for (int i = 0; i < route.size(); i++)
    {
        cout << "(" << route[i].layer << " " << route[i].row << " " << route[i].col << ") -> ";
    }

    cout << endl;

    return 0;
}
