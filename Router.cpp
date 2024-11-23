#include <iostream>
#include <vector>
#include <unordered_map>
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
    Maze(string inputf, string outputf){
        youffy_function1(inputf);
        output_file = outputf; 
    };

    void youffy_function1(string file){ //sets grid
        //intializes nets vector
    };

    void youffy_function2(vector<Coordinate>net){ //per net
        //intializes grid
    };

    bool Update(int x, int y, int layer, vector<Coordinate>unvisited){
        Coordinate temp;
        if(grid[layer][x+1][y].type==Target||grid[layer][x-1][y].type==Target||grid[layer][x][y+1].type==Target||grid[layer][x][y-1].type==Target||||grid[(layer+1)%2][x][y].type==Target){ 
                return 1;       
        }
        else{
            if(grid[layer][x+1][y].type!=Obstacle){ //1 to the right
                temp.x = x;
                temp.y = y;
                temp.layer = layer;
                if(layer==0){
                    if((grid[layer][x+1][y].cost > grid[layer][x][y].cost+1)||(grid[layer][x+1][y].cost==0))
                        grid[layer][x+1][y].cost = grid[layer][x][y].cost+1;
                }
                else{
                    if((grid[layer][x+1][y].cost > grid[layer][x][y].cost+bend_pen)||(grid[layer][x+1][y].cost==0))
                        grid[layer][x+1][y].cost = grid[layer][x][y].cost+bend_pen;
                }
                unvisited.push_back(temp);
            }
             if(grid[layer][x-1][y].type!=Obstacle){ //1 to the left
                temp.x = x;
                temp.y = y;
                temp.layer = layer;
                if(layer==0){
                    if((grid[layer][x-1][y].cost > grid[layer][x][y].cost+1)||(grid[layer][x-1][y].cost==0))
                        grid[layer][x-1][y].cost = grid[layer][x][y].cost+1;
                }
                else{
                    if((grid[layer][x-1][y].cost > grid[layer][x][y].cost+bend_pen)||(grid[layer][x-1][y].cost==0))
                        grid[layer][x-1][y].cost = grid[layer][x][y].cost+bend_pen;
                }
                unvisited.push_back(temp);
            }
             if(grid[layer][x][y+1].type!=Obstacle){ //1 up
                temp.x = x;
                temp.y = y;
                temp.layer = layer;
                if(layer==0){
                    if((grid[layer][x][y+1].cost > grid[layer][x][y].cost+bend_pen)||(grid[layer][x][y+1].cost==0))
                        grid[layer][x][y+1].cost = grid[layer][x][y].cost+bend_pen;
                }
                else{
                    if((grid[layer][x][y+1].cost > grid[layer][x][y].cost+1)||(grid[layer][x][y+1].cost==0))
                        grid[layer][x][y+1].cost = grid[layer][x][y].cost+1;
                }
                unvisited.push_back(temp);
            }
             if(grid[layer][x][y-1].type!=Obstacle){ //1 down
                temp.x = x;
                temp.y = y;
                temp.layer = layer;
                if(layer==0){
                    if((grid[layer][x][y-1].cost > grid[layer][x][y].cost+bend_pen)||(grid[layer][x][y-1].cost==0))
                        grid[layer][x][y-1].cost = grid[layer][x][y].cost+bend_pen;
                }
                else{
                    if((grid[layer][x][y-1].cost > grid[layer][x][y].cost+1)||(grid[layer][x][y-1].cost==0))
                        grid[layer][x][y-1].cost = grid[layer][x][y].cost+1;
                }
                unvisited.push_back(temp);
            }
            if(grid[(layer+1)%2][x][y].type!=Obstacle){ //1 across layers
                temp.x = x;
                temp.y = y;
                temp.layer = (layer+1)%2;
                if((grid[(layer+1)%2][x][y].cost > grid[(layer+1)%2][x][y].cost+via_pen)||(grid[(layer+1)%2][x][y].cost==0))
                    grid[(layer+1)%2][x][y].cost = grid[(layer+1)%2][x][y].cost+via_pen;
                unvisited.push_back(temp);
            }
            return 0;
        }

    }

    void Fill(vector<Coordinate>sources){ //called per net per target. Does cost assignment till target reached.
        bool hit=false;
        vector<Coordinate> unvisited;
        int size;
        unvisited=sources;     
        while(!hit){
            {
                size = unvisited.size()
                for(int i=0;i<size;i++){
                    hit = Update(unvisited[0].x,unvisited[0].y,unvisited[0].layer, unvisited); //black box
                    unvisited.erase(unvisited.begin());
                    if(hit){
                        break;
                    }
                    //now we have the new unvisited
                }
            }
            
        }
        
    };

    void Propagate(){ //called per net. Propagates to determine path and returns path sequence while marking it as an obstacle for next nets.

    };

    void WriteRoute(){ //saves progress in file.

    };

    void EmptyGrid(){ //re-initializes all cell costs to 0

        for(int i=0;i<grid.size();i++){
            for(int j=0;j<grid[i].size();j++){
                grid[i][j].cost = 0;
            }
        }

    };

    void Route(){ //puts all functions above together. This will be the only function called in main.

        

    };

};

int main(){
    return 0;
}



