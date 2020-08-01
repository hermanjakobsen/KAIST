function A = construct_A_matrix(img, pers_img)
    [~,number_points] = size(img);
    A = zeros(2*number_points, 9);
    for i = 1:number_points
        % First equation for each pair of points
        A((i-1)*2+1,:) = [
            0, 0, 0, ...
            (-pers_img(3,i)*img(:,i))', ...
            (pers_img(2,i)*img(:,i))'
        ];
        % Second equation for each pair of points
        A((i-1)*2+2,:) = [
            (pers_img(3,i)*img(:,i))', ...
            0, 0, 0, ...
            (-pers_img(1,i)*img(:,i))'
        ];
    end
end