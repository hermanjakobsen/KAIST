syms ddx mb t lcom ddt dt g real;

f22 = [ddx*mb*cos(t)+mb*g*sin(t)-mb*ddt*lcom; -ddx*mb*sin(t)+mb*g*cos(t)+mb*dt^2*lcom; 0];

R12 = [cos(t) -sin(t) 0; sin(t) cos(t) 0; 0 0 1];

test = simplify(R12*f22);

disp(test);