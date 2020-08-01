function [relative_orientation,relative_location] = relative_pose(K,R,t_left)

camera_params_left = cameraParameters('IntrinsicMatrix',K','RotationVectors',rotationMatrixToVector(R), ...
        'TranslationVectors',t_left');

pattern_path_left = fullfile('calibrationPattern','pattern_left.jpg');
pattern_path_right = fullfile('calibrationPattern','pattern_right.jpg');

I_left = rgb2gray(imread(pattern_path_left));
I_right = rgb2gray(imread(pattern_path_right));

corners_left = detectHarrisFeatures(I_left);
corners_right = detectHarrisFeatures(I_right);

[feat_left,valid_pts_left] = extractFeatures(I_left,corners_left);
[feat_right,valid_pts_right] = extractFeatures(I_right,corners_right);

idx_pairs = matchFeatures(feat_left, feat_right);
matched_pts_left = valid_pts_left(idx_pairs(:,1),:);
matched_pts_right = valid_pts_right(idx_pairs(:,2),:);

relative_location = [1 1 1];

% Exists several solutions for essential matrix and relative location. Loop
% ensures that we get the one with no translation in z-direction.
while (abs(relative_location(3)) > 0.05)
    [E,inliers_idx] = estimateEssentialMatrix(matched_pts_left,matched_pts_right,camera_params_left);
    
    % Assumes same intrinsic parameters for both cameras
    [relative_orientation,relative_location] = relativeCameraPose(E,camera_params_left,...
        matched_pts_left(inliers_idx),matched_pts_right(inliers_idx));
end