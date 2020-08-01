function T_all = camera_motion(img_fldr_left,img_fldr_right,stereo_params,...
    tracked_pts,corners_right)
    
    options = optimset('Display','off');
    
    img_paths_left = get_paths(img_fldr_left);
    img_paths_right = get_paths(img_fldr_right);

    T_all = cell(1,numel(img_paths_left));
    identity_proj = [1 0 0 0; 0 1 0 0; 0 0 1 0; 0 0 0 1];
    T_all{1} = identity_proj;
    
    for i = 2:numel(img_paths_left) 
        
        prev_pts_left = tracked_pts{i-1};
        prev_pts_right = corners_right{i-1}.Location;
        
        prev_I_left = rgb2gray(imread(img_paths_left{i-1}));
        prev_I_right = rgb2gray(imread(img_paths_right{i-1}));
        
        [prev_feat_left,prev_valid_pts_left] = extractFeatures(prev_I_left,prev_pts_left);
        [prev_feat_right,prev_valid_pts_right] = extractFeatures(prev_I_right,prev_pts_right);
        
        prev_idx_pairs = matchFeatures(prev_feat_left,prev_feat_right);
        prev_matched_left = prev_valid_pts_left(prev_idx_pairs(:,1),:);
        prev_matched_right = prev_valid_pts_right(prev_idx_pairs(:,2),:);
        prev_pts_3d = triangulate(prev_matched_left,prev_matched_right,stereo_params);
        
        curr_pts_left = tracked_pts{i};
        curr_pts_right = corners_right{i}.Location;
        
        curr_I_left = rgb2gray(imread(img_paths_left{i}));
        curr_I_right = rgb2gray(imread(img_paths_right{i}));
        
        [curr_feat_left,curr_valid_pts_left] = extractFeatures(curr_I_left,curr_pts_left);
        [curr_feat_right,curr_valid_pts_right] = extractFeatures(curr_I_right,curr_pts_right);
        
        
        curr_idx_pairs = matchFeatures(curr_feat_left,curr_feat_right);
        curr_matched_left = curr_valid_pts_left(curr_idx_pairs(:,1),:); 
        curr_matched_right = curr_valid_pts_right(curr_idx_pairs(:,2),:);
        curr_pts_3d = triangulate(curr_matched_left,curr_matched_right,stereo_params);
        
        % Features corresponding to points in left image that have a match
        % in right image.
        prev_feat_left = prev_feat_left(prev_idx_pairs(:,1),:);
        curr_feat_left = curr_feat_left(curr_idx_pairs(:,1),:);
        
        % Match between features in left image sequence. These features
        % also have a match to their corresonding right image.
        seq_idx_pair = matchFeatures(prev_feat_left,curr_feat_left);
        
        % 3D points that match in left image sequence.
        prev_pts_3d = prev_pts_3d(seq_idx_pair(:,1),:);
        curr_pts_3d = curr_pts_3d(seq_idx_pair(:,2),:);
        
        % Convert to homogeneous coordinates.
        prev_pts_3d(:,4) = 1;
        curr_pts_3d(:,4) = 1;
        
        % Solve 3D-to-3D motion estimation minimization problem.
        obj = @(T) objective_function(T,prev_pts_3d,curr_pts_3d);
        T_init = [1 0 0 0; 0 1 0 0; 0 0 1 0];
        T_curr = fminsearch(obj,T_init,options);
        T_curr(4,:) = [0 0 0 1];
        T_all{i} = T_all{i-1}*T_curr;
        
    end  
end