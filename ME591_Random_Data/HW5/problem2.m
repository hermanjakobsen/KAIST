clc; clear; close all;

%% Data Preparation
N = 256;
dt = 0.1; % Sampling interval
[x, t] = data_preparation(N, dt);


%% Problem 2 - Digital IIR Filter
n = 7; % Filter order
Fp = 1; % Passband-edge frequency
df = 1/dt; % Sampling frequency
fc = df/2; % Nyquist cut-off freq.


fp = Fp/fc; % Normalized frequency
[b, a] = butter(n,fp);


%% 2a - Frequency response
[h, w] = freqz(b,a,1024);
figure(2);
plot (w/pi*fc, abs(h)); grid
xlabel('Frequency [Hz]');
ylabel('Amplitude (Linear)');
title('IIR low-pass filter frequency response');


%% 2b - Apply low-pass filter
y = filter(b,a,x);
figure(3);
plot(t,x,t,y);
legend('Original input', 'IIR filtered output');
xlabel('Time [s]');
ylabel('Amplitude');
title('Result of IIR low-pass filtering');


%% 2c - Fourier coefficient
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
