function approxRandomError = calcApproxRandomError(numberOfSamples, classInterval, m, binEdges)
    approxRandomError = zeros(1, length(binEdges));
    
    % Calculate mean and variance for a single dice
    probability = 1/6; % uniform distribution
    var1 = 35/12.0; % Variance for toss of one dice
    mu1 = 3.5; % Expected value for toss of one dice

    for i = 1:length(approxRandomError)-1
        if m ~= 1
            x = binEdges(i) + 0.5;
            mu = m * mu1; % Independent variables
            sigma = sqrt(m*var1); % Independent variables
            probability = normpdf(x, mu, sigma); % Central limit theorem -> normal distribution
        end
        approxRandomError(i) = 1/sqrt(numberOfSamples*classInterval*probability);
    end
    approxRandomError(end) = NaN; % Do no want to plot the last value
end
