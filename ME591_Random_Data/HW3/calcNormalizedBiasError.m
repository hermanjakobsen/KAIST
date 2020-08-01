function biasError = calcNormalizedBiasError(classInterval, m, binEdges)
    biasError = zeros(1, length(binEdges));
    
    % Expected value and variance for a single dice
    probability = 1/6;
    ddProbability = 0;
    var1 = 35/12.0; % Variance for toss of one dice
    mu1 = 3.5; % Expected value for toss of one dice
    
    for i = 1:(length(binEdges)-1)
        if m ~=1
            
            mu = m * mu1; % Independent variables
            sigma = sqrt(m*var1); % Independent variables
            x = binEdges(i)+0.5;
            
            probability = normpdf(x, mu, sigma); % Central limit theorem -> normal distribution
            ddProbability = (exp(-(mu - x)^2/(2*sigma^2))*(2*mu - 2*x)^2)/(8*sigma^6*pi) - exp(-(mu - x)^2/(2*sigma^2))/(2*sigma^4*pi);
            
        end
        biasError(i) = (classInterval^2 * ddProbability) / (24*probability);
    end
    biasError(end) = NaN; % Do no want to plot the last value
end