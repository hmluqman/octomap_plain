#include <iostream>
#include <octomap/octomap.h>
#include <octomap/OcTree.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

using namespace std;
using namespace octomap;
int main()
{
    cout << "Hello World!" << endl;

//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ> cloud ;

    if (pcl::io::loadPCDFile<pcl::PointXYZ> ("/lhome/luqman/Work/octomap_plain/ibeo_local.pcd", cloud) == -1) //* load the file
    {
        PCL_ERROR ("Couldn't read file test_pcd.pcd \n");
        return (-1);
    }
    std::cout << "Loaded  points "<<cloud.size()<<std::endl;
    octomap::OcTree tree (0.1);  // create empty tree with resolution 0.1
    octomap::point3d sensorOrigin(0.0, 0.0, 0.0);

    octomap::KeySet occupied_cells;
    // define a std pair to get results of insert function later
    std::pair<octomap::KeySet::iterator,bool> ret = std::make_pair(occupied_cells.begin(), false);

    for(int i = 0; i < int(cloud.size()); ++i)
    {
        octomap::point3d point(cloud[i].x, cloud[i].y, cloud[i].z);
        // occupied endpoint
        octomap::OcTreeKey key;
        if (tree.coordToKeyChecked(point, key))
        {
            ret.second = false;
            // get the results of insert function - returns false if key is already present
            ret = occupied_cells.insert(key);
            if (ret.second)
                tree.updateNode(key, true);
        }

    }
/*

    octomap::Pointcloud octoPointCloud;
    for(int i = 0; i < int(cloud.size()); ++i)
    {
        octomap::point3d point(cloud[i].x, cloud[i].y, cloud[i].z);
        octoPointCloud.push_back(point);
    }
    tree.insertPointCloud(octoPointCloud, sensorOrigin);

*/
    std::cout<<"Octree Size "<<tree.size();

    for(octomap::OcTree::leaf_iterator it = tree.begin_leafs(),
           end=tree.end_leafs(); it!= end; ++it)
    {
      //manipulate node, e.g.:
 //     std::cout << "Node center: " << it.getCoordinate() << std::endl;
    //    if(tree.isNodeOccupied(*it))
  //      std::cout << "Node center: " << it->getOccupancy() << std::endl;
    }

    std::cout << endl;
    tree.writeBinary("simple_tree.bt");
    std::cout << "wrote example file simple_tree.bt" << endl << endl;
    std::cout << "now you can use octovis to visualize: octovis simple_tree.bt"  << endl;
    std::cout << "Hint: hit 'F'-key in viewer to see the freespace" << endl  << endl;


    return 0;
}
