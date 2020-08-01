clc; clear; close all;

%% Data preparation

h = 3e-3;   % Sampling interval (s)
Fs = 1/h;   % Sampling frequency (Hz)
N = 2048;   % Number of data
Fn = Fs/2;  % Nuquist cut-off frequency


[x,y,in,out] = data_preparation('data/siso1.in','data/siso1.out',N);


%% 2a

% Obtain Frequency Response Functions
[H1,H2,f] = frequency_response(x,y,Fs);

% Plot
figure;
semilogy(f,abs(H1));
title('Frequency Response Function $H_1(f)$', 'Interpreter', 'Latex');
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
xlim([0 180]);
ylabel('$|H_1(f)|$', 'Interpreter', 'Latex');

figure;
semilogy(f, abs(H2));
title('Frequency Response Function $H_2(f)$', 'Interpreter', 'Latex');
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
xlim([0 180]);
ylabel('$|H_2(f)|$', 'Interpreter', 'Latex');


%% 2b

% Obtain values used for zoom transform
f1 = 40;                % Left-side frequency (Hz)
f2 = 60;                % Right-side frequency (Hz)
B = f2-f1;              % Bandwidth of interest (Hz)
Fc = (f1+f2)/2;         % Center frequency of bandwidth of interest (Hz)
d = floor(Fs/B);        % Decimation factor
hz = d*h;               % New sampling interval (s)
Fsz = 1/hz;             % New sampling frequency (Hz)

zfft = dsp.ZoomFFT(d,Fc,Fs);

xz = zfft(x);
yz = zfft(y);

[H1z, H2z, fz] = frequency_response(xz,yz,Fsz);

figure;
semilogy(fz+f1,abs(H1z));
title('Zoomed Frequency Response Function $H_1(f)$', 'Interpreter', 'Latex');
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$|H_1(f)|$', 'Interpreter', 'Latex');

figure;
semilogy(fz+f1, abs(H2z));
title('Zoomed Frequency Response Function $H_2(f)$', 'Interpreter', 'Latex');
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$|H_2(f)|$', 'Interpreter', 'Latex');


figure;
polarplot(angle(H1z),abs(H1z));
title('Zoomed Frequency Response Function $H_1(f)$', 'Interpreter', 'Latex');

figure;
polarplot(angle(H2z),abs(H2z));
title('Zoomed Frequency Response Function $H_2(f)$', 'Interpreter', 'Latex');


%% Functions 
function [H1,H2,f] = frequency_response(x,y,Fs)
    % Obtain cross power spectral densities
    [Sxx, f] = cpsd(x,x,[],[],[],Fs);
    [Syy, ~] = cpsd(y,y,[],[],[],Fs);
    [Sxy, ~] = cpsd(x,y,[],[],[],Fs);
    [Syx, ~] = cpsd(y,x,[],[],[],Fs);

    % Obtain Frequency Response Functions
    H1 = Sxy./Sxx;
    H2 = Syy./Syx;
end

