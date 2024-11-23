#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip> 
using namespace std;

enum Type {
	Obstacle, //0
	Source,   //1
	Target,   //2
	Empty,    //3
    Via       //4
};

struct  Cell {
	Type type;
	int cost;
};

struct  Coordinate {
	int row;
	int col;
    int layer;
    Type type;

    bool operator==(const Coordinate& rhs) const {
        return row == rhs.row && col == rhs.col && layer == rhs.layer;
    }
};

class MazeRouter{
    private: 
    unordered_map <int, vector<vector<Cell>>> grid;
    vector<vector<Coordinate>> nets;
    int length;
    int width;
    int bend_pen;
    int via_pen;
    string output_file;
    
    public:
    MazeRouter(){
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

    void printgrid() {
        cout << "M1:\n";
        cout << "========\n";
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                cout << setw(3) << grid[0][i][j].cost << " "; // setw for uniform spacing
            }
            cout << endl;
        }

        cout << "\nM2:\n";
        cout << "========\n";
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                cout << setw(3) << grid[1][i][j].cost << " ";
            }
            cout << endl;
        }
    }


    void youffy_function1(string file){ //sets grid
        //intializes nets vector
    };

    void youffy_function2(vector<Coordinate>net){ //per net
        //intializes grid
    };

    void pushback(vector<Coordinate>&border, Coordinate value) {
        if (find(border.begin(), border.end(), value) == border.end()) {
            border.push_back(value); // Add the value only if it's not found
        }
    };

    bool Update(int row, int col, int layer, vector<Coordinate>&border){
        Coordinate temp;
        
        if(row+1<length){ //1 up
            if(grid[layer][row+1][col].type==Type::Target)
                return 1;
            if(grid[layer][row+1][col].type!=Type::Obstacle && grid[layer][row+1][col].type!=Type::Source){ 
                temp.row = row+1;
                temp.col = col;
                temp.layer = layer;
                if(layer==0){ //Metal layer 1 (Horizontal)
                    if((grid[layer][row+1][col].cost > grid[layer][row][col].cost+bend_pen)||(grid[layer][row+1][col].cost==0))
                        grid[layer][row+1][col].cost = grid[layer][row][col].cost+bend_pen;
                }
                else{ //Metal layer 2 (Vertical)
                    if((grid[layer][row+1][col].cost > grid[layer][row][col].cost+1)||(grid[layer][row+1][col].cost==0))
                        grid[layer][row+1][col].cost = grid[layer][row][col].cost+1;
                }
                pushback(border, temp);
            }
        }
        
        if(row-1>=0){ //1 down
            if(grid[layer][row-1][col].type==Type::Target)
                return 1;
            if(grid[layer][row-1][col].type!=Type::Obstacle && grid[layer][row-1][col].type!=Type::Source){ 
                temp.row = row-1;
                temp.col = col;
                temp.layer = layer;
                if(layer==0){ //Metal layer 1 (Horizontal)
                    if((grid[layer][row-1][col].cost > grid[layer][row][col].cost+bend_pen)||(grid[layer][row-1][col].cost==0))
                        grid[layer][row-1][col].cost = grid[layer][row][col].cost+bend_pen;
                }
                else{ //Metal layer 2 (Vertical)
                    if((grid[layer][row-1][col].cost > grid[layer][row][col].cost+1)||(grid[layer][row-1][col].cost==0))
                        grid[layer][row-1][col].cost = grid[layer][row][col].cost+1;
                }
                pushback(border, temp);
            }
        }
        
        if(col+1<length){ //1 to the right
            if(grid[layer][row][col+1].type==Type::Target)
                return 1;
            if(grid[layer][row][col+1].type!=Type::Obstacle && grid[layer][row][col+1].type!=Type::Source){ 
                temp.row = row;
                temp.col = col+1;
                temp.layer = layer;
                if(layer==0){ //Metal layer 1 (Horizontal)
                    if((grid[layer][row][col+1].cost > grid[layer][row][col].cost+1)||(grid[layer][row][col+1].cost==0))
                        grid[layer][row][col+1].cost = grid[layer][row][col].cost+1;
                }
                else{ //Metal layer 2 (Vertical)
                    if((grid[layer][row][col+1].cost > grid[layer][row][col].cost+bend_pen)||(grid[layer][row][col+1].cost==0))
                        grid[layer][row][col+1].cost = grid[layer][row][col].cost+bend_pen;
                }
                pushback(border, temp);
            }
        }
        
        if(col-1>=0){ //1 to the left
            if(grid[layer][row][col-1].type==Type::Target)
                return 1;
            if(grid[layer][row][col-1].type!=Type::Obstacle && grid[layer][row][col-1].type!=Type::Source){ 
                temp.row = row;
                temp.col = col-1;
                temp.layer = layer;
                if(layer==0){ //Metal layer 1 (Horizontal)
                    if((grid[layer][row][col-1].cost > grid[layer][row][col].cost+1)||(grid[layer][row][col-1].cost==0))
                        grid[layer][row][col-1].cost = grid[layer][row][col].cost+1;
                }
                else{ //Metal layer 2 (Vertical)
                    if((grid[layer][row][col-1].cost > grid[layer][row][col].cost+bend_pen)||(grid[layer][row][col-1].cost==0))
                        grid[layer][row][col-1].cost = grid[layer][row][col].cost+bend_pen;
                }
                pushback(border, temp);
            }
        }
        
        if(grid[(layer+1)%2][row][col].type==Type::Target) //1 across layers
            return 1;
        if(grid[(layer+1)%2][row][col].type!=Type::Obstacle && grid[(layer+1)%2][row][col].type!=Type::Source){ 
            temp.row = row;
            temp.col = col;
            temp.layer = (layer+1)%2;
            if((grid[(layer+1)%2][row][col].cost > grid[layer][row][col].cost+via_pen)||(grid[(layer+1)%2][row][col].cost==0)){
                grid[(layer+1)%2][row][col].cost = grid[layer][row][col].cost+via_pen;
                grid[(layer+1)%2][row][col].type = Type::Via;
            }
            pushback(border, temp);
        }
        return 0;

    }

    void Fill(vector<Coordinate>sources){ //called per net per target. Does cost assignment till target reached.
        bool hit=false;
        vector<Coordinate> border;
        int size;
        for(int i=0;i<sources.size();i++){
            border.push_back(sources[i]);
        }    
        while(!hit){
            {
                size = border.size();
                for(int i=0;i<size;i++){
                    hit = Update(border[0].row,border[0].col,border[0].layer, border); //black borow
                    border.erase(border.begin());
                    if(hit){
                        break;
                    }
                    //now we have the new border
                }

                printgrid();
                cout<< "_________________________" << endl;
                
            }
            
        }
        
    };

    void Propagate(){ //called per net. Propagates to determine path and returns path sequence while marking it as an obstacle for nerowt nets.

    };

    void WriteRoute(){ //saves progress in file.

    };

    void EmptyGrid(){ //re-initializes all cell costs to 0

        // for(int i=0;i<grid.size();i++){
        //     for(int j=0;j<grid[i].size();j++){
        //         grid[i][j].cost = 0;
        //     }
        // }

    };

    void Route(){ //puts all functions above together. This will be the only function called in main.

        

    };

};

int main(){

    MazeRouter router;
    router.Fill({{2,1,0,Type::Source}});

    return 0;
}



