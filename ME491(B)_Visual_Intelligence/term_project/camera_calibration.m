function [K_left,R_left,t_left,Co_left, K_right,R_right,t_right,Co_right] = camera_calibration()
    
[img_pts_left,img_pts_right,world_pts_left,world_pts_right] = get_cal_data_pts();
    
% Normalize data points
[norm_img_pts_left,T_img_left] = data_normalization(img_pts_left);
[norm_img_pts_right,T_img_right] = data_normalization(img_pts_right);
[norm_world_pts_left,T_world_left] = data_normalization(world_pts_left);
[norm_world_pts_right,T_world_right] = data_normalization(world_pts_right);

A_left = constraint_matrix(norm_img_pts_left, norm_world_pts_left);
A_right = constraint_matrix(norm_img_pts_right, norm_world_pts_right);

[K_left,R_left,t_left,Co_left] = camera_parameters(A_left,T_img_left,T_world_left);
[K_right,R_right,t_right,Co_right] = camera_parameters(A_right,T_img_right,T_world_right);

end
