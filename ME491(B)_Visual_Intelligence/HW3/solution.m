clear all
% pixels
image_points = [
532 760 1
228 707 1
543 918 1
826 779 1
353 453 1
946 786 1
370 1136 1
453 625 1
718 772 1
460 928 1
545 461 1
825 620 1
826 936 1
555 1100 1
]';

% 3D points
world_points = [
0 0 0 1.0000
0.1200 0 0.4800 1.0000
0 0.2400 0.1200 1.0000
0.3600 0.3600 0 1.0000
0.4800 0 0.4800 1.0000
0.4800 0.4800 0 1.0000
0 0.4800 0.4800 1.0000
0.2400 0 0.2400 1.0000
0.2400 0.2400 0 1.0000
0 0.2400 0.2400 1.0000
0.4800 0 0.2400 1.0000
0.4800 0.2400 0 1.0000
0.2400 0.4800 0 1.0000
0 0.4800 0.2400 1.0000
]';

[d_p, n_p] = size(image_points);

% image data normalization
mean_image_points = mean(image_points');
dist_sum=0;
for i=1:n_p
    dist_sum=dist_sum+norm(image_points(:, i)-mean_image_points')^2;
end
scale_2 = sqrt(dist_sum/2/n_p);
T_image = [1/scale_2, 0, -1/scale_2*mean_image_points(1); 0, 1/scale_2, -1/scale_2*mean_image_points(2); 0, 0, 1];
norm_image_points = T_image*image_points;

% world data normalization
mean_world_points = mean(world_points');
dist_sum=0;
for i=1:n_p
    dist_sum=dist_sum+norm(world_points(:, i)-mean_world_points')^2;
end
scale_3 = sqrt(dist_sum/3/n_p);
T_world = [1/scale_3, 0, 0, -1/scale_3*mean_world_points(1);0, 1/scale_3, 0, -1/scale_3*mean_world_points(2); 0, 0, 1/scale_3, -1/scale_3*mean_world_points(2); 0, 0, 0,1];
norm_world_points = T_world*world_points;


% Data matrix construction
A=zeros(2*n_p, 12);
for i=1:n_p
A((i-1)*2+1, :) = [norm_world_points(1,i),norm_world_points(2, i), norm_world_points(3, i),1, 0, 0, 0, 0, -norm_image_points(1,i)*norm_world_points(1, i), -norm_image_points(1,i)*norm_world_points(2, i), -norm_image_points(1,i)*norm_world_points(3, i), -norm_image_points(1,i)];
A((i-1)*2+2, :) = [0, 0, 0, 0, -norm_world_points(1, i), -norm_world_points(2,i), -norm_world_points(3, i), -1,norm_image_points(2, i)*norm_world_points(1, i),norm_image_points(2, i)*norm_world_points(2, i),norm_image_points(2, i)*norm_world_points(3, i),norm_image_points(2, i)];
end
% projection matrix computation
[U, S, V] = svd(A);
p = V(:, 12);
proj = reshape(p, 4, 3)';
% backward transform to fit to the original data
proj_ori = inv(T_image)*proj*T_world;
% camera center position in the world frame
C = null(proj_ori);
Co = C/C(4);
% QR factorization
M = proj_ori(1:3, 1:3);
[R_inv, K_inv] =qr(inv(M));
R=inv(R_inv);
% K matrix normalization (making the (3,3) element 1)
K_temp=inv(K_inv);
K=K_temp/K_temp(3,3);
% also rescaling the projection matrix
tmy = -R*Co(1:3);
proj_final = proj_ori/K_temp(3,3);
t = inv(K) * proj_final(:, 4);