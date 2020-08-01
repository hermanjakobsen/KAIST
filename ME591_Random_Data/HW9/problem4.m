clc; clear; close all;

%% Data preparation

N = 15000;          % Number of data points
Fs = 44100;         % Sampling frequency (Hz)
dt = 1/Fs;          % Sampling time (s)
T = readtable('driver_impactsignal.dat'); % Table with data
x = T.(1)(1:N-1)';  % Data
t = dt*(0:N-2);     % Time vector

figure;
plot(t,x);
xlabel('Time (s)')
title('Driver impact sound data');


%% Spectrogram

figure;
y=chirp(t,2500,0.08,3500,'quadratic'); 
spectrogram(y,kaiser(128,18),120,128,Fs,'yaxis');
title('Quadratic Chirp: start at 2.5kHz and cross 3.5kHz at t=80msec');
