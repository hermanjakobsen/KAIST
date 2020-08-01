%% Given information
Fs = 2048;      % Sampling frequency (Hz)
T = 1;          % Time record length (s)
df = 1/T;       % Frequency resolution (Hz)


%% 1a
Fc = Fs/2;      % Nyquist cut-off frequency (Hz) 

% The number of spectral lines over the frequency range 0 to the Nyquist
% cut-off frequency is then given by the following formula
numSpectralLines = (Fc-0)/df;


%% 1b
% Have a band-pass filter with pass band frequencies 500~756 Hz.
f1 = 500;   % Hz
f2 = 756;   % Hz

% The band-pass is then given as 
B = 756-500;

% The center frequency of the band-pass is given as
f0 = f1+B/2;

% The modulating frequency is then
fm = f0-B/2;


%% 1d
N = 2048;   % Number of samples

% From the slides the decimation factor is given as 
d = Fc/B;