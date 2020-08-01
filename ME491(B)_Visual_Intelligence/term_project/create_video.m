function video_path = create_video(name,save_dest_fldr,img_seq_fldr)
    if ~exist(save_dest_fldr, 'dir')
       mkdir(save_dest_fldr);
    end
    video_path = fullfile(save_dest_fldr,name);
    video = VideoWriter(video_path);
    open(video);
    img_paths = get_paths(img_seq_fldr);
    for i = 1:numel(img_paths)
        I = imread(img_paths{i});
        writeVideo(video,I);
    end
    close(video);
end