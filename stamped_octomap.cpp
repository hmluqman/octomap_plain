#include <octomap/octomap.h>
#include <octomap/OcTree.h>
#include <octomap/OcTreeStamped.h>
#include <unistd.h>

using namespace std;
using namespace octomap;

OcTreeStamped tree (0.1);  // create empty tree with resolution 0.1

void print_query_info(point3d query, OcTreeNodeStamped* node)
{
  if (node != NULL)
  {
   // node->setValue(0.5);
     // cout << "occupancy probability at " << query << ":\t " << node->getOccupancy() << endl;
    cout << "Value of the Node " << query << ":\t " << node->getValue() << endl;
    cout << "Log Odds at " << query << ":\t " << node->getLogOdds() << endl;
    cout << "Probability " << query << ":\t " << node->getOccupancy() << endl;
  }

  else
    cout << "occupancy probability at " << query << ":\t is unknown" << endl;
  //  cout << "ProbOfBeingHIt " << query << ":\t " << tree.getProbHitLog() << endl;

}

int main(int argc, char** argv)
{

    //Inserting Points in Octomap
    for (int x=-5; x<5; x++)
    {
        for (int y=-5; y<5; y++)
        {
            for (int z=-2; z<2; z++)
            {
                point3d endpoint((float) x*1.00f, (float) y*1.00f, (float) z*1.00f);
                tree.updateNode(endpoint, true); // integrate 'occupied' measurement
            }
        }
    }

    for (int x=-5; x<5; x++)
    {
        for (int y=-5; y<5; y++)
        {
            for (int z=-2; z<2; z++)
            {
                point3d endpoint ((float) x*1.00f+0.5f, (float) y*1.00f+0.5f, (float) z*1.00f+0.5f);
                tree.updateNode(endpoint, false);  // integrate 'free' measurement
            }
        }
    }


    point3d query (1.0, 1.0, 1.0);
    OcTreeNodeStamped* result = tree.search (query);
    print_query_info(query, result);



    point3d endpoint ((float) 1.00f, (float) 1.00f, (float) 1.00f);
    tree.updateNode(endpoint, true); // integrate 'occupied' measurement

    point3d query2 (1.0, 1.0, 1.0);
    result = tree.search (query2);
    print_query_info(query2, result);

    std::cout<< "After third time insert "<<endl;
    tree.updateNode(endpoint, true); // integrate 'occupied' measurement

    result = tree.search (query2);
    print_query_info(query2, result);

    //  cout << "Last Update time " << tree.getLastUpdateTime()<< endl << endl;

    usleep(3000000);
    cout <<endl <<endl<< " Get Prob Miss " << tree.getProbMiss()<< endl << endl;
    cout << " Get Prob Miss log " << tree.getProbMissLog()<< endl << endl;
    tree.setProbMiss(0.475);
//    cout << " After setting ProbMiss; Prob Miss log " << tree.getProbMissLog()<< endl << endl;
    //  tree.setProbMiss(0.1);
    tree.degradeOutdatedNodes(1);

    cout << endl;
    cout << " After Degradation of Probability: " << endl;

//    query = point3d(1.5, 1.5, 1.5);
    result = tree.search (query2);
    print_query_info(query2, result);

    /*
    usleep(3000000);
    tree.degradeOutdatedNodes(1);

    cout << endl;
    cout << " After Second Degradation of Probability: " << endl;

    query = point3d(1.5, 1.5, 1.5);
    result = tree.search (query);
    print_query_info(query, result);

*/
    /*

    point3d endpoint ((float) 1.00f, (float) 1.00f, (float) 1.00f);
    //  tree.updateNode(endpoint, true); // integrate 'occupied' measurement
    // insert some measurements of free cells

    point3d startpoint ((float) (1.00f+0.5f), (float) (1.00f+0.5f), (float) (1.00f+0.5f));
    //  tree.updateNode(startpoint, false);  // integrate 'free' measurement

    cout << endl;
    cout << "performing some queries:" << endl;

    point3d query (1.5, 1.5, 1.5);
    OcTreeNodeStamped* result = tree.search (query);
    print_query_info(query, result);

  query = point3d(1.5,1.5,1.5);
  result = tree.search (query);
  print_query_info(query, result);

  query = point3d(-1.,-1.,-1.);
  result = tree.search (query);
  print_query_info(query, result);

    //  cout << "Last Update time " << tree.getLastUpdateTime()<< endl << endl;

    usleep(3000000);
    cout << " Get Prob Miss " << tree.getProbMiss()<< endl << endl;
    cout << " Get Prob Miss log " << tree.getProbMissLog()<< endl << endl;
    tree.setProbMiss(0.475);
    cout << " After setting ProbMiss; Prob Miss log " << tree.getProbMissLog()<< endl << endl;
    //  tree.setProbMiss(0.1);
    tree.degradeOutdatedNodes(1);

    cout << endl;
    cout << " After Degradation of Probability: " << endl;

    query = point3d(1.5, 1.5, 1.5);
    result = tree.search (query);
    print_query_info(query, result);
    usleep(3000000);
    tree.degradeOutdatedNodes(1);

    cout << endl;
    cout << " After Second Degradation of Probability: " << endl;

    query = point3d(1.5, 1.5, 1.5);
    result = tree.search (query);
    print_query_info(query, result);

    query = point3d(1.5,1.5,1.5);
    result = tree.search (query);
    print_query_info(query, result);

    query = point3d(-1.,-1.,-1.);
    result = tree.search (query);
    print_query_info(query, result);

    tree.writeBinary("simple_tree.bt");
    cout << "wrote example file simple_tree.bt" << endl << endl;
    cout << "now you can use octovis to visualize: octovis simple_tree.bt"  << endl;
    cout << "Hint: hit 'F'-key in viewer to see the freespace" << endl  << endl;
*/
    return 0;
}
