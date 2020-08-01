clc; clear; close all;
%% Data preparation

f1 = 12;    % Hz
f2 = 12.5;  % Hz

N = 512;    % Number of data
dt = 1/N;   % Sampling time interval (s)
Fs = 1/dt;  % Sampling frequency (Hz)

x1 = zeros(1,N);
x2 = zeros(1,N);
n = zeros(1,N);

for i = 1:N
    x1(i) = sin(2*pi*f1*dt*(i-1));
    x2(i) = sin(2*pi*f2*dt*(i-1));
    n(i) = (i-1);
end

%% 2a

figure(1);
plot(n,x1);
ylabel('x_{1}(n)');
xlabel('Timestep [n]');
xlim([0 N]);
ylim([-1.5 1.5]);
title('Sampled time record');

figure(2);
plot(n,x2);
ylabel('x_{2}(n)');
xlabel('Timestep [n]');
xlim([0 N]);
ylim([-1.5 1.5]);
title('Sampled time record');


%% 2b
% Will use periodogram function. Uses default rectangular window.
% One-sided PSD estimate is default.
[Gxx1, F1] = periodogram(x1, [], length(x1), Fs);   
[Gxx2, F2] = periodogram(x2, [], length(x2), Fs);

figure(3);
plot(F1,10*log10(Gxx1));
xlabel('Frequency [Hz]');
ylabel('Power/Frequency [db/Hz]');
title('Autopectral Density Estimate, $\hat{G}_{x_1x_1}(f)$', 'Interpreter', 'Latex');

figure(4);
plot(F2,10*log10(Gxx2));
xlabel('Frequency [Hz]');
ylabel('Power/Frequency [db/Hz]');
title('Autospectral Density Estimate, $\hat{G}_{x_2x_2}(f)$', 'Interpreter', 'Latex');


%% 2d
[Gxx1, F1] = periodogram(x1, hann(length(x1)), length(x1), Fs);
[Gxx2, F2] = periodogram(x2, hann(length(x2)), length(x2), Fs);
 
figure(5);
plot(F1,10*log10(Gxx1));
xlabel('Frequency [Hz]');
ylabel('Power/Frequency [db/Hz]');
title('Autopectral Density Estimate with Hanning Window, $\hat{G}_{x_1x_1}(f)$', 'Interpreter', 'Latex');

figure(6);
plot(F2,10*log10(Gxx2));
xlabel('Frequency [Hz]');
ylabel('Power/Frequency [db/Hz]');
title('Autopectral Density Estimate with Hanning Window, $\hat{G}_{x_2x_2}(f)$', 'Interpreter', 'Latex');
