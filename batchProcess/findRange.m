function [near, far] = findRange(searchRangeSize, C, planeParam)

numOfCams = numel(C);


far = zeros(numOfCams,1);

for i = 1:numOfCams
    far(i) = (-planeParam(4) - planeParam(1:3)'*C{i}.C)/(planeParam(1:3)' * C{i}.ori ) ;
    assert(far(i) > 0);
    
end
far = far - 0.1 + rand(1)*0.1;
near = far - searchRangeSize;

near(near<0) = 0;























