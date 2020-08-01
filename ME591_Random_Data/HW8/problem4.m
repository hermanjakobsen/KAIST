clc;clear; close all;

%% Data preparation

N = 1024;   % Number of data points
h = 2e-3;   % Sampling interval (s)
fs = 1/h;   % Sampling frequency (Hz)
nd = 20;    % Ensemble average
T = readtable('Lathe.dat'); % Table with data

x1 = T.(2)(1:N);    % Input
x2 = T.(4)(1:N);    % Input
y = T.(3)(1:N);     % Output
z = T.(5)(1:N);     % Output

input = [x1 x2];
output = [y z];


%% 4a

% Obtain Power Spectral Densities
[G11,fa] = cpsd(x1,x1,nd,[],[],fs);
[G22,~] = cpsd(x2,x2,nd,[],[],fs);
[Gyy,~] = cpsd(y,y,nd,[],[],fs);

% Plot
figure;
plot(fa,G11);
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$G_{11}(f)$', 'Interpreter', 'Latex');
title('Power Spectral Density, $G_{11}$', 'Interpreter', 'Latex');

figure;
plot(fa,G22);
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$G_{22}(f)$', 'Interpreter', 'Latex');
title('Power Spectral Density, $G_{22}$', 'Interpreter', 'Latex');

figure;
plot(fa,Gyy);
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$G_{yy}(f)$', 'Interpreter', 'Latex');
title('Power Spectral Density, $G_{yy}$', 'Interpreter', 'Latex');


%% 4b

% Obtain ordinary coherence functions 
[C1y,fb] = mscohere(x1,y,nd,[],[],fs);
[C2y,~] = mscohere(x2,y,nd,[],[],fs);
[C12,~] = mscohere(x1,x2,nd,[],[],fs);

% Plot
figure;
plot(fb,C1y);
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$\gamma_{1y}^2(f)$', 'Interpreter', 'Latex');
title('Ordinary coherence function, $$\gamma_{1y}^2$', 'Interpreter', 'Latex');

figure;
plot(fb,C2y);
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$\gamma_{2y}^2(f)$', 'Interpreter', 'Latex');
title('Ordinary coherence function, $\gamma_{2y}^2$', 'Interpreter', 'Latex');

figure;
plot(fb,C12);
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$\gamma_{12}^2(f)$', 'Interpreter', 'Latex');
title('Ordinary coherence function, $\gamma_{12}^2$', 'Interpreter', 'Latex');


%% 4d

% Obtain cross-power spectral densities
[G1y,fd] = cpsd(x1,y,nd,[],[],fs);
[G12,~] = cpsd(x1,x2,nd,[],[],fs);
[G1z,~] = cpsd(x1,z,nd,[],[],fs);
[G2y,~] = cpsd(x2,y,nd,[],[],fs);
[G21,~] = cpsd(x2,x1,nd,[],[],fs);
[G2z,~] = cpsd(x2,z,nd,[],[],fs);

% Obtain constant-parameter linear system
L1y = G1y./G11;
L12 = G12./G11;
L1z = G1z./G11;

% Obtain FRF estimate
H2y = G2y.*(1-(G21.*G1y)./(G11.*G2y))./(G22.*(1-C12));
L2y = H2y;
H1y = L1y-L12.*L2y;
H2z = G2z.*(1-(G21.*G1z)./(G11.*G2z))./(G22.*(1-C12));
L2z = H2z;
H1z = L1z-L12.*L2z;

% Plot
figure;
plot(fd,abs(H1y));
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$|H_{1y}(f)|$', 'Interpreter', 'Latex');
title('Frequency Response Function estimate, $H_{1y}$', 'Interpreter', 'Latex');

figure;
plot(fd,abs(H2y));
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$|H_{2y}(f)|$', 'Interpreter', 'Latex');
title('Frequency Response Function estimate, $H_{2y}$', 'Interpreter', 'Latex');

figure;
plot(fd,abs(H1z));
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$|H_{1z}(f)|$', 'Interpreter', 'Latex');
title('Frequency Response Function estimate, $H_{1z}$', 'Interpreter', 'Latex');

figure;
plot(fd,abs(H2z));
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$|H_{2z}(f)|$', 'Interpreter', 'Latex');
title('Frequency Response Function estimate, $H_{2z}$', 'Interpreter', 'Latex');


%% 4e

% Plot 
figure;
plot(fd,abs(L1y));
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$|L_{1y}(f)|$', 'Interpreter', 'Latex');
title('Constant-parameter linear system estimate, $L_{1y}$', 'Interpreter', 'Latex');

figure;
plot(fd,abs(L12));
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$|L_{12}(f)|$', 'Interpreter', 'Latex');
title('Constant-parameter linear system estimate, $L_{12}$', 'Interpreter', 'Latex');


%% 4f

% Obtain necessary spectral density functions
[G2y,~] = cpsd(x2,y,nd,[],[],fs);
[G21,~] = cpsd(x2,x1,nd,[],[],fs);

% Obtain conditioned spectral density functions
G22_1 = (1-C12).*G22;
G2y_1 = G2y-(G21./G11).*G1y; % Complex, different from other
Gyy_1 = (1-C1y).*Gyy;

% Plot
figure;
plot(fa,G22_1);
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$G_{22\cdot1}(f)$', 'Interpreter', 'Latex');
title('Conditioned Spectral Density, $G_{22\cdot1}(f)$', 'Interpreter', 'Latex');

figure;
plot(fa,G2y_1);
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$G_{2y\cdot1}(f)$', 'Interpreter', 'Latex');
title('Conditioned Spectral Density, $G_{2y\cdot1}(f)$', 'Interpreter', 'Latex');

figure;
plot(fa,Gyy_1);
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$G_{yy\cdot1}(f)$', 'Interpreter', 'Latex');
title('Conditioned Spectral Density, $G_{yy\cdot1}(f)$', 'Interpreter', 'Latex');


%% 4g

% Obtain necessary coherence function
[C21,fg] = mscohere(x2,x1,nd,[],[],fs);

% Obtain necessary conditioned spectral densities
G1y_2 = G1y-(G12./G22).*G2y;
G11_2 = (1-C21).*G11;

% Obtain partial coherence functions
C2y_1 = abs(G2y_1).^2./(G22_1.*Gyy);
C1y_2 = abs(G1y_2).^2./(G11_2.*Gyy);

% Plot
figure;
plot(fg,C2y_1);
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$\gamma_{2y\cdot1}^2(f)$', 'Interpreter', 'Latex');
title('Partial coherence function, $\gamma_{2y\cdot1}^2$', 'Interpreter', 'Latex');

figure;
plot(fg,C1y_2);
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$\gamma_{1y\cdot2}^2(f)$', 'Interpreter', 'Latex');
title('Partial coherence function, $\gamma_{1y\cdot2}^2$', 'Interpreter', 'Latex');


%% 4c

MCOH = C1y + C2y_1;

% Plot
figure;
plot(fg,MCOH);
xlabel('Frequency (Hz)', 'Interpreter', 'Latex');
ylabel('$\gamma_{y:x}^2(f)$', 'Interpreter', 'Latex');
title('Multiple coherence function, $\gamma_{y:x}^2$', 'Interpreter', 'Latex');
