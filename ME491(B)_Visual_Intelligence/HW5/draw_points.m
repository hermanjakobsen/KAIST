function RGB = draw_points(I,img_pts)
    [~,npts] = size(img_pts);
    radius = 20;
    RGB = I;
    for i = 1:npts
        x = img_pts(1,i);
        y = img_pts(2,i);
        RGB = insertShape(RGB,'FilledCircle',[x,y,radius],'Color','red'); 
    end
end