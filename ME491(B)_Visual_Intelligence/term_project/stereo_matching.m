function [matched_pts_left, matched_pts_right] = stereo_matching(ext_feat_left,ext_feat_right,valid_pts_left,valid_pts_right)
    matched_pts_left = cell(1, numel(ext_feat_left));
    matched_pts_right = cell(1, numel(ext_feat_right));
    for i = 1:numel(ext_feat_left)
        idx_pairs = matchFeatures(ext_feat_left{i}, ext_feat_right{i});
        matched_pts_left{i} = valid_pts_left{i}(idx_pairs(:,1),:);
        matched_pts_right{i} = valid_pts_right{i}(idx_pairs(:,2),:);
    end
end