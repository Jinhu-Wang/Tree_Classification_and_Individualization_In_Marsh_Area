# Tree classification and individualization in marsh area from 3D point cloud data obtained by airborne laser scanning

Extracting and individualize trees and shrubs in marsh reedbed habitat area using 3D point cloud data obtained by airborne laser scanning

Module 1:
*OostTree* - the tree&shrub segmentation algorithm to obtain points of trees&shrubs.

Parameters:
- *search_radius*: The neighbourhood size for determing number of points and clustering;
- *num_pts_per_cluster*: The minimum number of points per cluster;
- *mean_height_cluster*: To filter the obtained clusters that have low elevation.




Module 2:
*TreeIndividualization* - the tree&shrub individualization based on the obtained tree&shrub points.

Parameters:
- *radius*: The neighbourhood size used for neighbourhood searching;
- *verticalResolution*: The vertical slicing resolution;
- *miniPtsPerCluster*: The minimum number of points that defines a cluster.


  
