#include "router.h"

int main()
{
    MazeRouter router("test.txt", "output.txt");

    cout << "Router initialized\n";
    cout << "Maze dimentions: " << router.length << " x " << router.width << endl;
    cout << "Bend penalty: " << router.bend_pen << endl;
    cout << "Via penalty: " << router.via_pen << endl;

    router.printGrid();
    router.printNets();

    return 0;
}