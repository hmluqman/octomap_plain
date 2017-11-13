#include <octomap/octomap.h>
#include <octomap/OcTree.h>

using namespace std;

int main(int argc, char** argv)
{

    float resolution = 0.1;
    int h = 3;
    octomap::OcTree tree (resolution);  // create empty tree with resolution 0.1
    int z = 0;
    octomap::KeySet occupied_cells;
    // define a std pair to get results of insert function later
    std::pair<octomap::KeySet::iterator,bool> ret = std::make_pair(occupied_cells.begin(), false);

    for (int x=1; x<10; x= x+4)
    {
        for (int y=-3; y<4; y= y+3)
        {
            octomap::point3d endpoint ((float) x*1.00f, (float) y*1.00f, (float) z*1.00f);
            octomap::OcTreeKey key;
            if (tree.coordToKeyChecked(endpoint, key))
            {
                ret.second = false;
                // get the results of insert function - returns false if key is already present
                ret = occupied_cells.insert(key);
                if (ret.second)
                    tree.updateNode(endpoint, true); // integrate 'occupied' measurement
            }
        }
    }

    octomap::KeySet empty_cells;
    // define a std pair to get results of insert function later
    std::pair<octomap::KeySet::iterator,bool> retemty = std::make_pair(empty_cells.begin(), false);

    for(octomap::OcTree::leaf_iterator it = tree.begin_leafs(), end=tree.end_leafs(); it!= end; ++it)
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
                    float xIncrement = (point.x() + x); float yIncrement = (point.y() + y); float zIncrement = (point.z() + z);

                    octomap::point3d endpoint(xIncrement, yIncrement, zIncrement);

                    octomap::OcTreeKey key;
                    if (tree.coordToKeyChecked(endpoint, key))
                    {
                        retemty.second = false;
                        // get the results of insert function - returns false if key is already present
                        retemty = empty_cells.insert(key);
                        if (retemty.second)
                            tree.updateNode(endpoint, false); // integrate 'occupied' measurement
                    }

                }
            }

        }

    }

/*
    for(octomap::OcTree::leaf_iterator it = tree.begin_leafs(), end=tree.end_leafs(); it!= end; ++it)
    {
            octomap::OcTreeKey key;
            key = it.getKey();
            octomap::point3d point = tree.keyToCoord(key);
//            std::cout <<"Point = "<<point.x()<<" "<<point.y()<<" "<<point.z()<<std::endl;
//            std::cout <<"Key = "<<key.k[0]<<" "<<key.k[1]<<" "<<key.k[2]<<std::endl;
    }
*/
    tree.writeBinary("automated_tree.bt");
    cout << "wrote example file automated_tree.bt" << endl << endl;

    return 0;
}
