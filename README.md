# Marsh_Trees_Shrubs
Extracting and individualize trees and shrubs in marsh reedbed habitat area using 3D point cloud data obtained by airborne laser scanning

Step 1:
Run *OostTree* - the tree&shrub segmentation algorithm to obtain points of trees&shrubs.

Parameters:
- *search_radius*: The neighbourhood size for determing number of points and clustering;
- *num_pts_per_cluster*: The minimum number of points per cluster;
- *mean_height_cluster*: To filter the obtained clusters that have low elevation.

Step 2:
Run *ResultsTree* - reformulate the format of extracted trees&shrubs for quality comparison.

Step 3:
Run *QE_TreeNonTree* - evalute the accuracy of the method w.r.t. ground truths.


  
