clear variables; 
%% Image points (2D)
% The points are given as homogeneous coordinates with pixels as
% coordinates. The origin (0,0) is defined in the left top corner.
h = 1199;   % Max pixel value on y-axis of image. Used to correct origin 
            % to left bottom corner.
            
image_points = [
    761     532     1
    833     576     1
    765     454     1
    693     574     1
    697     494     1
    765     620     1
    840     498     1
    771     372     1
    700     411     1
    625     453     1
    624     538     1
    623     617     1
    695     667     1
    772     717     1
    842     668     1
    908     622     1
    918     543     1
    928     459     1
    848     414     1
    453     353     1
    468     714     1
    787     946     1
    1069    720     1
    1146    369     1
    782     190     1
    545     498     1
    1012    507     1
    779     825     1
    ]';

image_points(2,:) = h-image_points(2,:);    % Move origin to left bottom 
                                            % Normalization of data makes
                                            % this redundant, though
                                            

%% World points (3D)
% The points are given as homogeneous coordinates with meters as 
% coordinates. The origin of the world frame is set at the intersection
% between the three planes (see image).
world_points = [
    0       0       0       1
    0       0.12    0       1
    0       0       0.12    1
    0.12    0       0       1
    0.12    0       0.12    1
    0.12    0.12    0       1
    0       0.12    0.12    1
    0       0       0.24    1
    0.12    0       0.24    1
    0.24    0       0.24    1
    0.24    0       0.12    1
    0.24    0       0       1
    0.24    0.12    0       1
    0.24    0.24    0       1
    0.12    0.24    0       1
    0       0.24    0       1
    0       0.24    0.12    1
    0       0.24    0.24    1
    0       0.12    0.24    1
    0.48    0       0.48    1
    0.48    0       0       1
    0.48    0.48    0       1
    0       0.48    0       1
    0       0.48    0.48    1
    0       0       0.48    1
    0.36    0       0.24    1
    0       0.36    0.24    1
    0.36    0.36    0       1
    ]';

[~,number_points] = size(image_points);


%% Data normalization (Isotropic scaling)
% Necessary in order to use Direct Linear Transformation when
% constructing the A matrix.

% Image points normalization
mean_image_points = mean(image_points,2); % Mean of each row (coordinate)
dist_sum = 0;
for i = 1:number_points
   dist_sum = dist_sum+norm(image_points(:,i)-mean_image_points)^2;
end
scale_2 = sqrt(dist_sum/(2*number_points));
T_image = [
    1/scale_2, 0, -1/scale_2*mean_image_points(1);
    0, 1/scale_2, -1/scale_2*mean_image_points(2);
    0, 0, 1
    ];
norm_image_points = T_image*image_points;

% World points normalization
mean_world_points = mean(world_points,2);
dist_sum = 0;
for i = 1:number_points
   dist_sum = dist_sum+norm(world_points(:,i)-mean_world_points)^2;
end
scale_3 = sqrt(dist_sum/(3*number_points));
T_world = [
    1/scale_3, 0, 0, -1/scale_3*mean_world_points(1);
    0, 1/scale_3, 0, -1/scale_3*mean_world_points(2);
    0, 0, 1/scale_3, -1/scale_3*mean_world_points(3);
    0, 0, 0, 1;
    ];
norm_world_points = T_world*world_points;


%% Camera Calibration
% Constructing A matrix
A = zeros(2*number_points, 12);
for i = 1:number_points
    % First equation for each point
    A((i-1)*2+1,:) = [
        0, 0, 0, 0, ...
        (-norm_image_points(3,i)*norm_world_points(:,i))', ...
        (norm_image_points(2,i)*norm_world_points(:,i))'
    ];
    % Second equation for each point
    A((i-1)*2+2,:) = [
        (norm_image_points(3,i)*norm_world_points(:,i))', ...
        0, 0, 0, 0, ...
        (-norm_image_points(1,i)*norm_world_points(:,i))'
    ];
end

% Constructing projection matrix
[~,~,V] = svd(A);

p = V(:,12);    % Vector containing projection matrix elements
proj = reshape(p,4,3)'; % Projection matrix
proj_og = T_image\proj*T_world; % Backward transform to get original data

% Optimize projection matrix
fun = @(P,x_data)(P*x_data);
options = optimoptions('lsqcurvefit','Algorithm','levenberg-marquardt');
lb = [];    % Should restrict the norm of the vector from x to be unitary?
ub = [];
x = lsqcurvefit(fun,proj_og,world_points,image_points,lb,ub,options);
% Gave good results when displaying x*world_points, but the following Co
% matrix became weird? Maybe due to the optimization being none restricted?
% Will therefore use proj_og.

% Camera center position in world frame
C = null(proj_og);
Co = C / C(4);   % Normalize

% Constructing K and R matrix
M = proj_og(1:3,1:3);
[R_inv,K_inv] = qr(inv(M));
R = inv(R_inv);
K_temp = inv(K_inv);
K = K_temp / K_temp(3,3); % Normalization. Make K(3,3) equal to 1.

% Constructing translation vector
t = -R*Co(1:3);