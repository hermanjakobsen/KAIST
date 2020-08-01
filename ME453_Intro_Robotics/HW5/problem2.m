clc; clear; close all;
%% Expressions 
syms r real

f = 1.02*10^(-4)*r^2-40*r+4619.57;
g = 0.014*r^2-48*r+1783.15;
h = 2.76*10^(-3)*r^2-40*r+1043.48;
i = 0.374*r^2-48*r+402.78;


% Found out which curve and solutions represent min and max by looking at  
% the curves graphically
solMin = solve(f==0, r);
solMax = solve(i==0, r);


rmin = double(solMin(1));
rmax = double(solMax(2));


% The expressions should all be less than 0 with the given gear ratios
% Min
fmin = 1.02*10^(-4)*rmin^2-40*rmin+4619.57
gmin = 0.014*rmin^2-48*rmin+1783.15
hmin = 2.76*10^(-3)*rmin^2-40*rmin+1043.48
imin = 0.374*rmin^2-48*rmin+402.78

% The expressions should all be less than 0 with the given gear ratios
% Max
fmax = 1.02*10^(-4)*rmax^2-40*rmax+4619.57
gmax = 0.014*rmax^2-48*rmax+1783.15
hmax = 2.76*10^(-3)*rmax^2-40*rmax+1043.48
imax = 0.374*rmax^2-48*rmax+402.78