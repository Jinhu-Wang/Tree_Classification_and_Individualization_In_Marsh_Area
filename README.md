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

This repository holds the scripts and validation datasets for classifying **[trees&shrubs]** from 3D point cloud data ([AHN4](https://www.arcgis.com/home/webscene/viewer.html?webscene=c6db29808aad459cbf6488cd96828e9a)) obtained by Airborne Laser Scanning (ALS) in marsh area of [Oostvaardersplassen](https://www.staatsbosbeheer.nl/uit-in-de-natuur/locaties/oostvaardersplassen) nature reserve.

---

## File Structure

```plaintext
project_directory/
│
├── 3rd_party/      # The third party software, i.e. LAStools.
|
├── cmake/          # The cmake environment settings.
|
├── examples/       # Two examples on classification and individualization of trees.
|
├── resources/      # Typically holds the example datasets.
|      ├── data/    # Testing data.
|      |
|      └── validation_data/   # Manually created ground truth.
|
|
└── trees3D/                                # Directory holds the source codes
    ├── tree_classification                 # C++ scripts of classify trees from reedbeds vegetation.
    |
    └── tree_individualization              # C++ scripts of individualize single trees.

```

## Requirements

The `C++` scripts in this repository depends on the **[LAStools](https://lastools.github.io/)** to read point cloud data in **LAS/LAZ** formats.

To use the scripts, a `C++` compiler, i.e. `g++`,`gcc`, `mscv`, `clang++`, etc., should be installed.

## Usage Instructions

### There are three ways to build:

- Option 1: Using CMake to generate makefiles and then 'make' (on Linux/macOS).

  - On Linux or maxOS, simply:
    ```
    $ cd path-to-root-dir-of-Trees
    $ mkdir release  && cd release
    $ cmake -DCMAKE_BUILD_TYPE=release ..
    $ make
    ```
  - On Windows with Microsoft Visual Studio, use the `x64 Native Tools Command Prompt for VS XXXX` (**don't** use the x86 one), then
    ```
      $ cd path-to-root-dir-of-Trees
      $ mkdir Release
      $ cd Release
      $ cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ..
      $ nmake
    ```

- Option 2: Use any IDE that can directly handle CMakeLists files to open the `CMakeLists.txt` in the **root** directory of Trees3D.
  Then you should have obtained a usable project and just build it. I recommend using
  [CLion](https://www.jetbrains.com/clion/) or [QtCreator](https://www.qt.io/product). For Windows users: your IDE must be set for `x64`.
  
- Option 3: Use CMake-Gui to generate project files for your IDE. Then load the project to your IDE and build it. For Windows users: your IDE must be set for `x64`.

### Module 1: [TreeClassification]

This module classifies the original points from the AHN4 point clouds to **[trees&shrubs]** and **[others]** in Oostvaardersplassen marsh area.

There are three parameters:

```
    search_radius: The neighbourhood size for determing number of points and clustering;
    num_pts_per_cluster: The minimum number of points per cluster;
    mean_height_cluster: To filter the obtained clusters that have low height.
```

### Module 2: [TreeIndividualization]

This module performs the individual tree delineation from the classified **[trees&shrubs]** points.
There are three parameters:

```
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

To create the ground truth data for validating **Module [TreeClassification]**, trees and shrubs in a marsh reedbed habitat were identified manually by referring to both the original 3D point cloud data of [AHN4](https://www.ahn.nl/) and [aerial photographs](https://app.pdok.nl/viewer/#x=153872.61&y=496307.30&z=13.4935&background=BRT-A%20standaard&layers=a301ddc7-c26f-42d8-b367-509ae5ae47d0;2020_ortho25) with a resolution of 8 cm. Then, the points of [trees&shrubs] were manually clipped and labeled as **`1`** while the rest of the points were labeled as **`0`**. A total of **20 square plots** of size 50 m were created as ground truth in Folder `\resources\validation_data\tree_classification\`.

Similarly, the ground truth datasets for validating **Module [TreeIndividualization]** were established by manually labeling individual trees with the obtained 3D points of [trees&shrubs], using the same aerial photographs as reference. A total of **20 patches** of trees from the obtained [trees&shrubs] in **Module [TreeClassification]**. Moreover, these 20 patches were categorized into three difficulty levels: easy, medium and hard. Those ground truth datasets are in Folder `\resources\validation_data\tree_individualization\`.

**The original coordinate reference system was preserved during the creation of ground truth datasets. Thus, the exact geospatial locations of those plots and patches can be found directly with their own coordinates.**

## License

MIT License

## Contact

For any suggestions and bug reports, please contact:

Jinhu Wang

jinhu.wang (at) hotmail (dot) com
