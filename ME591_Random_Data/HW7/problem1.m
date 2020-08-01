clc; clear; close all;

%% Data preparation

h = 3e-3;   % Sampling interval [s]
N = 2048;   % Number of data

[x,y,inRaw,outRaw] = data_preparation('data/siso1.in','data/siso1.out',N);


%% 1a
% Wish to obtain the coherence estimate whith nd=1. This means that only one
% window is applied over t  he whole dataset. 

[Cxy1,F1] = mscohere(x,y,N,[],[],1/h);

figure;
plot(F1,Cxy1);
ylim([0.9 1.1]);
xlim([0 180]);
title('Instantaneous estimation of $\gamma^2_{xy}$', 'Interpreter', 'Latex');
xlabel('Frequency (HZ)', 'Interpreter', 'Latex');


%% 1b

[Cxy2,F2] = mscohere(x,y,[],[],[],1/h); % Maybe choose different parameters?

figure;
plot(F2,Cxy2);
title('Ensembled averaged estimation of $\gamma^2_{xy}$', 'Interpreter', 'Latex');
xlabel('Frequency (HZ)', 'Interpreter', 'Latex');

