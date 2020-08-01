
%% Parameters and matrices
syms x theta mc mb lcom Ib real

q = [x theta]';
D = [mc+mb -mb*lcom*cos(theta); -mb*lcom*cos(theta) mb*lcom^2+Ib];
N = 2;

%% Christoffel symbols
for i = 1:N
    for j = 1:N
        for k = 1:N
            c = 1/2*(diff(D(k,j),q(i)) + diff(D(k,i),q(j)) - diff(D(i,j),q(k)));
            fprintf('%d%d%d, %s\n', i, j, k, c);
        end
    end
end
