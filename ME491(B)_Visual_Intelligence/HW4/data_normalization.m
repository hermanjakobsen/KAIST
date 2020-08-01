function [norm_data, T_data] = data_normalization(data) % Isotropic scaling
    % Asuming data (image points) are given as [x; y; w].
    [~,number_points] = size(data);
    mean_data = mean(data,2); % Mean of each row (coordinate)
    dist_sum = 0;
    for i = 1:number_points
        dist_sum = dist_sum+norm(data(:,i)-mean_data)^2;
    end
    scale_2 = sqrt(dist_sum/(2*number_points));
    T_data = [
        1/scale_2, 0, -1/scale_2*mean_data(1);
        0, 1/scale_2, -1/scale_2*mean_data(2);
        0, 0, 1
        ];
    norm_data = T_data*data;
end