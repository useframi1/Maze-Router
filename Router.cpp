#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
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
	int x;
	int y;
    int layer;
    Type type;

    bool operator==(const Coordinate& rhs) const {
        return x == rhs.x && y == rhs.y && layer == rhs.layer;
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
            {empty, empty, target},
            {empty, obstacle, empty},
            {source, empty, empty}

        };

        vector<vector<Cell>> grid2 = 
        {
            {empty, empty, empty},
            {empty, empty, empty},
            {empty, empty, empty}

        };

        grid[0] = grid1;
        grid[1] = grid2;

        length = 3;
        width = 3;
        bend_pen = 2;
        via_pen = 3;
    };

    void printgrid(){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                cout<<grid[0][i][j].cost<<" ";
            }
            cout<<endl;
        }

        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                cout<<grid[1][i][j].cost<<" ";
            }
            cout<<endl;
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

    bool Update(int x, int y, int layer, vector<Coordinate>&border){
        Coordinate temp;
        
        if(x+1<length){ //1 to the right
            if(grid[layer][x+1][y].type==Type::Target)
                return 1;
            if(grid[layer][x+1][y].type!=Type::Obstacle && grid[layer][x+1][y].type!=Type::Source){ 
                temp.x = x;
                temp.y = y;
                temp.layer = layer;
                if(layer==0){ //Metal layer 1 (Horizontal)
                    if((grid[layer][x+1][y].cost > grid[layer][x][y].cost+1)||(grid[layer][x+1][y].cost==0))
                        grid[layer][x+1][y].cost = grid[layer][x][y].cost+1;
                }
                else{ //Metal layer 2 (Vertical)
                    if((grid[layer][x+1][y].cost > grid[layer][x][y].cost+bend_pen)||(grid[layer][x+1][y].cost==0))
                        grid[layer][x+1][y].cost = grid[layer][x][y].cost+bend_pen;
                }
                pushback(border, temp);
            }
        }
        
        if(x-1>=0){ //1 to the left
            if(grid[layer][x-1][y].type==Type::Target)
                return 1;
            if(grid[layer][x-1][y].type!=Type::Obstacle && grid[layer][x-1][y].type!=Type::Source){ 
                temp.x = x;
                temp.y = y;
                temp.layer = layer;
                if(layer==0){ //Metal layer 1 (Horizontal)
                    if((grid[layer][x-1][y].cost > grid[layer][x][y].cost+1)||(grid[layer][x-1][y].cost==0))
                        grid[layer][x-1][y].cost = grid[layer][x][y].cost+1;
                }
                else{ //Metal layer 2 (Vertical)
                    if((grid[layer][x-1][y].cost > grid[layer][x][y].cost+bend_pen)||(grid[layer][x-1][y].cost==0))
                        grid[layer][x-1][y].cost = grid[layer][x][y].cost+bend_pen;
                }
                pushback(border, temp);
            }
        }
        
        if(y+1<width){ //1 up
            if(grid[layer][x][y+1].type==Type::Target)
                return 1;
            if(grid[layer][x][y+1].type!=Type::Obstacle && grid[layer][x][y+1].type!=Type::Source){ 
                temp.x = x;
                temp.y = y;
                temp.layer = layer;
                if(layer==0){ //Metal layer 1 (Horizontal)
                    if((grid[layer][x][y+1].cost > grid[layer][x][y].cost+bend_pen)||(grid[layer][x][y+1].cost==0))
                        grid[layer][x][y+1].cost = grid[layer][x][y].cost+bend_pen;
                }
                else{ //Metal layer 2 (Vertical)
                    if((grid[layer][x][y+1].cost > grid[layer][x][y].cost+1)||(grid[layer][x][y+1].cost==0))
                        grid[layer][x][y+1].cost = grid[layer][x][y].cost+1;
                }
                pushback(border, temp);
            }
        }
        
        if(y-1>=0){ //1 down
            if(grid[layer][x][y-1].type==Type::Target)
                return 1;
            if(grid[layer][x][y-1].type!=Type::Obstacle && grid[layer][x][y-1].type!=Type::Source){ 
                temp.x = x;
                temp.y = y;
                temp.layer = layer;
                if(layer==0){ //Metal layer 1 (Horizontal)
                    if((grid[layer][x][y-1].cost > grid[layer][x][y].cost+bend_pen)||(grid[layer][x][y-1].cost==0))
                        grid[layer][x][y-1].cost = grid[layer][x][y].cost+bend_pen;
                }
                else{ //Metal layer 2 (Vertical)
                    if((grid[layer][x][y-1].cost > grid[layer][x][y].cost+1)||(grid[layer][x][y-1].cost==0))
                        grid[layer][x][y-1].cost = grid[layer][x][y].cost+1;
                }
                pushback(border, temp);
            }
        }
        
        if(grid[(layer+1)%2][x][y].type==Type::Target) //1 across layers
            return 1;
        if(grid[(layer+1)%2][x][y].type!=Type::Obstacle && grid[(layer+1)%2][x][y].type!=Type::Source){ 
            temp.x = x;
            temp.y = y;
            temp.layer = (layer+1)%2;
            if((grid[(layer+1)%2][x][y].cost > grid[(layer+1)%2][x][y].cost+via_pen)||(grid[(layer+1)%2][x][y].cost==0)){
                grid[(layer+1)%2][x][y].cost = grid[(layer+1)%2][x][y].cost+via_pen;
                grid[(layer+1)%2][x][y].type = Type::Via;
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
                cout << "Size: " << size << endl;
                for(int i=0;i<size;i++){
                    cout<< "First Source: " << border[0].x << " " << border[0].y << " " << border[0].layer << endl << endl;
                    
                    hit = Update(border[0].x,border[0].y,border[0].layer, border); //black box
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

    void Propagate(){ //called per net. Propagates to determine path and returns path sequence while marking it as an obstacle for next nets.

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
    router.Fill({{2,0,0,Type::Source}});

    return 0;
}



