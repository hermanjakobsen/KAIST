function Y = rearrange_fourier_coeff(Ypure, l)
   Y = zeros(1,l);
   Y(1:l/2) = Ypure(l/2+1:l);
   Y(l/2+1:l) = Ypure(1:l/2);
end