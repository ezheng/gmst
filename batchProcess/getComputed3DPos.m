function calculatedPos = getComputed3DPos(cams, nodesCVX, discretizedLevel, near, far)

numOfCams = numel(cams);
% [nodeId,camId,~] = find(nodesCVX);
nodeId = nodesCVX;



calculatedPos = zeros(3, numOfCams);
for i = 1:numOfCams  
    thetaValue = linspace(near(i), far(i), discretizedLevel);      
    calculatedPos(:,i) = cams{i}.ori * thetaValue(nodeId(i))...
        + cams{i}.C;       
    
end
