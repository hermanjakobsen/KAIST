function [distance] = get_distance(matched_pts_left,fldr)
    distance = cell(1,numel(matched_pts_left));
    img_paths = get_paths(fldr);
    for i = 1:numel(matched_pts_left)
        dist_img = zeros(1,matched_pts_left{i}.Count);
        for j = 1:matched_pts_left{i}.Count
            I = imread(img_paths{i})/655.35;
            x = fix(matched_pts_left{i}.Location(j,1));
            y = fix(matched_pts_left{i}.Location(j,2));
            dist_img(j) = I(y,x);
        end
        distance{i} = dist_img';
    end
end

