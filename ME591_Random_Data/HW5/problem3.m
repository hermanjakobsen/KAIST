clc; clear; close all;
%% Problem 3 - Spatial filter
% Spatial filtering is done through convolving the filter mask over the
% pixels. 

h = 1/16*[1 2 1; 2 4 2; 1 2 1]; % Filter mask - Gaussian approximation
w = conv2(h,h); % Filter mask by applying filter two times
