#pragma once

// system
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

// local
#include "BasicStructures.h"
#include "nanoflann.hpp"

namespace fs = std::filesystem;

using namespace nanoflann;
typedef KDTreeSingleIndexAdaptor<L2_Simple_Adaptor<double, PointCloud2<double>>,
                                 PointCloud2<double>, 3>
    kdTree;

/**
 * @brief This class is used to classify the trees from the point cloud data.
 *
 */
class TreeClassifier
{
public:
    TreeClassifier();
    ~TreeClassifier();

    /// <summary>
    /// Initialize the input directory for las files and output directory for trees.
    /// </summary>
    /// <param name="in_las_dir"></param>
    /// <param name="out_tree_dir"></param>
    void initiateParameters(std::string in_las_dir, std::string out_tree_dir);

    /// <summary>
    ///
    /// </summary>
    /// <param name="radius"></param>
    /// <param name="min_pt"></param>
    void parseEachLasFile(float radius, float min_pt, float median_height);

protected:
private:
    /* /// <summary>
    /// Convert string to wide string on windows.
    /// </summary>
    /// <param name="str"></param>
    /// <returns></returns>
    std::wstring stringToWString(const std::string &str);

    /// <summary>
    /// Convert wide string to string on windows.
    /// </summary>
    /// <param name="wstr"></param>
    /// <returns></returns>
    std::string wstringToString(const std::wstring &wstr);

    /// <summary>
    /// List all files in the directory.
    /// </summary>
    /// <param name="directoryPath"></param>
    /// <param name="outFiles"></param>
    void ListFilesInDirectory(const std::string &directoryPath, std::vector<std::string> &outFiles);
    */

    /**
     * @brief To collect all the files in the given directory.
     *
     * @param directoryPath The directory path.
     * @param outFiles The list of files in the directory.
     */
    void listFilesInDirectory(const std::string &directoryPath, std::vector<std::string> &outFiles);

    /// <summary>
    /// Obtain the tree points that has more than the required number of points in the neighbourhood.
    /// Tree points will be labelled as "1" and other points will be "0";
    /// </summary>
    /// <param name="inPts"></param>
    /// <param name="outpts"></param>
    std::vector<dPoint3D> identifyTrees(std::vector<dPoint3D> &inPts, std::vector<dPoint3D> &treePts);

    /// <summary>
    /// Read the points into the container.
    /// </summary>
    /// <param name="filePath"></param>
    /// <returns></returns>
    std::vector<dPoint3D> readLasFile(std::string filePath);

    /// <summary>
    /// Cluster the tree points to clusters (Euclidean space).
    /// </summary>
    /// <param name="treePts"></param>
    /// <returns></returns>
    std::vector<std::vector<dPoint3D>> clusterPoints(std::vector<dPoint3D> &treePts);

    /// <summary>
    /// Obtain the median height of the points.
    /// </summary>
    /// <param name="heights"></param>
    /// <returns></returns>
    double computeMedianHeight(std::vector<double> heights);
    double computeMeanHeight(std::vector<double> heights);

    /// <summary>
    ///
    /// </summary>
    /// <param name="tree_clusters"></param>
    /// <returns></returns>
    std::vector<std::vector<dPoint3D>> filterMedianHeight(std::vector<std::vector<dPoint3D>> tree_clusters);

public:
    /// @brief The list of las files in the input directory.
    std::vector<std::string> m_las_files;

protected:
private:
    /// <summary>
    ///
    /// </summary>
    std::string m_input_las_dir;

    /// <summary>
    ///
    /// </summary>
    std::string m_output_tree_dir;

    /// <summary>
    /// Radius for neighbourhood searching.
    /// </summary>
    float m_search_radius;

    /// <summary>
    /// The number of points per cluster;
    /// </summary>
    float m_num_pts_per_cluster;

    /// <summary>
    /// The median height of the tree cluster;
    /// </summary>
    float m_media_height_cluster;
};
