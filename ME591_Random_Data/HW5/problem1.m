clc; clear; close all;

%% Data Preparation
N = 256;
dt = 0.1; % Sampling interval
[x, t] = data_preparation(N, dt);


%% Problem 1 - Digital FIR Filter
n = 20; % Filter order
Fp = 1; % Passband-edge frequency
Fs = 2; % Stopband-edge frequency. Arbitrary chosen, but must satisfy Fs>Fp
df = 1/dt; % Sampling frequency
fc = df/2; % Nyquist cut-off freq.


%% 1a - Filter weights
% Normalizing frequencies w.r.t. Nyquist cut-off freq.
fp = Fp/fc;
fs = Fs/fc;

f = [0 fp fs 1]; % Frequency band edges 
m = [1 1 0 0];  % Desired amplitudes
b = firpm(n,f,m); % Filter weights

figure(1);
xaxis = (-10:1:10);
plot(xaxis,b,'.'); hold on
plot(xaxis,b); grid on;
xlabel('Time Sequence [n]');
ylabel('Filter Weightings');
title('Filter weights for FIR low-pass filter');


%% 1b - Frequency response
[h, w] = freqz(b,[1 zeros(1,20)], 1024);
figure(2);
plot (w/pi*fc, abs(h));
grid on;
xticks(1:1:10);
xlabel('Frequency [Hz]');
ylabel('Amplitude (Linear)');
title('FIR low-pass filter frequency response');


%% 1c - Apply low-pass filter
y = filter2(b,x);
figure(3);
plot(t,x,t,y);
legend('Original input', 'FIR filtered output');
xlabel('Time [s]');
ylabel('Amplitude');
title('Result of FIR low-pass filtering');


%% 1d - Fourier coefficient
Y = fft(y);
T = N*dt; % Period

% Apply relationship between Finite F.T. and Fourier Series
Apure = (1/T)*Y; % Fourier coefficient

% Rearrange Fourier coefficient
l = length(Apure);
A = zeros(1,l);
A(1:l/2) = Apure(l/2+1:l);
A(l/2+1:l) = Apure(1:l/2);

% Obtain frequency indeces
k = (-l/2:l/2-1);

figure(4);
hold on;
plot(k, abs(A));
xlabel('$k$', 'Interpreter', 'Latex', 'Fontsize', 16);
ylabel('$|A_k|$', 'Interpreter', 'Latex', 'Fontsize', 16);
title('Fourier coefficient');
grid on;
