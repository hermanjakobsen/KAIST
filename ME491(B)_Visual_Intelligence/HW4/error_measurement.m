function error = error_measurement(H, img, truth_img)
    % Calculates sum(sqrt((truth_xi-align_xi)^2 +(truth_yi-align_yi)^2))
    error = 0;
    [~,img_size] = size(img);
    for i = 1:img_size
        align_pt = H*img(:,i);
        align_pt = align_pt/align_pt(3);
        truth_pt = truth_img(:,i); 
        diff = sqrt((truth_pt(1)-align_pt(1))^2+(truth_pt(2)-align_pt(2))^2);
        error = error + diff;
    end
end