function F = fundamental_matrix(A)
    % Extract fundamental matrix from the column of V corresponding to the 
    % smallest singular value.
    [~,~,V] = svd(A);
    F = reshape(V(:,9),3,3)';

    % Enforce rank2 constraint
    [U,D,V] = svd(F);
    F = U*diag([D(1,1) D(2,2) 0])*V';
end