function view_video(video_path)
    video = VideoReader(video_path);
    i = 1;
    while hasFrame(video)
        mov(i) = im2frame(readFrame(video));
        i = i+1;
    end
    movie(mov,1,video.FrameRate)
end