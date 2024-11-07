/*
 *	Copyright (C) 2016 by
 *       Jinhu Wang (jinhu.wang@tudelft.nl)
 *       Roderik Lindenbergh (r.c.lindenbergh@tudelft.nl) [http://doris.tudelft.nl/~rlindenbergh/]
 *       Laser and Optical Remote Sensing
 *	Dept. of Geoscience and Remote Sensing
 *	TU Delft, https://tudelft.nl
 *
 *	This is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License Version 3
 *	as published by the Free Software Foundation.
 *
 *	TreeSeparation is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

#include "../LASlib/lasreader.hpp"
#include "FoxTree.h"

/**
 * @brief Convert string to wide string
 *
 * @param str  The input string
 * @return std::wstring The converted wide string
 */
std::wstring stringToWString(const std::string &str)
{
    if (str.empty())
        return std::wstring();
    int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstr(sizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], sizeNeeded);
    return wstr;
}

/**
 * @brief To convert wide string to string
 *
 * @param wstr  The wide string
 * @return std::string The converted string.
 */
std::string wstringToString(const std::wstring &wstr)
{
    if (wstr.empty())
        return std::string();
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string str(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], sizeNeeded, NULL, NULL);
    return str;
}

/**
 * @brief Get all files in the given directory.
 *
 * @param directoryPath The given directory.
 * @param outFiles The names of the files in the given directory.
 */
void ListFilesInDirectory(const std::string &directoryPath, std::vector<std::string> &outFiles)
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

/**
 * @brief Read the las files of the given file path.
 *
 * @param filePath The path of the file
 * @return std::vector<Point3D> The 3D points of the file.
 */
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

/**
 * @brief This is the main function to individualize the trees from the point cloud data.
 *
 */
void main()
{
    /// The directory of the input directory;
    std::string las_dir = "path\to\input\las\files";

    /// The output directory of the output tree individualization;
    std::string out_dir = "path\to\output\las\files";

    std::vector<std::string> las_files;
    ListFilesInDirectory(las_dir, las_files);

    // Parameters;
    const double radius = 2.0;
    const double verticalResolution = 0.5;
    const int miniPtsPerCluster = 10;

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
