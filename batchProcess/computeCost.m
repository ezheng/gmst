function cost = computeCost(pointsList)

numOfPoints = size(pointsList, 1);

cost = 0;
for i = 1:numOfPoints-1
    oneCost = sum(      (pointsList(i,:) - pointsList(i+1,:)).^2  );
    
    cost = cost + oneCost;
end
