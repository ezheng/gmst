function orientation = findOrientationByDetection(C, h)

directionId = [1 1 4 3 4 4 1 3 3 1 ...
2 1 4 4 4 3 1 1 1 1 ...
2 2 1 1 4 3 4 3 2 2 ...
2 1 1 1 1 3 4 1 1 1 ...
4 2 2 2 1 1 1 2 2 3 2 4];       % 1 is left, 2 is 45, 3 is 90. 4 is 135 degrees

% find the ground plane

numOfCams = numel(C);
cams = [C{:}];
camCenters = [cams.C];
[planeParam, ~,~] = ransacfitplane( camCenters, 0.1);
planeParam = planeParam(1:3)/norm(planeParam(1:3));

orientation  = zeros(3, numOfCams);

for i = 1:numel(C)
    
    
    switch( directionId(i) )
        case 1
            direction = [-1 0 0]';
        case 2
            direction = [-1/sqrt(2), 0, -1/sqrt(2) ]';
        case 3
            direction = [0,0,-1]';
        case 4
            direction = [1/sqrt(2), 0, -1/sqrt(2)]';
    end
    
    direction = C{i}.R' * direction;
    
    direction = direction - (direction'*planeParam)*planeParam;    
    orientation(:,i) = direction/norm(direction);
%     transform direction    
%     project to the image plane        
end


if(nargin == 2)
    figure(h);   
    hold on; 
    for i = 1:numOfCams
        center = C{i}.C;
%         also plot the orientation 
        plot3( [center(1), center(1) + 0.1*orientation(1,i)],[center(2), center(2) + 0.1*orientation(2,i)],...
            [center(3), center(3) + 0.1*orientation(3,i)]);
%         plot3( [center(1), center(1)+ 0.1*orientation(4,i)],[center(2), center(2)+ 0.1*orientation(5,i)],...
%             [center(3), center(3)+ 0.1*orientation(6,i)], 'r');
    end
    hold off;
end