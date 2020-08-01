function vv = Fdiversity(data,rowsize,colsize)
  
for i=1:colsize
    maxf(i) = max(data(:,i));
    minf(i) = min(data(:,i));
end

distance = zeros(rowsize,rowsize);
for i=1:rowsize-1
    for j=i+1:rowsize
        for k=1:colsize
            distance(i,j) = distance(i,j) + (data(i,k) - data(j,k))^2;       
        end
        distance(i,j) = sqrt(distance(i,j));
        distance(j,i) = distance(i,j);
    end
end
 
mindistance = zeros(rowsize,1);

for i=1:rowsize
    mindistance(i) = 100000;
    for j=1:rowsize
        if (distance(i,j) > 0.0001)
           if (mindistance(i) > distance(i,j))
                mindistance(i) = distance(i,j);
            end
        end
    end
end

meddistance = 0;
for i=1:rowsize
    meddistance = meddistance + mindistance(i);
end

meddistance = meddistance / rowsize;

diversity = 0;

for i=1:colsize
    diversity = diversity + maxf(i) - minf(i);
end

denom = 0;
for i=1:rowsize
    denom = denom + (mindistance(i) - meddistance)^2;
end

denom = denom/rowsize;
denom = sqrt(denom);
denom = denom;

diversity = diversity/denom;

vv = diversity;