
#include "TreeClassifier.h"

// #include "../LASlib/lasreader.hpp"

#include <3rd_party/lastools/LASlib/inc/lasreader.hpp>
#include <3rd_party/lastools/LASlib/inc/laswriter.hpp>
#include <3rd_party/lastools/LASzip/src/mydefs.hpp>

#include <algorithm>
#include <numeric>
#include <stack>

TreeClassifier::TreeClassifier()
    : m_search_radius(1.0),
      m_num_pts_per_cluster(10),
      m_media_height_cluster(2.0)
{
}

TreeClassifier::~TreeClassifier()
{
}
/*
std::wstring TreeClassifier::stringToWString(const std::string &str)
{
    if (str.empty())
        return std::wstring();
    int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstr(sizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], sizeNeeded);
    return wstr;
}

std::string TreeClassifier::wstringToString(const std::wstring &wstr)
{
    if (wstr.empty())
        return std::string();
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string str(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], sizeNeeded, NULL, NULL);
    return str;
}

void TreeClassifier::ListFilesInDirectory(const std::string &directoryPath, std::vector<std::string> &outFiles)
{
    std::wstring searchPath = stringToWString(directoryPath) + L"\\*";
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                outFiles.push_back(wstringToString(findData.cFileName));
            }
        } while (FindNextFile(hFind, &findData) != 0);
        FindClose(hFind);
    }
    else
    {
        std::cerr << "Failed to list files in directory." << std::endl;
    }
}
*/
void TreeClassifier::listFilesInDirectory(const std::string &directoryPath, std::vector<std::string> &outFiles)
{
    try
    {
        // Clear the output vector to avoid appending to existing data
        outFiles.clear();

        // Check if the path exists and is a directory
        if (fs::exists(directoryPath) && fs::is_directory(directoryPath))
        {
            // Iterate through the directory
            for (const auto &entry : fs::directory_iterator(directoryPath))
            {
                // Add only regular files to the output vector with full paths
                if (fs::is_regular_file(entry.status()))
                {
                    // Store full path as a string
                    // outFiles.push_back(entry.path().string());
                    outFiles.push_back(entry.path().filename().string());
                }
            }
        }
        else
        {
            std::cerr << "The path specified is not a valid directory: " << directoryPath << std::endl;
        }
    }
    catch (const fs::filesystem_error &ex)
    {
        std::cerr << "Filesystem error: " << ex.what() << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

void TreeClassifier::initiateParameters(std::string in_las_dir, std::string out_tree_dir)
{

    this->m_input_las_dir = in_las_dir;

    this->m_output_tree_dir = out_tree_dir;

    this->listFilesInDirectory(this->m_input_las_dir, this->m_las_files);

    return;
}

void TreeClassifier::parseEachLasFile(float radius, float min_pt, float median_height)
{
    if (this->m_las_files.empty())
        return;

    this->m_search_radius = radius;
    this->m_num_pts_per_cluster = min_pt;
    this->m_media_height_cluster = median_height;

    for (int i = 0; i < this->m_las_files.size(); ++i)
    {
        std::string fileName = this->m_las_files.at(i);

        std::string filePathName = this->m_input_las_dir + "/" + fileName;
        // std::string filePathName = fileName;
        std::vector<dPoint3D> lasPts = readLasFile(filePathName);

        std::vector<dPoint3D> treePts;

        std::vector<dPoint3D> allPts = identifyTrees(lasPts, treePts);

        std::vector<std::vector<dPoint3D>> tree_cluster = clusterPoints(treePts);

        std::vector<std::vector<dPoint3D>> filtered_tree_clusters = filterMedianHeight(tree_cluster);

        std::string outFilePath = this->m_output_tree_dir + "/" + fileName + ".xyz";
        // std::string outFilePath = fileName + ".xyz";

        FILE *file = fopen(outFilePath.c_str(), "w");
        for (int m = 0; m < filtered_tree_clusters.size(); ++m)
        {
            for (int n = 0; n < filtered_tree_clusters.at(m).size(); ++n)
            {
                fprintf(file, "%lf %lf %lf %d\n",
                        filtered_tree_clusters.at(m).at(n).x,
                        filtered_tree_clusters.at(m).at(n).y,
                        filtered_tree_clusters.at(m).at(n).z,
                        m);
            }
        }
        fclose(file);
    }

    return;
}

std::vector<std::vector<dPoint3D>> TreeClassifier::filterMedianHeight(std::vector<std::vector<dPoint3D>> tree_cluster)
{
    std::vector<std::vector<dPoint3D>> new_clusters;

    for (int i = 0; i < tree_cluster.size(); ++i)
    {
        std::vector<double> heights;
        for (int j = 0; j < tree_cluster.at(i).size(); ++j)
        {
            heights.push_back(tree_cluster.at(i).at(j).z);
        }
        double mean = computeMeanHeight(heights);

        if (mean <= m_media_height_cluster)
        {
            continue;
        }

        new_clusters.push_back(tree_cluster.at(i));
    }

    return new_clusters;
}

double TreeClassifier::computeMedianHeight(std::vector<double> heights)
{
    std::vector<double> v = heights;

    std::sort(v.begin(), v.end());

    size_t n = v.size();

    if (n % 2 == 0)
    {
        // If even, return the average of the two middle elements
        return (v[n / 2 - 1] + v[n / 2]) / 2.0;
    }
    else
    {
        // If odd, return the middle element
        return v[n / 2];
    }
}

double TreeClassifier::computeMeanHeight(std::vector<double> heights)
{
    double sum = std::accumulate(heights.begin(), heights.end(), 0.0);
    double mean = sum / heights.size();

    return mean;
}

std::vector<dPoint3D> TreeClassifier::readLasFile(std::string filePath)
{
    std::vector<dPoint3D> pts;

    LASreadOpener readOpener;
    readOpener.set_file_name(filePath.c_str(), true);

    LASreader *reader = readOpener.open();
    if (!reader || reader->npoints == 0)
    {
        reader->close();
        delete reader;
        reader = nullptr;
    }

    while (reader->read_point())
    {
        dPoint3D pt;
        pt.x = reader->point.get_x();
        pt.y = reader->point.get_y();
        pt.z = reader->point.get_z();
        pt.class_id = reader->point.classification;

        pts.push_back(pt);
    }

    reader->close();
    delete reader;
    reader = nullptr;

    return pts;
}

std::vector<dPoint3D> TreeClassifier::identifyTrees(std::vector<dPoint3D> &lasPts, std::vector<dPoint3D> &treePts)
{
    PointCloud2<double> kdPts;
    kdPts.pts.resize(lasPts.size());
    for (size_t i = 0; i < lasPts.size(); ++i)
    {
        if (lasPts[i].class_id != 1)
        {
            continue;
        }

        kdPts.pts[i].x = lasPts[i].x;
        kdPts.pts[i].y = lasPts[i].y;
        kdPts.pts[i].z = lasPts[i].z;
    }

    kdTree *tree = new kdTree(3, kdPts, KDTreeSingleIndexAdaptorParams(10));
    tree->buildIndex();

    std::vector<std::pair<size_t, double>> pairs;
    SearchParams params;

    std::vector<dPoint3D> all_pts;
    for (size_t i = 0; i < lasPts.size(); ++i)
    {
        dPoint3D pt = lasPts[i];

        if (pt.class_id == 1)
        {
            double queryPt[3];
            queryPt[0] = pt.x;
            queryPt[1] = pt.y;
            queryPt[2] = pt.z;

            const size_t numPt = tree->radiusSearch(&queryPt[0], this->m_search_radius, pairs, params);

            lasPts[i].neigh_count = numPt - 1;

            if (lasPts[i].neigh_count >= this->m_num_pts_per_cluster)
            {
                pt.class_id = 1;
                treePts.push_back(pt);
            }
            else
            {
                pt.class_id = 0;
            }
        }
        else
        {
            pt.class_id = 0;
        }

        all_pts.push_back(pt);
    }

    if (tree)
    {
        delete tree;
        tree = nullptr;
    }

    return all_pts;
}

/// <summary>
///
/// </summary>
/// <param name="treePts"></param>
/// <returns></returns>
std::vector<std::vector<dPoint3D>> TreeClassifier::clusterPoints(std::vector<dPoint3D> &treePts)
{
    std::vector<std::vector<dPoint3D>> Clusters;
    Clusters.clear();

    if (treePts.size() == 0)
    {
        return Clusters;
    }

    for (size_t i = 0; i < treePts.size(); ++i)
    {
        treePts[i].id = i;
    }

    PointCloud2<double> kdPts;
    kdPts.pts.resize(treePts.size());
    for (size_t i = 0; i < treePts.size(); ++i)
    {
        kdPts.pts[i].x = treePts[i].x;
        kdPts.pts[i].y = treePts[i].y;
        kdPts.pts[i].z = treePts[i].z;
    }

    kdTree *tree = new kdTree(3, kdPts, KDTreeSingleIndexAdaptorParams(10));
    tree->buildIndex();

    std::vector<std::pair<size_t, double>> pairs;
    SearchParams params;

    for (size_t i = 0; i < treePts.size(); ++i)
    {
        dPoint3D pt = treePts[i];

        if (pt.isVisited)
            continue;

        std::vector<dPoint3D> currCluster;
        currCluster.clear();

        std::stack<dPoint3D> tempStack;
        tempStack.push(pt);

        treePts[i].isPushed = true;

        while (!tempStack.empty())
        {
            dPoint3D currPt = tempStack.top();
            if (!currPt.isVisited)
            {
                currCluster.push_back(currPt);
                treePts[currPt.id].isVisited = true;
            }
            tempStack.pop();

            double queryPt[3];
            queryPt[0] = currPt.x;
            queryPt[1] = currPt.y;
            queryPt[2] = currPt.z;

            const size_t numPairs = tree->radiusSearch(&queryPt[0], 1.0, pairs, params);
            for (size_t j = 0; j < numPairs; ++j)
            {
                if (!treePts[pairs[j].first].isPushed)
                {
                    tempStack.push(treePts[pairs[j].first]);
                    treePts[pairs[j].first].isPushed = true;
                }
            }
        }
        if (currCluster.size() >= 20)
        {
            Clusters.push_back(currCluster);
        }
    }

    if (tree)
    {
        delete tree;
        tree = nullptr;
    }

    return Clusters;
}
