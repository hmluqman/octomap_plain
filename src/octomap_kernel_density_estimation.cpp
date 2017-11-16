#include "octomap_kernel_density_estimation.h"

mgc::KDEOctomap::KDEOctomap(double resolution, int h)
    : resolution_(resolution)
    ,h_(h)
{
    tree_ = new octomap::ColorOcTree(resolution_);
}
void mgc::KDEOctomap::treeCreation(float xStart, float yStart, float zStart, unsigned int step,
                                   unsigned int xNum, unsigned int yNum)
{
    occupied_cells_.clear();
    leafs_node_.resize(0);
    int xLimit = xStart+step*xNum;
    int yLimit = yStart+step*yNum;
    int z = 0;
    std::pair<octomap::KeySet::iterator,bool> ret = std::make_pair(occupied_cells_.begin(), false);

    for (float x=xStart; x<xLimit; x= x+step)
    {
        for (float y=yStart; y<yLimit; y= y+step)
        {
            octomap::point3d endpoint((float) x*1.00f, (float) y*1.00f, (float) z*1.00f);
            octomap::OcTreeKey key;
            //Converts a 3D coordinate into a 3D OcTreeKey, with boundary checking
            if (tree_->coordToKeyChecked(endpoint, key))
            {
                ret.second = false;
                // get the results of insert function - returns false if key is already present

                ret = occupied_cells_.insert(key);
                octomap::ColorOcTreeNode* result = tree_->search(endpoint);
                if (ret.second)
                {
                    if (result == NULL)
                    {
                        octomap::ColorOcTreeNode* nodePtr = tree_->updateNode(endpoint, true);
                        leafs_node_.push_back(nodePtr);
                        nodePtr->setValue(0.0);
                       // std::cout<<"By default value of node "<<nodePtr->getValue()<<std::endl;

                        // nodePtr->setColor(255,0,0);
                        //nodePtr->setValue(0.75);
                    }
                    else
                    {
                       float oldValue = result->getValue();
                       octomap::ColorOcTreeNode* nodePtr = tree_->updateNode(endpoint, true);
                       leafs_node_.push_back(nodePtr);
                       nodePtr->setValue(oldValue);
                //       std::cout<<"Already existed node "<<endpoint.x()<<" "<<endpoint.y()<<" "<< "value "<<oldValue<<std::endl;
                    }
                }
            }
        }
    }
}

void mgc::KDEOctomap::kdeImplementation()
{
    float minimum = 0.0, maximum = 1.0, ratio;
    unsigned int r, g, b;
    int z = 0;
    octomap::KeySet::iterator keyIt = occupied_cells_.begin();
    for(std::vector<octomap::ColorOcTreeNode*>::iterator it = leafs_node_.begin();
        it!= leafs_node_.end(); ++it,++keyIt )
    {
        octomap::OcTreeKey key;
        key = *keyIt;
        octomap::point3d point = tree_->keyToCoord(key);
        for (int x=-h_; x<=h_; x = x+1)
        {
            for (int y=-h_; y<=h_; y = y+1)
            {
               // for (float z=-h_; z<=h_; z++)
               // {
                    float xIncrement = (point.x() + x*resolution_);
                    float yIncrement = (point.y() + y*resolution_);
                    float zIncrement = (point.z() + z*resolution_);

                    octomap::point3d newPoint(xIncrement, yIncrement, zIncrement);
                    octomap::ColorOcTreeNode* result = tree_->search(newPoint);
                    if (result == NULL)
                    {
                        octomap::OcTreeKey key;
                        if (tree_->coordToKeyChecked(newPoint, key))
                        {
                            octomap::ColorOcTreeNode* nodePtr = tree_->updateNode(newPoint, false);
                            float deltaS = sqrt(x*x + y*y + z*z );
                            if (deltaS <= (h_))
                            {
                                float u = deltaS / h_;
                                float prob = 0.75*(1- u*u);
                                if (prob < 0)
                                    prob = 0;
                                nodePtr->setValue(prob);
                                ratio = 2 * (prob - minimum);
                                b = (int)std::max(minimum, 255*(1 - ratio));
                                r = (int)std::max(minimum, 255*(ratio -1));
                                g = 255 - b - r;
                                nodePtr->setColor(r,g,b);
                         //       std::cout<<"SetValue "<<prob<<" Point "<<xIncrement<< " "<<yIncrement<<std::endl;
                            }
                        }
                    }
                    else
                    {
                        float oldValue = result->getValue();
                        float deltaS = sqrt(x*x + y*y + z*z );
                        if (deltaS <= h_)
                        {
                            float u = deltaS / h_;
                            float prob = 0.75*(1- u*u) + oldValue;
                            if (prob < 0)
                                prob = 0;
                            if (prob > 1)
                                prob = 1;
                            result->setValue(prob);
                            ratio = 2 * (prob - minimum);
                            b = (int)std::max(minimum, 255*(1 - ratio));
                            r = (int)std::max(minimum, 255*(ratio -1));
                            g = 255 - b - r;
                            result->setColor(r,g,b);
                        //    std::cout<<"Already Node existed SetValue "<<prob<<" Point "<<xIncrement<< " "<<yIncrement<<std::endl;

                        }
                    }
                //}
            }
        }
    }

}

void mgc::KDEOctomap::parentNodeValueSet()
{
    unsigned int i = 15;
    bool checkChildExistance;
    float maxProb = 0, prob = 0;
    float minimum = 0.0, maximum = 1.0, ratio;
    unsigned int r, g, b;
    for(unsigned int i = 15; i > 12; i--)
    {
        for(octomap::ColorOcTree::iterator it = tree_->begin(i), end=tree_->end(); it!= end; ++it)
        {
            unsigned int childIdx = 0;
            while( (tree_->nodeChildExists(&(*it),childIdx)) == true)
            {
                octomap::ColorOcTreeNode* ptrNode = tree_->getNodeChild(&(*it),childIdx);
                prob = ptrNode->getValue();
                if (maxProb < prob)
                    maxProb = prob;
                //std::cout << "Child Number: "<< childIdx <<" State " << checkChildExistance <<std::endl;
                std::cout << "Childe Value : " << ptrNode->getValue() <<" IsNodeOccupied "<<tree_->isNodeOccupied(*ptrNode) << std::endl;
                //            std::cout << "Node value: " << ptrNode->getValue() << std::endl;
                childIdx++;
            }
//            octomap::ColorOcTreeNode* parentNode = &(*it);
            it->setValue(maxProb);
            ratio = 2 * (maxProb - minimum);
            b = (int)std::max(minimum, 255*(1 - ratio));
            r = (int)std::max(minimum, 255*(ratio -1));
            g = 255 - b - r;
            it->setColor(r,g,b);
        }
    }
}
