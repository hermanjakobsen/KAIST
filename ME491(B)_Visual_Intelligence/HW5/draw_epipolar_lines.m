function RGB = draw_epipolar_lines(I,lines)
    % Tried to use toBorderLinePoints instead of this function, but
    % something caused weird points to be calculated.
    [~,nlines] = size(lines);
    RGB = I;
    [x_size,~,~] = size(I);
    for i = 1:nlines
        a = lines(1,i);
        b = lines(2,i);
        c = lines(3,i);

        x1 = 0;
        x2 = x_size;
        y1 = -(a*x1+c)/b;
        y2 = -(a*x2+c)/b;

        RGB = insertShape(RGB,'Line',[x1,y1,x2,y2],'Color','blue','Linewidth',4); 
    end
end