clc; clear; close all;
%% Define parameters and constants

% Link 1
a1 = 0;
alpha1 = pi/2;
d1 = 0.1;

% Link 2
a2 = 0.5;
alpha2 = 0;
d2 = 0;

% Link 3
a3 = 0.3;
alpha3 = 0;
d3 = 0;

% Joint angles 
theta1 = linspace(-pi/2,pi/2,50); % theta1 range from -pi/2 to pi/2 and divide into 50 units 
theta2 = linspace(pi/6, pi/2, 50); 
theta3 = linspace(-pi/3, pi/2, 50);

%% Forward kinematics 

% Define rotation matrices and translation matrices
syms alpha;
syms theta;
syms a;
syms d;

Rx = [1 0 0 0; 0 cos(alpha) -sin(alpha) 0; 0 sin(alpha) cos(alpha) 0; 0 0 0 1];
Rz = [cos(theta) -sin(theta) 0 0; sin(theta) cos(theta) 0 0; 0 0 1 0; 0 0 0 1];

Tx = [1 0 0 a; 0 1 0 0; 0 0 1 0; 0 0 0 1];
Tz = [1 0 0 0; 0 1 0 0; 0 0 1 d; 0 0 0 1];

% Homogeneous transformation
H = Tz*Rz*Tx*Rx;

syms theta01;
syms theta12;
syms theta23;

H01 = subs(H, [alpha a theta d], [alpha1 a1 theta01 d1]);
H12 = subs(H, [alpha a theta d], [alpha2 a2 theta12 d2]);
H23 = subs(H, [alpha a theta d], [alpha3 a3 theta23 d3]);

H03= H01*H12*H23; % Position and orientation of frame 3 with respect to frame 0
R03 = H03(1:3, 1:3);
p03 = H03(1:3, 4);

% Coordinates of end-effector with respect to frame 0
x = p03(1);
% x = (cos(theta01)*cos(theta12))/2 - (3*cos(theta01)*sin(theta12)*sin(theta23))/10 + (3*cos(theta01)*cos(theta12)*cos(theta23))/10
y = p03(2); 
% y = (cos(theta12)*sin(theta01))/2 - (3*sin(theta01)*sin(theta12)*sin(theta23))/10 + (3*cos(theta12)*cos(theta23)*sin(theta01))/10
z = p03(3);
% z = sin(theta12)/2 + (3*cos(theta12)*sin(theta23))/10 + (3*cos(theta23)*sin(theta12))/10 + 1/10


%% Plot workspace

% Obtain all the different x-y-z coordinates
[THETA1,THETA2,THETA3]=ndgrid(theta1,theta2,theta3);
xM = (cos(THETA1).*cos(THETA2))/2-(3*cos(THETA1).*sin(THETA2).*sin(THETA3))/10 + (3*cos(THETA1).*cos(THETA2).*cos(THETA3))/10;
yM = (cos(THETA2).*sin(THETA1))/2 - (3*sin(THETA1).*sin(THETA2).*sin(THETA3))/10 + (3*cos(THETA2).*cos(THETA3).*sin(THETA1))/10;
zM = sin(THETA2)/2 + (3*cos(THETA2).*sin(THETA3))/10 + (3*cos(THETA3).*sin(THETA2))/10 + 1/10;

plot3(xM(:),yM(:),zM(:));
hold on;
xlabel('x');
ylabel('y');
zlabel('z');
title('Workspace of robot arm');
plot3(0,0,0, '*', 'color','r');
grid on;


%% Test 2c - Inverse kinematics

% Define an arbitrary point inside the arm's workspace
xe = 0.2;
ye = 0.1;
ze = 0.5;

% Calculate the joint angles
c3 = (xe^2+ye^2+(ze-d1)^2-a2^2-a3^2)/(2*a2*a3);
t1 = atan2(ye, xe);
t3 = atan2(sqrt(1-c3^2), c3);
t2 = atan2(ze-d1, sqrt(xe^2+ye^2)) - atan2(a3*sin(t3), a2+a3*cos(t3));

% Calculate the coordinates using forward kinematics with the newly obtained angles
xcalc = (cos(t1).*cos(t2))/2-(3*cos(t1).*sin(t2).*sin(t3))/10 + (3*cos(t1).*cos(t2).*cos(t3))/10;
ycalc = (cos(t2).*sin(t1))/2 - (3*sin(t1).*sin(t2).*sin(t3))/10 + (3*cos(t2).*cos(t3).*sin(t1))/10;
zcalc = sin(t2)/2 + (3*cos(t2).*sin(t3))/10 + (3*cos(t3).*sin(t2))/10 + 1/10;

% Check if calculated coordinates equal defined coordinates
testCase = matlab.unittest.TestCase.forInteractiveUse;
assertEqual(testCase,xe,round(xcalc, 10)); % Using round() because of precision loss in calculations
assertEqual(testCase,ye,round(ycalc, 10));
assertEqual(testCase,ze,round(zcalc, 10));

