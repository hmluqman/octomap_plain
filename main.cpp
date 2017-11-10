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
 //   std::cout << "Loaded  points "<<cloud->size();
    octomap::OcTree tree (0.1);  // create empty tree with resolution 0.1
    octomap::point3d sensorOrigin(0.0, 0.0, 0.0);
    octomap::Pointcloud octoPointCloud;

    for(int i = 0; i < int(cloud.size()); ++i)
    {
        octomap::point3d point(cloud[i].x, cloud[i].y, cloud[i].z);
        octoPointCloud.push_back(point);
    }
    tree.insertPointCloud(octoPointCloud, sensorOrigin);

    for(octomap::OcTree::tree_iterator it = tree.begin_tree(),
           end=tree.end_tree(); it!= end; ++it)
    {
      //manipulate node, e.g.:
      std::cout << "Node center: " << it.getCoordinate() << std::endl;
      std::cout << "Node size: " << it.getSize() << std::endl;
//      std::cout << "Node value: " << it->getValue() << std::endl;
    }

    // insert some measurements of occupied cells
/*
    for (int x=-20; x<20; x++) {
      for (int y=-20; y<20; y++) {
        for (int z=-20; z<20; z++) {
          octomap::point3d endpoint ((float) x*0.05f, (float) y*0.05f, (float) z*0.05f);
          tree.updateNode(endpoint, true); // integrate 'occupied' measurement
        }
      }
    }

    // insert some measurements of free cells

    for (int x=-30; x<30; x++) {
      for (int y=-30; y<30; y++) {
        for (int z=-30; z<30; z++) {
          octomap::point3d endpoint ((float) x*0.02f-1.0f, (float) y*0.02f-1.0f, (float) z*0.02f-1.0f);
          tree.updateNode(endpoint, false);  // integrate 'free' measurement
        }
      }
    }
*/
    std::cout << endl;
    std::cout << "performing some queries:" << endl;

    octomap::point3d query (0., 0., 0.);
    octomap::OcTreeNode* result = tree.search (query);
   // print_query_info(query, result);

    std::cout << endl;
    tree.writeBinary("simple_tree.bt");
    std::cout << "wrote example file simple_tree.bt" << endl << endl;
    std::cout << "now you can use octovis to visualize: octovis simple_tree.bt"  << endl;
    std::cout << "Hint: hit 'F'-key in viewer to see the freespace" << endl  << endl;


    return 0;
}
