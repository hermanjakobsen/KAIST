function [norm_data, T_data] = data_normalization(data) % Isotropic scaling
    [dim,npts] = size(data);
    if (dim == 3)   % 2D points given as homogeneous coordinates
        mean_data = mean(data,2); % Mean of each row (coordinate)
        dist_sum = 0;
        for i = 1:npts
            dist_sum = dist_sum+norm(data(:,i)-mean_data)^2;
        end
        scale_2 = sqrt(dist_sum/(2*npts));
        T_data = [
            1/scale_2, 0, -1/scale_2*mean_data(1);
            0, 1/scale_2, -1/scale_2*mean_data(2);
            0, 0, 1
            ];
        norm_data = T_data*data;
    else
       disp('Have not implemented data normalization for this point dimension'); 
    end
end