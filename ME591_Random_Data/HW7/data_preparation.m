function [x,y,inRaw,outRaw] = data_preparation(inFilename, outFilename, N)
    % Open file
    inID = fopen(inFilename);
    outID = fopen(outFilename);

    % Read whole file
    formatSpec = '%f';
    inRaw = fscanf(inID, formatSpec);
    outRaw = fscanf(outID, formatSpec);
    
    % Extract desired length of data
    x = zeros(N,1);
    y = zeros(N,1);
    for i = 1:N
       x(i) = inRaw(i);
       y(i) = outRaw(i);
    end
end