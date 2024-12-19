
// System
#include <iostream>
#include <string>
#include <vector>


// Local
#include <3rd_party/lastools/LASlib/inc/lasreader.hpp>
#include <trees3d/tree_individualization/FoxTree.h>

namespace fs = std::filesystem;

void ListFilesInDirectory(const std::string &directoryPath, std::vector<std::string> &outFiles)
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
                // Add only regular files to the output vector
                if (fs::is_regular_file(entry.status()))
                {
                    outFiles.push_back(entry.path().filename().string()); // Store only the file name
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

std::vector<Point3D> readLasFile(std::string filePath)
{
    std::vector<Point3D> pts;

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
        Point3D pt;
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

int main(int argc, char **argv)
{

    std::string inputLasDir = "/home/jinhu/Work/Projects/Cpp/Trees/resources/data/tree_individualization/input";

    std::string outputTreeDir = "/home/jinhu/Work/Projects/Cpp/Trees/resources/data/tree_individualization/output";

    std::vector<std::string> las_files;
    // List all LAS/LAZ files in the input directory;
    ListFilesInDirectory(inputLasDir, las_files);

    // Parameters;
    const float radius = 2.0;
    const double verticalResolution = 1.0;
    const int minPtsPerCluster = 10;

    for (size_t i = 0; i < las_files.size(); ++i)
    {
        std::vector<Point3D> treePts;
        std::string las_file_path = inputLasDir + "/" + las_files[i];
        treePts = readLasFile(las_file_path.c_str());

        auto foxtree = new FoxTree(treePts, radius, verticalResolution, minPtsPerCluster);

        foxtree->separateTrees(1, 1);
        std::string outputTree = outputTreeDir + "/" + las_files[i] + ".xyz";
        foxtree->outputTrees(outputTree.c_str(), foxtree->m_nTrees);

        if (foxtree)
        {
            delete foxtree;
            foxtree = nullptr;
        }
    }

    return 0;
}
