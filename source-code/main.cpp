#include "router.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

    MazeRouter router(argv[1], argv[2]);

    cout << "\nRouter initialized\n";
    cout << "Maze dimentions: " << router.length << " x " << router.width << endl;
    cout << "Bend penalty: " << router.bend_pen << endl;
    cout << "Via penalty: " << router.via_pen << endl;

    router.route();

    return 0;
}