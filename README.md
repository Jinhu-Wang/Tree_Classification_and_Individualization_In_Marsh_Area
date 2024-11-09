# Tree classification and individualization in marsh area from 3D point cloud data obtained by airborne laser scanning

This repository holds the *C++* scripts and validation datasets for first segmenting trees&shrubs from 3D point cloud data obtained by Airborne Laser Scanning (ALS) in marsh area of [Oostvaardersplassen](https://www.staatsbosbeheer.nl/uit-in-de-natuur/locaties/oostvaardersplassen). 








Extracting and individualize trees and shrubs in marsh reedbed habitat area using 3D point cloud data obtained by airborne laser scanning

## Module 1:
*OostTree* - the tree&shrub segmentation algorithm to obtain points of trees&shrubs.

Parameters:
- *search_radius*: The neighbourhood size for determing number of points and clustering;
- *num_pts_per_cluster*: The minimum number of points per cluster;
- *mean_height_cluster*: To filter the obtained clusters that have low elevation.





## Module 2:
*TreeIndividualization* - the tree&shrub individualization based on the obtained tree&shrub points.


Parameters:
- *radius*: The neighbourhood size used for neighbourhood searching;
- *verticalResolution*: The vertical slicing resolution;
- *miniPtsPerCluster*: The minimum number of points that defines a cluster.


  
## Example

``` javascript {.line-numbers}

void main()
{
    /// The directory of the input directory;
    std::string las_dir = "path\to\input\las\files";

    /// The output directory of the output tree individualization;
    std::string out_dir = "path\to\output\las\files";

    /// Get all the las files in the given directory;
    std::vector<std::string> las_files;
    ListFilesInDirectory(las_dir, las_files);

    /// Parameters;

    // The radius for neighbourhood searching.
    const double radius = 2.0;
    // The vertical resolution;
    const double verticalResolution = 0.5;
    // The minimum number of points for a cluster;
    const int miniPtsPerCluster = 10;

    /// Individualize the trees from the point cloud data;
    for (size_t i = 0; i < las_files.size(); ++i)
    {
        std::vector<Point3D> treePts;

        std::string las_file_path = las_dir + las_files[i];
        treePts = readLasFile(las_file_path.c_str());

        FoxTree *foxTree = new FoxTree(treePts, radius, verticalResolution, miniPtsPerCluster);

        foxTree->separateTrees(1, 1);
        std::string outputTree = out_dir + las_files[i] + ".xyz";
        foxTree->outputTrees(outputTree.c_str(), foxTree->m_nTrees);
        std::cout << "File: " << las_files[i] << " finished." << std::endl;

        if (foxTree)
        {
            delete foxTree;
            foxTree = nullptr;
        }
    }

    return;
}

```

