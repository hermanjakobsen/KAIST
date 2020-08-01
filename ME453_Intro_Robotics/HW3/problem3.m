clc; clear; close all;

%% Parameters

syms theta1 theta2 theta3 L1 L2 L3 real;


%% Problem 3a

% Forward kinematics
H01 = DH_matrix(L1,theta1+sym(pi)/2,0,sym(pi)/2);
H12 = DH_matrix(0,theta2,L2,0);
H2e = DH_matrix(0,theta3,L3,0);

H02 = H01*H12;
H0e = H02*H2e;

% Extract position
P01 = H01(1:3,4);
P02 = H02(1:3,4);
P0e = H0e(1:3,4);

% Extract z-axis
z0 = [0 0 1]';
z1 = H01(1:3,3);
z2 = H02(1:3,3);
ze = H0e(1:3,3);

% Every joint is revolute
% Calculate translation velocity part of Jacobian matrix of end-effector
Jv = [cross(z0,P0e) cross(z1,(P0e-P01)) cross(z2,(P0e-P02))];


%% Problem 3b

% Calculate determinant
detJv = simplify(det(Jv));

% Applying further algebraic manipulation and trigonometric identities on the determinant gives condition for
% singular configuration


%% Problem 3c

Jv = simplify(subs(Jv, [L1 L2 L3], [0.5 1.5 1]));

% Case 1, theta3 = 0
Jv1 = subs(Jv, theta3, 0);
nullsp1 = null(Jv1);

% Case 2, theta3 = pi
Jv2 = subs(Jv, theta3, pi);
nullsp2 = null(Jv2);

% Case 3, L2c2 + L3c23 = 0
% The expression can be rewritten as 
% theta3 = acos(-L2/L3*cos(theta2))-theta2
Jv3 = subs(Jv, theta3, acos(-L2/L3*cos(theta2))-theta2);
Jv3 = simplify(subs(Jv3, [L1 L2 L3], [0.5 1.5 1]));
nullsp3 = null(Jv3);


%% Functions

function M = DH_matrix(d,theta,a,alpha) % Calculate Denavit-Hartenberg Matrix
    M = [cos(theta) -sin(theta)*cos(alpha) sin(theta)*sin(alpha) a*cos(theta); sin(theta) cos(theta)*cos(alpha) -cos(theta)*sin(alpha) a*sin(theta); 0 sin(alpha) cos(alpha) d; 0 0 0 1];
end


