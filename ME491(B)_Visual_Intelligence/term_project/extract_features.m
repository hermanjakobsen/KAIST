function [ext_feat, valid_pts] = extract_features(fldr,corners)
    img_paths = get_paths(fldr);
    
    ext_feat = cell(1,numel(img_paths));
    valid_pts = cell(1,numel(img_paths));
    
    for i = 1:numel(img_paths)
       I =  rgb2gray(imread(img_paths{i}));
       corners_img = corners{i};
       [features,valid_points] = extractFeatures(I,corners_img);
       
       ext_feat{i} = features;
       valid_pts{i} = valid_points;
    end
end