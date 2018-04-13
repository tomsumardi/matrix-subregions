# matrix-subregions

### Summary: 
Given as input a two-dimensional array of values (that represent, say, measurements of some signal strength over each cell in a region), we wish to automatically generate a description of the sub- regions of interest, defined as all contiguous groups of adjacent cells for which the signal is greater than a threshold value T. Cells that are touching at corners are considered to be (diagonally) adjacent cells.

### Example: 
consider the 6 x 6 array shown in the following diagram. If we have threshold value T = 200, then the three gray-shaded sub-regions identified are the sub-regions of interest. The sub-regions of interest can be identified by listing the X,Y coordinates of the cells they contain. In the example, these would be:
- Lower-left sub-region: { (1,1) }
- Middle "W-shaped" sub-region: { (3,3), (3,2), (4,2), (4,1), (5,1) }
- Upper-right sub-region: { (5,4), (5,5) }

![](https://github.com/tomsumardi/matrix-subregions/blob/master/matrix.png)

##### Inputs:
- a two-dimensional array of values
- a threshold T

##### Output: 
A list of (X,Y) value pairs that identify the center of mass of each region of interest.
(There will be one X,Y coordinate pair for each sub-region.) Define "center of mass" for a given sub-region to be
the average position, expressed in X,Y coordinates, of the cells in that sub-region, each cell's location
being weighted by that cell's signal value.

### how to compile and execute:
cd matrix-subregions/Debug
make clean
make all

./matrix-subregions
Usage is -a <x,y array list> -t <threshold>
example -a "{{0,80,45,95,170,145}:{115,210,60,5,230,220}:{5,0,145,250,245,140}:{15,5,175,250,185,160}:{0,5,95,115,165,250}:{5,0,25,5,145,250}" -t 200

sample test cases:
./matrix-subregions -a "{0,80,45,95,170,145}:{115,210,60,5,230,220}:{5,0,145,250,245,140}:{15,5,175,250,185,160}:{0,5,95,115,165,250}:{5,0,25,5,145,250}"   -t 200
./matrix-subregions -a "{210,250,25,5,60,80}:{0,80,45,95,170,145}:{115,210,60,5,230,220}:{5,0,145,250,245,140}:{15,5,175,250,185,160}:{0,5,95,115,165,250}:{5,0,25,5,145,250}"   -t 200
./matrix-subregions -a "{0,5,95,115}:{5,0,25,210}" -t 200
./matrix-subregions -a "{0,5,95,115}:{5,0,25,21}"  -t 500
./matrix-subregions -a "{10,5,95,115}:{5,20,25,50}" -t 4

