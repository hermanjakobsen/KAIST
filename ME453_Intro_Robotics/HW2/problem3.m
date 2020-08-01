clc; clear; close all;
%% Define parameters and constants

% Given vectors and matrices
rcobj = [0.10 0.05 0.30]';
Rcobj = [0.98 0.17 -0.09; -0.17 0.99 0.02; 0.08 0 1];
Hcobj = [Rcobj, rcobj; 0 0 0 1];

qnow = [30*pi/180 45*pi/180 0.5 30*pi/180 -60*pi/180 30*pi/180]';
qtest = [0 0 0 0 0 0];

% Constants
l1 = 0.5;
l2 = 0.0;
l3 = 1.0;
l4 = 1.0;


%% 3a - Forward Kinematics

% Define rotation matrices and translation matrices
syms alpha;
syms theta;
syms a;
syms zTrans;

Rx = [1 0 0 0; 0 cos(alpha) -sin(alpha) 0; 0 sin(alpha) cos(alpha) 0; 0 0 0 1];
Rz = [cos(theta) -sin(theta) 0 0; sin(theta) cos(theta) 0 0; 0 0 1 0; 0 0 0 1];

Tx = [1 0 0 a; 0 1 0 0; 0 0 1 0; 0 0 0 1];
Tz = [1 0 0 0; 0 1 0 0; 0 0 1 zTrans; 0 0 0 1];

% Homogeneous transformation
H = Tz*Rz*Tx*Rx;

% DH-parameters
syms theta1;
syms theta2;
syms theta3;
syms theta4;
syms theta5;
syms d;

% Revolute joint, link {0}
alpha1 = -pi/2;
a1 = 0;
theta01 = theta1;
d1 = l1;

% Revolute joint, link {1}
alpha2 = pi/2;
a2 = 0;
theta12 = theta2 - pi/2;
d2 = -l2;

% Prismatic joint, link {2}
alpha3 = 0;
a3 = 0;
theta23 = pi/2;
d3 = l3+d;

% Revolute joint, link {3}
alpha4 = pi/2;
a4 = 0;
theta34 = theta3;
d4 = 0;

% Revolute joint, link {4}
alpha5 = -pi/2;
a5 = 0;
theta45 = theta4;
d5 = 0;

% Revolute joint, link {5}
alpha6 = 0;
a6 = 0;
theta56 = theta5;
d6 = l4;

H01 = subs(H, [alpha a theta zTrans], [alpha1 a1 theta01 d1]);
H12 = subs(H, [alpha a theta zTrans], [alpha2 a2 theta12 d2]);
H23 = subs(H, [alpha a theta zTrans], [alpha3 a3 theta23 d3]);
H34 = subs(H, [alpha a theta zTrans], [alpha4 a4 theta34 d4]);
H45 = subs(H, [alpha a theta zTrans], [alpha5 a5 theta45 d5]);
H5e = subs(H, [alpha a theta zTrans], [alpha6 a6 theta56 d6]);

H0e= H01*H12*H23*H34*H45*H5e;


%% 3b - Obtain r0obj and R0obj

% Given that frame {e} has the same position as frame {c}.
% Hence, orientation of frame {c} depends on frame {e} and will therefore
% assume {e}={c}.

% The transformation from frame {c} to frame {obj} is only valid when the 
% parameters are equal to the parameters given in qnow.
% So must set the parameters in R0c and r0c equal to the parameters in
% qnow.

% Finding homogeneous transformation
H0c = H0e;
H0obj= H0c*Hcobj;

% Insert parameters
H0obj = subs(H0obj,[theta1 theta2 d theta3 theta4 theta5], [qnow(1) qnow(2) qnow(3) qnow(4) qnow(5) qnow(6)]);

% Extracting rotation matrix and position vector
R0obj = H0obj(1:3, 1:3);
r0obj = H0obj(1:3, 4);


%% 3c - Inverse kinematics

% Obtain r03. Note that position of frame {e} is equal to position of frame
% {obj} 

r03 = r0obj-l4*R0obj*[0 0 1]';

x03 = r03(1);
y03 = r03(2);
z03 = r03(3);

% Obtain parameters from inverse kinematics
D = sqrt(x03^2+y03^2+(z03-l1)^2)-l3;
t2 = asin((z03-l1)/(l3+D));
t1 = atan(-y03/(1+D*cos(t2)), -x03/(1+D*cos(t2)));


%% 3d - Inverse kinematics
% Assuming that theta1, theta2 and d are given (found in previous
% task)
H03 = H01*H12*H23;
H03 = subs(H03, [theta1 theta2 d], [t1 t2 D]);

R03 = H03(1:3, 1:3);
R3obj = (R03')*R0obj;

r13 = R3obj(1,3);
r23 = R3obj(2,3);
r31 = R3obj(3,1);
r32 = R3obj(3,2);
r33 = R3obj(3,3);

% Obtain parameters from inverse kinematics
t4 = asin(-sqrt(1-r33^2));
t3 = atan2(-r23/sin(t4), -r13/sin(t4));
t5 = atan2(-r32/sin(t4), r31/sin(t4));


%% 3e - Comparison
Htest = subs(H0e, [theta1 theta2 d theta3 theta4 theta5], [t1 t2 D t3 t4 t5]);
Rtest = Htest(1:3, 1:3);
rtest = Htest(1:3, 4);

disp('Actual: ');
disp(double(r0obj));
disp(double(R0obj));

disp('From inverse kinematics: ');
disp(double(rtest));
disp(double(Rtest));

