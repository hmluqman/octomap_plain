#include <octomap/octomap.h>
#include <octomap/OcTree.h>
#include <octomap/ColorOcTree.h>
#include <math.h>
#include <algorithm>    //std::max
#include <vector>

namespace mgc
{
    class KDEOctomap
    {
    public:
        KDEOctomap (double resolution, int h);
        void treeCreation(float xStart, float yStart, float zStart, unsigned int step,
                          unsigned int xNum, unsigned int yNum);

        inline void writeTree(){tree_->write("automate/lhome/luqman/Work/radar_ros_ws/d_tree0.ot");}
        octomap::ColorOcTree* tree_;
        void kdeImplementation();
        void parentNodeValueSet();

    protected:
        /// Resolution of Octree
        double resolution_;
        /// Number of Cells for Probability Computation
        int h_;
        octomap::KeySet occupied_cells_;
        std::vector<octomap::ColorOcTreeNode*> leafs_node_;



    };

}
