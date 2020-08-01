clear variables;
%% Corresponding image points
% Origin (0,0) is top left corner.

% Image points for homography 1
img_h1_1 = [
    1695    1960    1;
    2213    2024    1;
    2195    2377    1;
    1681    2325    1;
    1644    3223    1;
    1584    3198    1;
    1635    3472    1;
    1576    3433    1;
    2769    2781    1;
    2788    3235    1;
    ]';

img_h1_2 = [
    547     649     1;
    1127    695     1;
    1146    1085    1;
    597     1078    1;
    697     1972    1;
    635     1960    1;
    722     2190    1;
    660     2167    1;
    1702    1427    1;
    1728    1820    1;
    ]';

% Image points for homography 2
img_h2_1 = [
    475     783     1;
    930     1024    1;
    833     1215    1;
    376     999     1;
    1080    2774    1;
    1586    2833    1;
    1444    3046    1;
    929     3009    1;
    342     2200    1;
    33      1350    1;
    ]';

img_h2_2 = [
    696     1971    1;
    1152    1912    1;
    1170    2113    1;
    722     2190    1;
    2265    3324    1;
    2750    3089    1;
    2759    3374    1;
    2277    3632    1;
    1331    3209    1;
    625     2651    1;
    ]';


%% Estimate homography matrix (DLT algorithm)
% Normalization is performed in order to make DLT algorithm invariant to
% scale and coordinate origin. 
[norm_img_h1_1, T_img_h1_1] = data_normalization(img_h1_1);
[norm_img_h1_2, T_img_h1_2] = data_normalization(img_h1_2);
[norm_img_h2_1, T_img_h2_1] = data_normalization(img_h2_1);
[norm_img_h2_2, T_img_h2_2] = data_normalization(img_h2_2);

A1 = construct_A_matrix(norm_img_h1_1, norm_img_h1_2);
A2 = construct_A_matrix(norm_img_h2_1, norm_img_h2_2);

[~,~,V1] = svd(A1);
[~,~,V2] = svd(A2);

norm_h1 = V1(:,9);   % Vector containing homography matrix elements
norm_h2 = V2(:,9);

norm_H1 = reshape(norm_h1,3,3)';   % Homography matrix with normalized elements
norm_H2 = reshape(norm_h2,3,3)';

H1 = T_img_h1_2\norm_H1*T_img_h1_1; % Backward transform to get original data
H2 = T_img_h2_2\norm_H2*T_img_h2_1;


%% Checking result
% Can compare pers1 to img_h1_2 and pers2 to img_h2_2.
pers1 = H1*img_h1_1;
for i = 1:size(pers1')
   pers1(:,i) = pers1(:,i)/pers1(3,i); 
end

pers2 = H2*img_h2_1;
for i = 1:size(pers2')
   pers2(:,i) = pers2(:,i)/pers2(3,i); 
end

% Calculates sum(sqrt((truth_xi-align_xi)^2 +(truth_yi-align_yi)^2))
error1 = error_measurement(H1, img_h1_1, img_h1_2);
error2 = error_measurement(H2, img_h2_1, img_h2_2);


%% Warping
% Homography 1
img1 = imread('hw04_h1_img1.jpg');
tform1 = projective2d(H1');
warped1 = imwarp(img1, tform1);
imshow(warped1);

% Homography 2
img2 = imread('hw04_h2_img1.jpg');
tform2 = projective2d(H2');
warped2 = imwarp(img2, tform2);
imshow(warped2);
