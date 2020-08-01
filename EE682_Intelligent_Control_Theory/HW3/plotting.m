clc; clear; close all;
%% Output data from controllers
fidData = fopen('output.txt');
tData = textscan(fidData, '%f%f', 'Delimiter', ';', 'headerlines', 1);
fclose(fidData);

T1Out = zeros(500,1);
T2Out = zeros(500,1);

for i= 1:size(tData{1},1)
    T1Out(i) = tData{1,1}(i);
    T2Out(i) = tData{1,2}(i);
end

% Plot
figure(1);
t = 1:500;
plot(t, T1Out);
hold on;
plot(t, T2Out);
legend('Type-1', 'Type-2');
title('Step response of FLC');
xlabel('Time step [$s$]', 'Interpreter', 'latex');
ylabel('Output', 'Interpreter', 'latex');
grid on;
hold off;

uiwait(helpdlg('Examine the figures, then click OK to finish.'));