function [world_pts] = calculate_3D_pts(matched_pts_left,matched_pts_right,stereo_params)
    % Origin of world coordinates is at left camera center.

    world_pts = cell(1,numel(matched_pts_left));

    for i = 1:numel(matched_pts_left)
        pts_3d = triangulate(matched_pts_left{i},matched_pts_right{i},stereo_params);
        world_pts{i} = pts_3d;
    end
end