function [K,R,t,Co] = camera_parameters(A,T_img,T_world)
    % Constructing projection matrix
    [~,~,V] = svd(A);
    p = V(:,12);    % Vector containing projection matrix elements
    proj = reshape(p,4,3)'; % Projection matrix
    proj_og = T_img\proj*T_world; % Backward transform to get original data
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
end