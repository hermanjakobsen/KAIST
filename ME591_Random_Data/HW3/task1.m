clc; clear; close all
%% 1a
N = 3600; % Number of data samples
xDelta = 1; % Class interval

%% 1a - For m = 1
m1 = 1; % Number of dice
sampleData1 = gatherSampleData(N, m1);
figure(1);
subplot(2,1,1);
h1 = histogram(sampleData1);
title('Histogram');
xlabel('x, sum of face values');
ylabel('Frequency count');

% Estimate pdf
[pdfValues1, edges1] = histcounts(sampleData1, 'Normalization', 'pdf');
ycoords = pdfValues1;
edgecoords = edges1;
xcoords = (edgecoords(1:end-1)+edgecoords(2:end))./2;
subplot(2,1,2);
plot(xcoords, ycoords, 'r');
title('PDF estimate');
xlabel('x, sum of face values');
ylabel('Density of probability');

sgtitle('Case 1, $m=1$', 'interpreter', 'latex');

%% 1a - For m = 5
m5 = 5; % Number of dice
sampleData5 = gatherSampleData(N, m5);
figure(2);
sgtitle('Case 2, $m=5$', 'interpreter', 'latex');
subplot(2,1,1);
h5 = histogram(sampleData5);
title('Histogram');
xlabel('x, sum of face values');
ylabel('Frequency count');

% Estimate pdf
[pdfValues5, edges5] = histcounts(sampleData5, 'Normalization', 'pdf');
ycoords = pdfValues5;
edgecoords = edges5;
xcoords = (edgecoords(1:end-1)+edgecoords(2:end))./2;
subplot(2,1,2);
plot(xcoords, ycoords, 'r');
title('PDF estimate');
xlabel('x, sum of face values');
ylabel('Density of probability');

%% 1a - For m = 15
m15 = 15; % Number of dice
sampleData15 = gatherSampleData(N, m15);
figure(3);
sgtitle('Case 3, $m=15$', 'interpreter', 'latex');
subplot(2,1,1);
h15 = histogram(sampleData15);
title('Histogram');
xlabel('x, sum of face values');
ylabel('Frequency count');

% Estimate pdf
[pdfValues15, edges15] = histcounts(sampleData15, 'Normalization', 'pdf');
ycoords = pdfValues15;
edgecoords = edges15;
xcoords = (edgecoords(1:end-1)+edgecoords(2:end))./2;
subplot(2,1,2);
plot(xcoords, ycoords, 'r');
title('PDF estimate');
xlabel('x, sum of face values');
ylabel('Density of probability');


%% 1b
Nb = 36; % Number of data samples used in task b

% Make new sample data
sampleData1b = gatherSampleData(Nb, m1);
sampleData15b = gatherSampleData(Nb, m15);

% Get frequency counts for histogram without making a plot
[h1bValues, h1bBinEdges] = histcounts(sampleData1b);
h1Values = h1.Values;
[h15bValues, h15bBinEdges] = histcounts(sampleData15b);
h15Values = h15.Values;

% Calculate approximate random error
randomError1 = calcApproxRandomError(N, xDelta, m1, h1.BinEdges);
randomError1b = calcApproxRandomError(Nb, xDelta, m1, h1bBinEdges);
randomError15 = calcApproxRandomError(N, xDelta, m15, h15.BinEdges);
randomError15b = calcApproxRandomError(Nb, xDelta, m15, h15bBinEdges);

% Calculate normalized approximate bias error
biasError1 = calcNormalizedBiasError(xDelta, m1, h1.BinEdges);
biasError1b = calcNormalizedBiasError(xDelta, m1, h1bBinEdges);
biasError15 = calcNormalizedBiasError(xDelta, m15, h15.BinEdges);
biasError15b = calcNormalizedBiasError(xDelta, m15, h15bBinEdges);

% Plot for case m=1
figure(4);
subplot(2,1,1);
plot(h1.BinEdges, randomError1);
hold on;
plot(h1bBinEdges, randomError1b);
hold off;
legend('$N = 3600$', '$N = 36$', 'interpreter', 'latex');
xlabel('x, sum of face values');
ylabel('Approximated random error');

subplot(2,1,2);
plot(h1.BinEdges, biasError1);
hold on;
plot(h1bBinEdges, biasError1b, '--');
hold off;
legend('$N = 3600$', '$N = 36$', 'interpreter', 'latex');
xlabel('x, sum of face values');
ylabel('Approximated bias error');

sgtitle('Estimation errors, $m=1$', 'interpreter', 'latex');

% Plot for case m=15
figure(5);
subplot(2,1,1);
plot(h15.BinEdges, randomError15);
hold on;
plot(h15bBinEdges, randomError15b);
hold off;
legend('$N = 3600$', '$N = 36$', 'interpreter', 'latex');
xlabel('x, sum of face values');
ylabel('Approximated random error');

subplot(2,1,2);
plot(h15.BinEdges, biasError15);
hold on;
plot(h15bBinEdges, biasError15b);
hold off;
legend('$N = 3600$', '$N = 36$', 'interpreter', 'latex');
xlabel('x, sum of face values');
ylabel('Approximated bias error');

sgtitle('Estimation erros, $m=15$', 'interpreter', 'latex');




