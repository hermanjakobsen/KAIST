function [norm_data, T_data] = data_normalization(data) % Isotropic scaling
    [dim,npts] = size(data);
    mean_data = mean(data,2); % Mean of each row (coordinate)
    dist_sum = 0;
    for i = 1:npts
            dist_sum = dist_sum+norm(data(:,i)-mean_data)^2;
    end
    if (dim == 3)   % 2D points given as homogeneous coordinates
        scale_2 = sqrt(dist_sum/(2*npts));
        T_data = [
            1/scale_2, 0, -1/scale_2*mean_data(1);
            0, 1/scale_2, -1/scale_2*mean_data(2);
            0, 0, 1
            ];
    elseif (dim == 4) % 3D points given as homogeneous coordinates
        scale_3 = sqrt(dist_sum/(3*npts));
        T_data = [
            1/scale_3, 0, 0, -1/scale_3*mean_data(1);
            0, 1/scale_3, 0, -1/scale_3*mean_data(2);
            0, 0, 1/scale_3, -1/scale_3*data(3);
            0, 0, 0, 1;
            ];   
    else
       error('Have not implemented data normalization for this point dimension'); 
    end
    norm_data = T_data*data;
end