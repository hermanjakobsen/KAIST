clc; clear; close all;

%% Constants
stepValue = 1;
timesteps = 500;

% Constants used for framing and zooming into plots
tUpper = 180;
tLower = 20;
yLowerRect = 0.8;
yUpperRect1 = 1.1; % Used for plausibility measure plot
yUpperRect2 = 1.15; % Used for belief measure plot


% Plausibility measure
% Fuzzy::xi = 0.9;
%	Fuzzy::preference = {1,10,2}; // Preference1 -> flc11.txt
%	Fuzzy::preference = {2,1,10}; // Preference2 -> flc12.txt
%	Fuzzy::preference = {10,2,1}; // Preference3 -> flc13.txt

% Belief measure
% Fuzzy::xi = 0.1;
%	Fuzzy::preference = {1,10,2}; // Preference1 -> flc21.txt
%	Fuzzy::preference = {2,1,10}; // Preference2 -> flc22.txt
%	Fuzzy::preference = {10,2,1}; // Preference3 -> flc23.txt

%% Output data from MFLC controller
% Read data from .txt files
fidData = fopen('flc11.txt');
mflcData11 = textscan(fidData, '%f');
fclose(fidData);

fidData = fopen('flc12.txt');
mflcData12 = textscan(fidData, '%f');
fclose(fidData);

fidData = fopen('flc13.txt');
mflcData13 = textscan(fidData, '%f');
fclose(fidData);

fidData = fopen('flc21.txt');
mflcData21 = textscan(fidData, '%f');
fclose(fidData);

fidData = fopen('flc22.txt');
mflcData22 = textscan(fidData, '%f');
fclose(fidData);

fidData = fopen('flc23.txt');
mflcData23 = textscan(fidData, '%f');
fclose(fidData);
 
% Extract data from data structs
mflcOut11 = zeros(timesteps,1);
mflcOut12 = zeros(timesteps,1);
mflcOut13 = zeros(timesteps,1);
mflcOut21 = zeros(timesteps,1);
mflcOut22 = zeros(timesteps,1);
mflcOut23 = zeros(timesteps,1);

for i= 1:size(mflcData11{1,1})
    mflcOut11(i) = mflcData11{1,1}(i);
    mflcOut12(i) = mflcData12{1,1}(i);
    mflcOut13(i) = mflcData13{1,1}(i);
    mflcOut21(i) = mflcData21{1,1}(i);
    mflcOut22(i) = mflcData22{1,1}(i);
    mflcOut23(i) = mflcData23{1,1}(i);
end


%% Display characteristics 
t = (1:timesteps);
disp('Preferences are given as {Tr:OS:Ts}.');
% For plausibility measure, xi=0.9
characteristics11 = stepinfo(mflcOut11, t, stepValue);
characteristics12 = stepinfo(mflcOut12, t, stepValue);
characteristics13 = stepinfo(mflcOut13, t, stepValue);
disp(' ');
disp('For plausibility measure, xi=0.9:');
disp('Preference1 {1:10:2}:');
disp(characteristics11);
disp('Preference2 {2:1:10}:');
disp(characteristics12);
disp('Preference3 {10:2:1}:');
disp(characteristics13);

% For belief measure, xi=0.1
characteristics21 = stepinfo(mflcOut21, t, stepValue);
characteristics22 = stepinfo(mflcOut22, t, stepValue);
characteristics23 = stepinfo(mflcOut23, t, stepValue);
disp('For belief meausre, xi=0.1:');
disp('Preference1 {1:10:2}:');
disp(characteristics21);
disp('Preference2 {2:1:10}:');
disp(characteristics22);
disp('Preference3 {10:2:1}:');
disp(characteristics23);


%% Plot
figure(1); % Showing response for plausibility measure
hold on;
plot(t, mflcOut11);
plot(t, mflcOut12);
plot(t, mflcOut13);
rectangle('Position',[tLower yLowerRect tUpper-tLower yUpperRect1-yLowerRect], 'EdgeColor','m');
hold off;
title('Step response of MFLC, $\xi=0.9$', 'Interpreter', 'latex');
xlabel('Time step', 'Interpreter', 'latex');
ylabel('Output', 'Interpreter', 'latex');
legend('$T_r:OS:T_s=1:10:2$', '$T_r:OS:T_s=2:1:10$', '$T_r:OS:T_s=10:2:1$', 'Interpreter', 'latex', 'FontSize',12);
grid on;

figure(2); % Zoomed in plot of response for plausibility measure
indexOfInterest = (t < tUpper) & (t > tLower);
hold on;
plot(t(indexOfInterest),mflcOut11(indexOfInterest));
plot(t(indexOfInterest),mflcOut12(indexOfInterest));
plot(t(indexOfInterest),mflcOut13(indexOfInterest));
rectangle('Position',[tLower yLowerRect tUpper-tLower yUpperRect1-yLowerRect], 'EdgeColor','m');
hold off;
axis([tLower tUpper yLowerRect yUpperRect1]);
title('Step response of MFLC, $\xi=0.9$', 'Interpreter', 'latex');
xlabel('Time step', 'Interpreter', 'latex');
ylabel('Output', 'Interpreter', 'latex');
legend('$T_r:OS:T_s=1:10:2$', '$T_r:OS:T_s=2:1:10$', '$T_r:OS:T_s=10:2:1$', 'Interpreter', 'latex','FontSize',12);
grid on;

figure(3); % Showing response for belief measure
hold on;
rectangle('Position',[tLower yLowerRect tUpper-tLower yUpperRect2-yLowerRect], 'EdgeColor','m');
plot(t, mflcOut21);
plot(t, mflcOut22);
plot(t, mflcOut23);
title('Step response of MFLC, $\xi=0.1$', 'Interpreter', 'latex');
xlabel('Time step', 'Interpreter', 'latex');
ylabel('Output', 'Interpreter', 'latex');
legend('$T_r:OS:T_s=1:10:2$', '$T_r:OS:T_s=2:1:10$', '$T_r:OS:T_s=10:2:1$', 'Interpreter', 'latex','FontSize',12);
grid on;
hold off;

figure(4); % Zoomed in plot of response for belief
indexOfInterest = (t < tUpper) & (t > tLower);
hold on;
plot(t(indexOfInterest),mflcOut21(indexOfInterest));
plot(t(indexOfInterest),mflcOut22(indexOfInterest));
plot(t(indexOfInterest),mflcOut23(indexOfInterest));
rectangle('Position',[tLower yLowerRect tUpper-tLower yUpperRect2-yLowerRect], 'EdgeColor','m');
hold off;
axis([tLower tUpper yLowerRect yUpperRect2]);
title('Step response of MFLC, $\xi=0.1$', 'Interpreter', 'latex');
xlabel('Time step', 'Interpreter', 'latex');
ylabel('Output', 'Interpreter', 'latex');
legend('$T_r:OS:T_s=1:10:2$', '$T_r:OS:T_s=2:1:10$', '$T_r:OS:T_s=10:2:1$', 'Interpreter', 'latex','FontSize',12);
grid on;

uiwait(helpdlg('Examine the figures, then click OK to finish.'));