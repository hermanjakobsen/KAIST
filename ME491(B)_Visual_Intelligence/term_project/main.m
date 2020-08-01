clear; clear variables;

%% 1. Camera calibration
% Assumed that cameras have same the intrinsic parameters
% Assumed that the stereo images are rectified. 
disp('Calibrating camera...');
[K_left,R_left,t_left,Co_left,K_right,R_right,t_right,Co_right] = camera_calibration();

% Since the cameras should have the same intrinsic parameters, we should have 
% K_left == K_right. Solving this by defining K as K_left.
K = K_left;

% Since the stereo images are rectified we should have R_left == R_right.
% Solving this by defining R as R_left.
R = R_left;

%[relative_orientation,relative_location] = relative_pose(K,R,t_left);
% Rotation matrix between cameras. Should be identity matrix.
relative_orient = R'*R;
disp('Rotation matrix between cameras is:');
disp(relative_orient);

% O'O-vector. Translation between camera centers. Should only be
% translation in x-direction.
baseline = R*(Co_right(1:3)-Co_left(1:3));
disp('Translation between cameras (from left to right camera with coordinate center being left camera) in camera coordinates is:');
disp(baseline);

camera_params_left = cameraParameters('IntrinsicMatrix',K','RotationVectors',rotationMatrixToVector(R), ...
    'TranslationVectors',t_left');
camera_params_right = cameraParameters('IntrinsicMatrix',K','RotationVectors',rotationMatrixToVector(R), ...
    'TranslationVectors',t_right');
stereo_params = stereoParameters(camera_params_left,camera_params_right, ...
     relative_orient,baseline);
 

%% 2. Extract feature points from images
fldr_left = fullfile('data','left_img_sample','validation');
fldr_right = fullfile('data','right_img_sample','validation');
fldr_dist_left = fullfile('data','left_dist_sample','validation');

disp('Detecting Harris features from image sequences...');
corners_left = find_corners(fldr_left);
corners_right = find_corners(fldr_right);

disp('Extracting features from image sequences...');
[ext_feat_left,valid_pts_left] = extract_features(fldr_left,corners_left);
[ext_feat_right,valid_pts_right] = extract_features(fldr_right,corners_right);


%% 3. Stereo matching and 3D coordinate computation
disp('Performing stereo matching...');
[matched_pts_left, matched_pts_right] = stereo_matching(ext_feat_left,ext_feat_right, ...
    valid_pts_left,valid_pts_right);

disp('Computing 3D coordinates for matched feature points...');
world_pts = calculate_3D_pts(matched_pts_left,matched_pts_right,stereo_params);

disp('Extracting true distance for matched points in left image sequence...');
true_dist_left = get_distance(matched_pts_left,fldr_dist_left);


%% 4. Feature tracking
disp('Tracking feature points for left image sequence...');
tracked_pts = feature_tracking(fldr_left,fullfile('tracking','left_img_sample','test'),corners_left);
vid_left_track = create_video('track_left_val_more_pts.avi','videos',fullfile('tracking','left_img_sample','test'));
%view_video(fullfile('videos','track_left.avi'));

disp('Estimating camera motion...');
T_all = camera_motion(fldr_left,fldr_right,stereo_params,tracked_pts,corners_right);

disp('Writing results to file...');
write_trans_to_txt_file(T_all,'motion_estimation.txt');
