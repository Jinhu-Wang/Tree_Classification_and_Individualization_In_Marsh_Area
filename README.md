# Tree classification and individualization in marsh area from 3D point cloud data obtained by airborne laser scanning

## Table of Contents

- [Overview](#overview)
- [File Structure](#file-structure)
- [Requirements](#requirements)
- [Usage Instructions](#usage-instructions)
- [Validation Data](#validation-data)
- [License](#license)
- [Contact](#contact)

## Overview

This repository holds the scripts and validation datasets for segmenting trees&shrubs from 3D point cloud data ([AHN4](https://www.arcgis.com/home/webscene/viewer.html?webscene=c6db29808aad459cbf6488cd96828e9a)) obtained by Airborne Laser Scanning (ALS) in marsh area of [Oostvaardersplassen](https://www.staatsbosbeheer.nl/uit-in-de-natuur/locaties/oostvaardersplassen) nature reserve.

---

## File Structure

```plaintext
project_directory/
│
├── data/                                   # Directory holds the manually created ground truth and validation data
│   ├── Trees&NonTrees                      # Validation data for [trees&shrubs] segmentation
│   │   ├── 1_GroundTruth                   # Manually created ground truth
│   |   └── 2_Trees_NonTrees_Results        # Results from the scripts
|   |
│   ├── IndividualiTrees                    # Validation data for [trees&shrubs] individualization
|   │   ├── 1_GroundTruth_Groups            # Manually created ground truth
|   │   └── 2_IndividualizedTrees_Results   # [trees&shrubs] individualization from the scripts
│
└── src                                     # Directory holds the source codes
    ├── LASlib                              # 3rdParty LASlib
    ├── TreeClassification                  # Scripts for trees&shrubs segmentation in marsh area
    └── TreeIndividualization               # Scripts for trees&shrubs individualization
```

## Requirements

The `C++` scripts in this repository depends on the **[LASlib](https://conan.io/center/recipes/laslib)** to read point cloud data in **LAS/LAZ** formats.

In this implementation, the scripts of **LASlib** are under directory `[LASlib]` and will be included in the C++ scripts.

## Usage Instructions

### Module 1: [TreeClassification]

This module segments the trees&shrubs points from the original AHN4 point clouds.

There are three parameters:

```bash
    search_radius: The neighbourhood size for determing number of points and clustering;
    num_pts_per_cluster: The minimum number of points per cluster;
    mean_height_cluster: To filter the obtained clusters that have low elevation.
```

### Module 2: [TreeIndividualization]

This module performs the individual tree delineation from the segmented trees&shrubs points.
There are three parameters:

```bash
    radius: The neighbourhood size used for neighbourhood searching;
    verticalResolution: The vertical slicing resolution;
    miniPtsPerCluster: The minimum number of points that defines a cluster.
```

Below shows an example on the usage of Module [TreeIndividualization]:

```javascript {.line-numbers}
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

## Validation data

To create the ground truth data for validating **Module [TreeClassification]**, trees and shrubs in a marsh reedbed habitat were identified manually by referring to both the original 3D point cloud data of [AHN4](https://www.ahn.nl/) and [aerial photographs](https://app.pdok.nl/viewer/#x=153872.61&y=496307.30&z=13.4935&background=BRT-A%20standaard&layers=a301ddc7-c26f-42d8-b367-509ae5ae47d0;2020_ortho25) with a resolution of 8 cm. Then, the points of trees&shrubs were manually clipped and labeled as "trees" while the rest of the points were labeled as "non-trees". A total of 20 square plots of size 50 m × 50 m were created as ground truth in Folder `\data\Trees&NonTrees\1_GroundTruth\`.

Similarly, the ground truth datasets for validating **Module [TreeIndividualization]** were established by manually labeling individual trees with the obtained 3D points of "trees", using the same aerial photographs as reference. A total of 20 patches of trees from the obtained "trees" in **Module [TreeClassification]**. Moreover, these 20 patches were categorized into three difficulty levels: easy, medium and hard. Those ground truth datasets are in Folder `\data\IndividualTrees\1_GroundTruth_Groups`.

The original coordinate reference system was preserved during the creation of ground truth datasets. Thus, the exact geospatial locations of those plots and patches can be found directly with their own coordinates.

## License

MIT License

Copyright (c) 2024 Jinhu Wang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Contact

For any suggestions and bug reports, please contact:

Jinhu Wang

jinhu.wang (at) hotmail (dot) com
