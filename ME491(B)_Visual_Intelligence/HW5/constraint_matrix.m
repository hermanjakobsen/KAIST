function A = constraint_matrix(img1,img2)
    % Asuming data (image points) are given as [x; y; w].
    x1 = img1(1,:)';
    y1 = img1(2,:)';
    x2 = img2(1,:)';
    y2 = img2(2,:)';
    [~,npts] = size(img1);
    A = [x2.*x1, x2.*y1, x2, y2.*x1, y2.*y1, y2, x1, y1, ones(npts,1)];
end