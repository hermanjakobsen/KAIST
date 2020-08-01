clc, clear, close all;
%% Data preparation
N = 256; % Samples
h = 0.1; % Sampling interval
T = N*h; % Period

xn = zeros(1,N); % Data sequence
arrayIndex = 1;

% Create/gather datapoints for xn
for t=0:h:T-h 
    if (t < 1/3*T-h)
        xn(arrayIndex) = 1;
    else
        xn(arrayIndex) = -1;
    end
    arrayIndex = arrayIndex + 1;
end


%% Task 1a)
Xpure = fft(xn, N); % Compute DFT of x
% Rearrange output from fft
X = zeros(1,length(xn));
X(1:128) = Xpure(129:256);
X(129:256) = Xpure(1:128);

mag = abs(X); % Magnitude

fs = 1/h; % Samples per second
binVals = [-N/2 : N/2-1];
faxHz = binVals*fs/N; % Frequency x-axis

figure(1);
plot(faxHz, mag);
hold on;
xlabel('Frequency [$Hz$]', 'interpreter', 'latex');
ylabel('$|(X(f_k)|$', 'interpreter', 'latex');
hold off;

figure(2);
plot(faxHz, real(X));
hold on;
xlabel('Frequency [Hz]', 'interpreter', 'latex');
ylabel('$Re[X(f_k)]$', 'interpreter', 'latex');
hold off;

figure(3);
plot(faxHz, imag(X));
hold on;
xlabel('Frequency [$Hz$]', 'interpreter', 'latex');
ylabel('$Im[X(f_k)]$', 'interpreter', 'latex');
hold off;


%% Task 1b)
figure(4);
plot(faxHz, real(X));
hold on;
plot(faxHz, imag(X));
legend('$Re[X(f_k)]$', '$Im[X(f_k)]$', 'interpreter', 'latex');
xlabel('Frequency [$Hz$]', 'interpreter', 'latex');
hold off;


%% Task 1c ???)
Nc = 256 + N; % Samples

xnc = zeros(1,Nc); % Data sequence
arrayIndex = 1;

% Create/gather datapoints for xnc
for period=1:2
    for t=0:h:T-h
        if (t < 1/3*T-h)
            xnc(arrayIndex) = 1;
        else
            xnc(arrayIndex) = -1;
        end
        arrayIndex = arrayIndex + 1;
        
    end
end

figure(5);
XcPure = fft(xnc);
% Rearrange output from fft
Xc = zeros(1, length(xnc));
Xc(1:256) = XcPure(257:512);
Xc(257:512) = XcPure(1:256);

binVals = [-length(Xc)/2 : length(Xc)/2-1];
faxHz = binVals*fs/length(Xc);

Xmink = zeros(1, length(Xc));
Xmink(128:256) = Xc(128:256);
Xnmink = zeros(1, length(Xc));
Xnmink(128:256) = Xc(384:512);

binVals = [-length(Xc)/2 : length(Xc)/2-1];
faxHz = binVals*fs/length(Xc);

plot(faxHz, Xmink);
hold on;
plot(faxHz, Xnmink);
hold off;


%% Task 1d)
figure(6);
Xhat = zeros(1,256);
Xhat(1:8) = Xpure(1:8);
Xhat(248:256) = Xpure(248:256);
binVals = [0 : length(Xhat)-1];
xhat = ifft(Xhat);
plot(binVals, xhat);
hold on;
plot(binVals, xn);
xlabel('Timestep $h$', 'interpreter', 'latex');
legend('$\hat{x}_n$', '$x_n$', 'interpreter', 'latex');



