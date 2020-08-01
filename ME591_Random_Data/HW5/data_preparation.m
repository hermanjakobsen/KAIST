function [data, t] = data_preparation(N, samplingInterval)
    data = zeros(1, N);
    t = zeros(1, N);
    h = samplingInterval;
    for i = 1:N
        t(i) = i*h;
        if ((1 <= i && i <= 32) || (65 <= i && i <= 96) || ...
           (129 <= i && i <= 160) || (193 <= i && i <= 224))
           data(i) = 1;
        else
           data(i) = -1;
        end
       
    end
end