# Prerequisites
In order to run the code you must have installed the following `MATLAB` toolboxes: 
* Image Processing Toolbox
* Computer Vision Toolbox

The folder structure must look like the following:
```
.
├── *.m
├── calibrationPattern
├── data
│   ├── left_dist_sample
│   │   ├── test
│   │   └── validation
│   ├── left_img_sample
│   │   ├── test
│   │   └── validation
│   ├── right_dist_sample
│   │   ├── test
│   │   └── validation
│   └── right_img_sample
│       ├── test
│       └── validation
├── evalutationCode
```
That is, the `MATLAB` files must be unzipped/lie in the root directory.

# How to run
Run `main.m`. This will create a file named `motion_estimation.txt` that contains the results from the camera motion estimation. The file will be placed in the `evalutationCode` folder. Navigate to the `evalutationCode` folder and run `evaluationCode.m` in order to produce the evaluation results. 

If you wish to switch from `validation` set to `test` set, or vice versa, change the values for the variables named `fldr_left`, `fldr_right` and `fldr_dist_left` to the desired image sequence. 


