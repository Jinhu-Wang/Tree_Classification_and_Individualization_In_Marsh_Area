
// System
#include <iostream>
#include <string>
#include <vector>

// Local
#include<trees3d/tree_classification/TreeClassifier.h>

int main(int argc, char **argv)
{
    
    std::string inputLasDir = "/home/jinhu/Work/Projects/Cpp/Trees/resources/data/tree_classification/input";
    
    std::string outputTreeDir = "/home/jinhu/Work/Projects/Cpp/Trees/resources/data/tree_classification/output";


    // Parameters;
    const float search_radius = 1.0;

    // Number of points per cluster;
    const size_t num_pts_per_cluster = 5;

    // Median height of the cluster;
    const float median_height_cluster = -2.0;

    // Classify the trees from the point cloud data.
    auto *tree_classifier = new TreeClassifier;
    // Initiate the parameters.
    tree_classifier->initiateParameters(inputLasDir, outputTreeDir);
    // Parse each las file.
    tree_classifier->parseEachLasFile(search_radius, num_pts_per_cluster, median_height_cluster);

    if (tree_classifier)
    {
        delete tree_classifier;
        tree_classifier = nullptr;
    }

    return 0;
}
