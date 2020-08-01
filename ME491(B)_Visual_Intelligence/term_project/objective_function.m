function val = objective_function(x,pts,pts_next)
    T(1,1) = x(1);
    T(1,2) = x(2);
    T(1,3) = x(3);
    T(1,4) = x(4);
    T(2,1) = x(5);
    T(2,2) = x(6);
    T(2,3) = x(7);
    T(2,4) = x(8);
    T(3,1) = x(9);
    T(3,2) = x(10);
    T(3,3) = x(11);
    T(3,4) = x(12);
    T(4,1) = 0;
    T(4,2) = 0;
    T(4,3) = 0;
    T(4,4) = 1;

    val = 0;
    [num_pts,~] = size(pts); 
    for i = 1:num_pts
        val = val + norm(pts_next(i,:)'-T*(pts(i,:)'));
    end
end