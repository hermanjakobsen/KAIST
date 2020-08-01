function sampleData = gatherSampleData(numberOfSamples, numberOfDice)
    sampleData = zeros(1, numberOfSamples);
    for sample = 1:numberOfSamples
        [~, sumOfDice] = simulateDiceToss(numberOfDice);
        sampleData(sample) = sumOfDice;
    end
end
