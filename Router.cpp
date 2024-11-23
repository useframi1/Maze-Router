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
        youffy_function(inputf);
        output_file = outputf; 
    };

    void youffy_function(string file){
        //intializes data structures through file
    };

    void Fill(vector<Pin> net){ //called per net. Does cost assignment till target reached.

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



