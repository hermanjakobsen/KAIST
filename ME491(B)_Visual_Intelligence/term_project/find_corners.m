function corners = find_corners(fldr)
    img_paths = get_paths(fldr);
    corners = cell(1,numel(img_paths));
    for i = 1:numel(img_paths)
        I =  rgb2gray(imread(img_paths{i}));
        corners_img = detectHarrisFeatures(I);
        corners{i} = corners_img;
    end
end