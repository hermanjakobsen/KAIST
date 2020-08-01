clc; clear; close all;
%% 3a
% The true spectrum is obtained by performing an ideal (infinite) Fourier
% transform. This results in two delta functions appearing at +-7/2 Hz. The
% magnitude of the Fourier coefficient at each peak is abs(1/(2*j)) = 1/2.

f0 = 7/2;       % Hz
mag = 1/2;      % |A_k|

figure(1);
plot(-f0, mag, 'r', 'Marker', '*');
hold on;
plot(f0, mag, 'r', 'Marker', '*');
line([-f0 -f0], [0, mag], 'Color', 'r');
line([f0 f0], [0, mag], 'Color', 'r');
xlim([-15 15]);
ylim([0 1]);
title('True Spectrum');
xlabel('Frequency [Hz]');
ylabel('$|A_k)|$', 'Interpreter', 'Latex', 'FontSize', 16);


%% 3b
T = 1;              % Time record length
dt = 1/32;          % Step length
t = 0:dt:T-dt;      % Time vector
y = sin(7*pi*t);    % Signal 

Y = fft(y);     % Compute DFT of x

l = length(Y);
Y = rearrange_fourier_coeff(Y, l);

k = (-l/2:l/2-1); % Frequency indices

figure(2);
hold on;
plot(k, abs(Y), 'r');
xlabel('Frequency [Hz]', 'Interpreter', 'Latex', 'Fontsize', 16);
title('Discrete Finite Fourier Transform');

% Plot spectral lines 
lim = 1/(2*dt)-1;
for k = 1:15    
    xline(k/T);
end
xlim([-lim lim]);
legend('$|Y(f_k,T)|$', '$f_k$', 'Interpreter', 'Latex');


%% 3c
w = sqrt(2/3)*(1-cos((2*pi*t)/T));  % Scaled Hanning window function
x = y.*w;                           % Scaled Hanning windowed signal

X = fft(x);     % Compute DFT of x

l = length(X);
X = rearrange_fourier_coeff(X, l);

k = (-l/2:l/2-1); % Frequency indices

figure(3);
hold on;
plot(k, abs(X), 'r');
xlabel('Frequency [Hz]', 'Interpreter', 'Latex', 'Fontsize', 16);
title('Discrete Finite Fourier Transform');

% Plot spectral lines 
lim = 1/(2*dt)-1;
for k = 1:lim    
    xline(k/T);
end
xlim([-lim lim]);
legend('$|X(f_k,T)|$', '$f_k$', 'Interpreter', 'Latex');