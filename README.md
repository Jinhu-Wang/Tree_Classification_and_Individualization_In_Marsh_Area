# Tree classification and individualization in marsh area from 3D point cloud data obtained by airborne laser scanning


## Table of Contents

- [Overview](#overview)
- [File Structure](#file-structure)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage Instructions](#usage-instructions)
- [Output Files](#output-files)
- [Validation Data](#validation-data)
- [License](#license)
- [Contact](#contact)

## Overview 
This repository holds the `C++` scripts and validation datasets for segmenting trees&shrubs from 3D point cloud data ([AHN4](https://www.arcgis.com/home/webscene/viewer.html?webscene=c6db29808aad459cbf6488cd96828e9a)) obtained by Airborne Laser Scanning (ALS) in marsh area of [Oostvaardersplassen](https://www.staatsbosbeheer.nl/uit-in-de-natuur/locaties/oostvaardersplassen).

### Source code


### Validation data


## File Structure

```plaintext
project_directory/
│
├── data/
│   ├── 0_json/DSM/kaartbladindex.json      # JSON file with tile information
│   ├── 1_shpfiles/AWD_sampling_area.shp    # Shapefile with sampling area
│   ├── 2_downloaded_geotiff/DSM/           # Directory for downloaded GeoTIFFs
│   ├── 3_clipped/DSM/                      # Directory for clipped GeoTIFFs
│   └── 4_merged/DSM/                       # Directory for final merged GeoTIFF
│
└── dsm.py                                  # Main Python script
```


## Module 1:

_OostTree_ - the tree&shrub segmentation algorithm to obtain points of trees&shrubs.

Parameters:

- _search_radius_: The neighbourhood size for determing number of points and clustering;
- _num_pts_per_cluster_: The minimum number of points per cluster;
- _mean_height_cluster_: To filter the obtained clusters that have low elevation.

## Module 2:

_TreeIndividualization_ - the tree&shrub individualization based on the obtained tree&shrub points.

Parameters:

- _radius_: The neighbourhood size used for neighbourhood searching;
- _verticalResolution_: The vertical slicing resolution;
- _miniPtsPerCluster_: The minimum number of points that defines a cluster.

## Example

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
