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

    if (pcl::io::loadPCDFile<pcl::PointXYZ> ("/lhome/luqman/Work/octomap_plain/ibeo_local.pcd", cloud) == -1) //* load the file
    {
        PCL_ERROR ("Couldn't read file test_pcd.pcd \n");
        return (-1);
    }

    std::cout << "Loaded points size "<<cloud.size()<<std::endl;

    octomap::OcTree tree (0.2);  // create empty tree with resolution 0.1
    octomap::point3d sensorOrigin(0.0, 0.0, 0.0);
    octomap::Pointcloud octoPointCloud;

    for(int i = 0; i < int(cloud.size()); ++i)
    {
        octomap::point3d point(cloud[i].x, cloud[i].y, cloud[i].z);
        octoPointCloud.push_back(point);
    }

    tree.insertPointCloud(octoPointCloud, sensorOrigin);
    std::cout<< "Tree Size "<<tree.size()<<std::endl;
    int numOfVoxels = 0;
    int numOfOccupiedVoxel = 0, numOfEmptyVoxel = 0;
    for(octomap::OcTree::tree_iterator it = tree.begin_tree(),
           end=tree.end_tree(); it!= end; ++it)
    {
        if(tree.isNodeOccupied(*it))
        {
            octomap::OcTreeKey key;
            key = it.getKey();
            octomap::point3d poin = tree.keyToCoord(key);
            numOfOccupiedVoxel++;
        }
    }
//std::cout <<"Num of leaf nodes "<< tree.getNumLeafNodes;
//    std::cout << "Total num of Voxels " << numOfVoxels <<std::endl;
    std::cout << "Num of Occupied Voxels " << numOfOccupiedVoxel <<std::endl;
//    std::cout << "Num of Empty Voxels " << numOfEmptyVoxel <<std::endl;
    tree.writeBinary("simple_tree.bt");

    return 0;
}
