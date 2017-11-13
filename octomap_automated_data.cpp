#include <octomap/octomap.h>
#include <octomap/OcTree.h>

using namespace std;

int main(int argc, char** argv)
{

    cout << endl;
    cout << "generating example map" << endl;

    octomap::OcTree tree (0.1);  // create empty tree with resolution 0.1
    int z = 0;
    for (int x=1; x<10; x= x+2) {
        for (int y=-3; y<4; y= y+2) {
            //for (int z=-2; z<2; z++) {
                octomap::point3d endpoint ((float) x*1.00f, (float) y*1.00f, (float) z*1.00f);
                tree.updateNode(endpoint, true); // integrate 'occupied' measurement
            }
        }
    //}

    std::cout << "Loaded  points "<<cloud.size()<<std::endl;

    tree.writeBinary("automated_tree.bt");
    cout << "wrote example file automated_tree.bt" << endl << endl;

    return 0;
}
