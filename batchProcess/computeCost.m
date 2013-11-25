function cost = computeCost(pointsList, camConnect)

if(nargin == 1)

numOfPoints = size(pointsList, 1);
cost = 0;
for i = 1:numOfPoints-1
    oneCost = sum(      (pointsList(i,:) - pointsList(i+1,:)).^2  );
    
    cost = cost + oneCost;
end


else
    numOfPairs = size(camConnect, 1);
    cost = 0;
    for i = 1:numOfPairs
        oneCost = sum(  (pointsList( camConnect(i,1), :) - pointsList( camConnect(i,2), :) ).^2 );        
        cost = cost + oneCost;
    end   
end

