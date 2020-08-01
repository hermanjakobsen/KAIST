%% Output data from controllers
fidData = fopen('output.txt');
tData = textscan(fidData, '%f%f%f%f', 'Delimiter', ';', 'headerlines', 1);
fclose(fidData);

pOut = zeros(500,1);
pidOut = zeros(500,1);
flcOut = zeros(500,1);
flc3Out = zeros(500,1);

for i= 1:size(tData{1},1)
    pOut(i) = tData{1,1}(i);
    pidOut(i) = tData{1,2}(i);
    flcOut(i) = tData{1,3}(i);
    flc3Out(i) = tData{1,4}(i);
end

% Plot
figure(1);
x = 1:500;
%plot(x, pOut);
hold on;
%plot(x, pidOut);
plot(x, flcOut);
%plot(x, flc3Out);
%legend('PID', 'FLC');
title('Step response for FLC controller');
xlabel('Time step [$s$]', 'Interpreter', 'latex');
ylabel('Output', 'Interpreter', 'latex');
grid on;
hold off;

%% Membership functions
fidMf = fopen('mf.txt');
tMf = textscan(fidMf, '%s%f%f%f', 'Delimiter', ';');
fclose(fidMf);

NB = zeros(3,1);
NM = zeros(3,1);
NS = zeros(3,1);
ZO = zeros(3,1);
PS = zeros(3,1);
PM = zeros(3,1);
PB = zeros(3,1);

for i= 1:size(tMf,2)-1
   NB(i) = tMf{i+1}(1);
   NM(i) = tMf{i+1}(2);
   NS(i) = tMf{i+1}(3);
   ZO(i) = tMf{i+1}(4);
   PS(i) = tMf{i+1}(5);
   PM(i) = tMf{i+1}(6);
   PB(i) = tMf{i+1}(7);
end

% Plot
figure(2);
fis = mamfis;
fis = addInput(fis, [-1.5 1.5], 'Name', 'Input');
fis = addMF(fis,'Input','trimf',NB,'Name','NB');
fis = addMF(fis,'Input','trimf',NM,'Name','NM');
fis = addMF(fis,'Input','trimf',NS,'Name','NS');
fis = addMF(fis,'Input','trimf',ZO,'Name','ZO');
fis = addMF(fis,'Input','trimf',PS,'Name','PS');
fis = addMF(fis,'Input','trimf',PM,'Name','PM');
fis = addMF(fis,'Input','trimf',PB,'Name','PB');
plotmf(fis, 'input', 1);


uiwait(helpdlg('Examine the figures, then click OK to finish.'));
