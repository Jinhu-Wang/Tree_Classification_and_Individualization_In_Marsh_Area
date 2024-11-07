

#include <iostream>
#include <string>
#include <vector>

#include "TreeClassifier.h"

#define NONMIN
#define NONMAX

/**
 * @brief The main function to classify the trees from the point cloud data in the marsh area.
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char **argv)
{
    std::string inputLasDir = "Input\Directory\of\Las\Files";
    std::string outputTreeDir = "Output\Directory\of\Trees";

    // Parameters;
    const float search_radius = 1.0;

    // Number of points per cluster;
    const size_t num_pts_per_cluster = 6;

    // Median height of the cluster;
    const float median_height_cluster = -2.0;

    TreeClassifier *tree_classifier = new TreeClassifier;
    tree_classifier->initiateParameters(inputLasDir, outputTreeDir);
    tree_classifier->parseEachLasFile(search_radius, num_pts_per_cluster, median_height_cluster);

    if (tree_classifier)
    {
        delete tree_classifier;
        tree_classifier = nullptr;
    }
    return 0;
}
