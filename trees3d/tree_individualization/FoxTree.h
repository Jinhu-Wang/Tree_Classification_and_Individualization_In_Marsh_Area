#pragma once

// local
#include "nanoflann.hpp"

// System
#include <filesystem>
#include <iostream>
#include <limits>
#include <map>
#include <math.h>
#include <stack>
#include <time.h>
#include <vector>

namespace fs = std::filesystem;
using namespace nanoflann;
typedef KDTreeSingleIndexAdaptor<L2_Simple_Adaptor<double, PointCloud2<double>>,
                                 PointCloud2<double>, 3>
    kdTree;

/**
 * @brief The data structure of the 3D points.
 *
 */
struct Point3D
{
    double x;
    double y;
    double z;

    bool isPushed;
    bool isVisited;

    int ptID;
    int treeID;
    int class_id;
    Point3D()
    {
        x = y = z = 0.0;
        isPushed = isVisited = false;
        ptID = treeID = class_id = -1;
    }
    Point3D &operator=(const Point3D *pt)
    {
        this->x = pt->x;
        this->y = pt->y;
        this->z = pt->z;
        this->isPushed = pt->isPushed;
        this->isVisited = pt->isVisited;
        this->ptID = pt->ptID;
        this->treeID = pt->treeID;
        this->class_id = pt->class_id;

        return *this;
    }
};

/**
 * @brief The data structure of the tree cluster.
 *
 */
struct TreeCluster
{
    std::vector<int> ptIDs;
    Point3D centroid;
    int r, g, b;
    int treeID;

    TreeCluster()
    {
        r = g = b = 0;
        treeID = 0;
    }
};

/**
 * @brief The 3D bounding box
 *
 */
struct BBX
{
    double xMin;
    double yMin;
    double zMin;
    double xMax;
    double yMax;
    double zMax;

    BBX()
    {
        xMax = yMax = zMax = -(std::numeric_limits<double>::max)();
        xMin = yMin = zMin = (std::numeric_limits<double>::max)();
    }
};

/**
 * @brief This class is used to individualize the trees from the point cloud data.
 * @note The class uses the nanoflann library for neighbourhood searching.
 */
class FoxTree
{
public:
    FoxTree();
    FoxTree(std::vector<Point3D> points, double radius, double verticalResolution, int minPtNum);
    ~FoxTree();

    //----------------------------------- Variables -------------------------------------

public:
    // All loaded points in memory;
    Point3D *m_Points;

    // Individualized trees;
    std::map<int, TreeCluster> m_nTrees;

    // Vertical resolution;
    double m_nVerticalResolution;

    // Search radius;
    double m_nRadius;

    // Minimun point number for tree seeds.
    int m_nMinPtSeeds;

    // VoxelCell* m_nCell;
    // Voxel* m_Voxel;

private:
    // Number of loaded tree points;
    int m_nNumPts;

    // AABB of the inpt tree points;
    BBX m_nBBX;

    // Index of individualized trees;
    int m_nTreeIndex;

    // Already parsed tree points;
    std::vector<int> m_nParsedPtIds;

    //---------------------------------- Operations -------------------------------------
public:
    // Initialize the point clouds;
    void initialize(std::vector<Point3D> points, double radius, double verticalResolution, int minPtsNum);

    // Separate trees;
    void separateTrees();
    void separateTrees(int PtOrVoxel, int direction);

    // Output clusters and points;
    void outputClusters(std::string fileName, std::vector<std::vector<int>> ptIDs);
    void outputPts(std::string fileName, std::vector<int> ptIDs);
    void outputTrees(std::string fileName, std::map<int, TreeCluster> trees);

private:
    /**
     * @brief To collect all the files in the given directory.
     *
     * @param directoryPath The directory path.
     * @param outFiles The list of files in the directory.
     */
    void listFilesInDirectory(const std::string &directoryPath, std::vector<std::string> &outFiles);

    // Initialized the class with tree points;
    void loadPoints(std::vector<Point3D> points);

    // Get points between the given heights;
    std::vector<int> getPts(double lower, double higher);

    // Cluster points with radius.
    std::vector<std::vector<int>> clusterPoints(double radius, std::vector<int> ptIndices);

    // Top downwards traversal of the tree points;
    void topDownSeparation(double radius, double verticalResolution);

    // Bottom upwards traversal of the tree points for tree separation;
    void bottomUpSeparation(double radius, double verticalResolution);

    // Concatenate the obtained points to parsed points;
    void ConcatenateToParsedPts(std::vector<int> ptIDs);
    void ConcatenateToParsedPts(std::vector<std::vector<int>> ptIDS);

    // Assign points to their closest tree and return the rest points;
    std::vector<int> assignPtsToTrees(std::vector<int> newPtIDs, double radius);

    // Generate tree clusters from the point clusters;
    void generateTreeClusters(std::vector<std::vector<int>> ptClusters);

    // Apply tree index to every points in the memory;
    void applyTreeIndexToPts(std::pair<int, TreeCluster> treeClusters);
};
