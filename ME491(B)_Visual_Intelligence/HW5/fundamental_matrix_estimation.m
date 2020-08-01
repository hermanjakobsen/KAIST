clear variables;
%% Corresponding image points
img1 = [
    476     785     1;
    424     895     1;
    715     912     1;
    663     1014    1;
    32      1351    1;
    340     2204    1;
    278     2330    1;
    701     1950    1;
    1785    2617    1;
    1632    1958    1;
    2502    1837    1;
    1702    950     1;
    2449    650     1;
    1316    3245    1;
    1260    2048    1;
    1942    2352    1;
    771     153     1;
    2788    1349    1;
    151     210     1;
    1420    274     1;
    ]';

img2 = [
    696     1974    1;
    709     2082    1;
    934     1942    1;
    946     2046    1;
    624     2651    1;
    1334    3211    1;
    1348    3356    1;
    1457    2790    1;
    2788    2770    1;
    2209    2272    1;
    2886    1662    1;
    1702    1429    1;
    2132    773     1;
    2772    3646    1;
    1958    2562    1;
    2758    2437    1;
    621     1354    1;
    2822    1098    1;
    204     1720    1;
    1144    1089    1;
    ]';


%% Eight-Point Algorithm
A = constraint_matrix(img1,img2);
F = fundamental_matrix(A);


%% Normalized Eight-Point Agortihm
[img1_norm,T1] = data_normalization(img1);
[img2_norm,T2] = data_normalization(img2);

A_norm = constraint_matrix(img1_norm, img2_norm);
F_norm = fundamental_matrix(A_norm);
F_og = T2'*F_norm*T1;


%% Visualize results in images
% Draw circles at point correspondences
I1 = imread('hw05_f_img1.jpg');
I1_pts = draw_points(I1,img1);
I2 = imread('hw05_f_img2.jpg');
I2_pts = draw_points(I2,img2);

imwrite(I1_pts,'hw05_f_img1_pts.jpg');
imwrite(I2_pts,'hw05_f_img2_pts.jpg');

% Draw epipolar lines in img2
epi_lines_img2 = epipolarLine(F,img1(1:2,:)')';
I2_lines = draw_epipolar_lines(I2_pts,epi_lines_img2);
imwrite(I2_lines,'hw05_f_img2_epilines.jpg');

epi_lines_img2_norm = epipolarLine(F_og,img1(1:2,:)')';
I2_lines_norm = draw_epipolar_lines(I2_pts,epi_lines_img2_norm);
imwrite(I2_lines_norm,'hw05_f_img2_norm_epilines.jpg');

% Draw epipolar lines in img1
epi_lines_img1 = epipolarLine(F',img2(1:2,:)')';
I1_lines = draw_epipolar_lines(I1_pts,epi_lines_img1);
imwrite(I1_lines,'hw05_f_img1_epilines.jpg');

epi_lines_img1_norm = epipolarLine(F_og',img2(1:2,:)')';
I1_lines_norm = draw_epipolar_lines(I1_pts,epi_lines_img1_norm);
imwrite(I1_lines_norm,'hw05_f_img1_norm_epilines.jpg');

