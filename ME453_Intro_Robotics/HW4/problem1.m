syms a b c rho x y z real

%% Ixx
% Integral of whole block/square
x1 = int((y^2+z^2)*rho, x, [-a/2 a/2]);
y1 = int(x1,y,[-b/2 b/2]);
z1 = int(y1,z,[-c-b/2 b/2+c]);

% Integral of "empty pole"
x2 = int((y^2+z^2)*rho,x, [-a/2 a/2]);
y2 = int(x2,y,[-b/2 -c/2]);
z2 = int(y2,z,[-b/2 b/2]);

% Subract inertia
Ixx = z1 - 2*z2;
disp(simplify(Ixx));


%% Iyy
% Integral of whole block/cube
x1 = int((x^2+z^2)*rho, x, [-a/2 a/2]);
y1 = int(x1,y,[-b/2 b/2]);
z1 = int(y1,z,[-c-b/2 b/2+c]);

% Integral of "empty pole"
x2 = int((x^2+z^2)*rho,x, [-a/2 a/2]);
y2 = int(x2,y,[-b/2 -c/2]);
z2 = int(y2,z,[-b/2 b/2]);

% Subract inertia
Iyy = z1 - 2*z2;
disp(simplify(Iyy));

%% Izz
% Integral of whole block/square
x1 = int((x^2+y^2)*rho, x, [-a/2 a/2]);
y1 = int(x1,y,[-b/2 b/2]);
z1 = int(y1,z,[-c-b/2 b/2+c]);

% Integral of "empty pole"
x2 = int((x^2+y^2)*rho,x, [-a/2 a/2]);
y2 = int(x2,y,[-b/2 -c/2]);
z2 = int(y2,z,[-b/2 b/2]);

% Subract inertia
Izz = z1 - 2*z2;
disp(simplify(Izz));

