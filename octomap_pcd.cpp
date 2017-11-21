#include <iostream>
#include <octomap/octomap.h>
#include <octomap/OcTree.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

using namespace std;
using namespace octomap;

int main()
{
    pcl::PointCloud<pcl::PointXYZ> cloud ;

    if (pcl::io::loadPCDFile<pcl::PointXYZ> ("/lhome/luqman/Work/octomap_plain/ibeo_local.pcd", cloud) == -1)
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

    for(octomap::OcTree::tree_iterator it = tree.begin_tree(), end=tree.end_tree(); it!= end; ++it)
    {
            octomap::OcTreeKey key;
            key = it.getKey();
//            octomap::point3d poin = tree.keyToCoord(key);
            std::cout <<"Key = "<<key.k[0]<<" "<<key.k[1]<<" "<<key.k[2]<<std::endl;
    }

/*
    for(octomap::OcTree::tree_iterator it = tree.begin_tree(), end=tree.end_tree(); it!= end; ++it)
    {
//      std::cout << "Node center: " << it.getCoordinate() << std::endl;
  //    std::cout << "Node size: " << it.getSize() << std::endl;
//      std::cout << "Node value: " << it->getValue() << std::endl;
    }
*/

    octomap::point3d query (0., 0., 0.);
    octomap::OcTreeNode* result = tree.search (query);
   // print_query_info(query, result);

    std::cout << endl;
    tree.writeBinary("simple_tree.bt");

    return 0;
}
