% Origin (0,0) of the image is set to the lower left corner. 
% The points are then given as pixel coordinates in x-direction and 
% y-direction as a "normal" coordinate system. 
% The z-coordinate is added to make a homogeneous coordinate.

%% Vanishing points
% Line top right
p1 = [544; 226; 1];
q1 = [845; 246; 1];
l1 = cross(p1,q1);

% Line top left
p2 = p1;
q2 = [267; 276; 1];
l2 = cross(p2,q2);

% Line bottom right
p3 = [544; 180; 1];
q3 = [845; 209; 1];
l3 = cross(p3,q3);

% Line bottom left
p4 = p3;
q4 = [267; 247; 1];
l4 = cross(p4,q4);

% Line down left
p5 = [132; 385; 1];
q5 = [134; 225; 1];
l5 = cross(p5,q5);

% Line down right
p6 = [552; 531; 1];
q6 = [545; 125; 1];
l6 = cross(p6,q6);

% Vanishing points
vx = cross (l2,l4); % Left vanishing point
vy = cross(l1,l3);  % Right vanishing point
vz = cross(l5,l6);   % Vertical vanishing point

% Transform vanishing points into image frame coordinates
vx = vx / vx(3);
vy = vy / vy(3);
vz = vz / vz(3);


%% Cross ratio and height measurement
R = 201; % cm
b = [408; 119; 1];
r = [415; 413; 1];
b0 = [314; 161; 1];
t0 = [318; 375; 1];

v = cross(cross(b,b0),cross(vx,vy));
t = cross(cross(v,t0),cross(r,b));
t = t / t(3);   % Transform into image frame coordinates

ratio = (norm(t-b)*norm(vz-r)) / (norm(r-b)*norm(vz-t));

height = R*ratio; % cm
