function A = constraint_matrix(img_pts, world_pts)
    [~,npts] = size(img_pts);
    A = zeros(2*npts, 12);
    for i = 1:npts
        % First equation for each point
        A((i-1)*2+1,:) = [
            0, 0, 0, 0, ...
            (-img_pts(3,i)*world_pts(:,i))', ...
            (img_pts(2,i)*world_pts(:,i))'
        ];
        % Second equation for each point
        A((i-1)*2+2,:) = [
            (img_pts(3,i)*world_pts(:,i))', ...
            0, 0, 0, 0, ...
            (-img_pts(1,i)*world_pts(:,i))'
        ];
    end
end