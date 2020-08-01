function tracked_pts = feature_tracking(img_fldr,save_dest,corners)
    if ~exist(save_dest, 'dir')
        mkdir(save_dest);
    end
    img_paths = get_paths(img_fldr);
    [filepath,name,~] = fileparts(save_dest);
    tracked_pts = cell(1,numel(img_paths));
    
    object_frame = imread(img_paths{1});
    tracker = vision.PointTracker();
    initialize(tracker,corners{1}.Location,object_frame);

    for i = 1:numel(img_paths)
      frame = imread(img_paths{i});
      [points,validity] = tracker(frame);
      [num_pts,~] = size(points(validity,:));
      tracked_pts{i} = points(validity,:);
      out = insertMarker(frame,points(validity,:),'+');
      write_path = fullfile(filepath,name,strcat('Image','0',string(629+i),'.jpg'));
      imwrite(out,write_path);
      
      % Track new points if the previous tracked points dissappear
      if num_pts < 50   
          setPoints(tracker,corners{i}.Location);
      end
    end
end