function nodes = getNodes3dPos(cams,discretizedLevel, near, far)

numOfCams = numel(cams);
nodes = zeros(3, numOfCams * discretizedLevel );

startP = 1;
endP = discretizedLevel;



for i = 1:numOfCams    
    thetaValue = linspace(near(i), far(i), discretizedLevel);
    
    nodes(:, startP:endP) = repmat(cams{i}.ori, 1, discretizedLevel) .* repmat(thetaValue, 3, 1)...
        + repmat(cams{i}.C, 1, discretizedLevel); 
    
    startP = endP + 1;
    endP = endP + discretizedLevel;
    
end