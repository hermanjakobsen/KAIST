clc; clear; close all;
%% Data preparation
N = 1024;           % Number of data
fs = 140;           % Sampling frequenzy (Hz)
dt = 1/fs;          % Sampling time interval (s)
T = N*dt;           % Period
load engine.dat     % Load out-handed data


%% 4a
% The rectangular window is implicitly applied due to the data not being
% infinite long
X = fft(engine);
l = length(X);
X = rearrange_fourier_coeff(X,l);
A = abs(X).^2/T^2;

k = (-l/2:l/2-1); % Frequency indices

figure(1);
plot(k, abs(X));
xlabel('Frequency [Hz]');
ylabel('|X(f)|');
title('Power Spectrum');

%% 4b
wh = hanning(N);
wk = kaiser(N, 2.5);

xh = engine.*wh;
xk = engine.*wk;

Xh = rearrange_fourier_coeff(fft(xh), length(xh));
Xk = rearrange_fourier_coeff(fft(xk), length(xk));

Ah = abs(Xh).^2/T^2;
Ak = abs(Xk).^2/T^2;

figure(2);
plot(k, abs(Xh));
xlabel('Frequency [Hz]');
ylabel('|X(f)|');
title('Power Spectrum with Hanning window');

figure(3);
plot(k, abs(Xk));
xlabel('Frequency [Hz]');
ylabel('|X(f)|');
title('Power Spectrum with Kaiser Bessel window');

