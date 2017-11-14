#include <octomap/octomap.h>
#include <octomap/OcTree.h>
#include <octomap/ColorOcTree.h>
#include <math.h>
#include <algorithm>    //std::max
using namespace std;

int main(int argc, char** argv)
{

    float resolution = 0.1;
    int h = 3;
    octomap::ColorOcTree tree (resolution);  // create empty tree with resolution 0.1
    int z = 0;
    octomap::KeySet occupied_cells;
    // define a std pair to get results of insert function later
    std::pair<octomap::KeySet::iterator,bool> ret = std::make_pair(occupied_cells.begin(), false);

    for (int x=1; x<10; x= x+5)
    {
        for (int y=-3; y<4; y= y+4)
        {
            octomap::point3d endpoint ((float) x*1.00f, (float) y*1.00f, (float) z*1.00f);
            octomap::OcTreeKey key;
            if (tree.coordToKeyChecked(endpoint, key))
            {
                ret.second = false;
                // get the results of insert function - returns false if key is already present
                ret = occupied_cells.insert(key);
                if (ret.second)
                {
                    octomap::ColorOcTreeNode* nodePtr = tree.updateNode(endpoint, true); // integrate 'occupied' measurement
                    //nodePtr->setColor(255,0,0);
                    tree.setNodeColor(key, 255,0,0);
                }
            }
        }
    }

    octomap::KeySet empty_cells;
    // define a std pair to get results of insert function later
    std::pair<octomap::KeySet::iterator,bool> retemty = std::make_pair(empty_cells.begin(), false);
    float minimum = 0.0, maximum = 1.0, ratio;
    unsigned int r, g, b;
    for(octomap::ColorOcTree::leaf_iterator it = tree.begin_leafs(), end=tree.end_leafs(); it!= end; ++it)
    {
        //set the value of radar target equal to 3/4
        it->setValue(0.75);

        octomap::OcTreeKey key;
        key = it.getKey();
        octomap::point3d point = tree.keyToCoord(key);

        for (float x=-(resolution*h); x<=(resolution*h); x= x+resolution)
        {
            for (float y=-(resolution*h); y<=(resolution*h); y= y+resolution)
            {
                for (float z=-(resolution*h); z<=(resolution*h); z= z+resolution)
                {
                    float xIncrement = (point.x() + x);
                    float yIncrement = (point.y() + y);
                    float zIncrement = (point.z() + z);

                    octomap::point3d newPoint(xIncrement, yIncrement, zIncrement);

                    octomap::OcTreeKey key;
                    if (tree.coordToKeyChecked(newPoint, key))
                    {
                        retemty.second = false;
                        // get the results of insert function - returns false if key is already present
                        retemty = empty_cells.insert(key);
                        if (retemty.second)
                        {
                            octomap::ColorOcTreeNode* nodePtr = tree.updateNode(newPoint, false);

                            float deltaS = sqrt(x*x + y*y + z*z );
                            if (deltaS < (h*resolution))
                            {
                                float u = deltaS / (h*resolution);
                                float prob = 0.75*(1- u*u);
                                if (prob < 0)
                                    prob = 0;
                                nodePtr->setValue(prob);
                                ratio = 2 * (prob - minimum);
                                b = (int)std::max(minimum, 255*(1 - ratio));
                                r = (int)std::max(minimum, 255*(ratio -1));
                                g = 255 - b - r;
                                nodePtr->setColor(r,g,b);
                                std::cout << "Added Node value "<<nodePtr->getValue()<<std::endl;
                            }
                        }
                    }

                }
            }
        }
    }

    tree.writeBinary("automated_tree.bt");
    cout << "wrote example file automated_tree.bt" << endl << endl;

    return 0;
}
