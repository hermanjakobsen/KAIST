clc; clear; close all;

%% Plot GA output %%

%% De Jong 1
plot_GA_output('GA_output/D1/galog_select.txt', 'De Jong 1', 'select');
plot_GA_output('GA_output/D1/galog_ranking.txt', 'De Jong 1', 'ranking');
plot_GA_output('GA_output/D1/galog_tournament.txt', 'De Jong 1', 'tournament');


%% De Jong 2
plot_GA_output('GA_output/D2/galog_select.txt', 'De Jong 2', 'select');
plot_GA_output('GA_output/D2/galog_ranking.txt', 'De Jong 2', 'ranking');
plot_GA_output('GA_output/D2/galog_tournament.txt', 'De Jong 2', 'tournament');


%% De Jong 3
plot_GA_output('GA_output/D3/galog_select.txt', 'De Jong 3', 'select');
plot_GA_output('GA_output/D3/galog_ranking.txt', 'De Jong 3', 'ranking');
plot_GA_output('GA_output/D3/galog_tournament.txt', 'De Jong 3', 'tournament');


%% De Jong 4
plot_GA_output('GA_output/D4/galog_select.txt', 'De Jong 4', 'select');
plot_GA_output('GA_output/D4/galog_ranking.txt', 'De Jong 4', 'ranking');
plot_GA_output('GA_output/D4/galog_tournament.txt', 'De Jong 4', 'tournament');


%% De Jong 5
plot_GA_output('GA_output/D5/galog_select.txt', 'De Jong 5', 'select');
plot_GA_output('GA_output/D5/galog_ranking.txt', 'De Jong 5', 'ranking');
plot_GA_output('GA_output/D5/galog_tournament.txt', 'De Jong 5', 'tournament');


%% Functions
function plot_GA_output(fileName, functionName, methodName)
    data = fopen(fileName);
    gaData = textscan(data, '%d%f%f%f', 'Delimiter', ';', 'headerlines', 1);
    fclose(data);

    % Variables to plot
    gen = zeros(size(gaData{1},1),1);
    bestValue = zeros(size(gaData{1},1),1);
    avgFit = zeros(size(gaData{1},1),1);
    std = zeros(size(gaData{1},1),1);

    % Extract variables from .txt file
    for i= 1:size(gaData{1,1})
        gen(i) = gaData{1,1}(i);
        bestValue(i) = gaData{1,2}(i);
        avgFit(i) = gaData{1,3}(i);
        std(i) = gaData{1,4}(i);
    end

    % Plot
    figure;
    hold on;
    plot(gen, bestValue);
    plot(gen, avgFit, 'color',[0,0,0]+0.5);
    plot(gen, std, 'color',[1,0.5,0]);
    legend('Best value', 'Average Fitness', 'Standard deviation');
    title(['Genetic Algorithm output with ' methodName '-method' ' for ' functionName]);
    xlabel('Generation');
end

