function orientation = findTwoWallsOrientation_coarse(C, points3D, h)


allPoints3D = [points3D.pos];
allPoints3D = reshape(allPoints3D, 3, []);

numOfAllPoints3D = size(allPoints3D,2);
% allPoints3D = allPoints3D';

[B1, ~, inliersIndices] = ransacfitplane(allPoints3D, 0.1);


leftIdx = setdiff([1:numOfAllPoints3D], inliersIndices);

[B2, ~, ~] = ransacfitplane(allPoints3D(:, leftIdx) , 0.1);
% find plane 2

% --------------------------------------------------------------------
numOfCams = numel(C);
cams = [C{:}];
camCenters = [cams.C];

[planeParam, ~,~] = ransacfitplane( camCenters, 0.1);


orientation1 = cross(  B1(1:3)/norm(B1(1:3)),   planeParam(1:3)/norm(planeParam(1:3)) );
orientation2 = cross(  B2(1:3)/norm(B2(1:3)),   planeParam(1:3)/norm(planeParam(1:3)) );
orientation1 = orientation1/norm(orientation1);
orientation2 = orientation2/norm(orientation2);

if(nargin == 3)
    figure(h); hold on; 
    pts = [camCenters(:,1), camCenters(:,1) + orientation1*10];
    plot3( pts(1,:),pts(2,:), pts(3,:) );
   
    pts = [camCenters(:,1), camCenters(:,1) + orientation2*10];
    plot3( pts(1,:),pts(2,:), pts(3,:) );
    hold off;
end

%
% orientation = [orientation1, orientation2];
orientation = zeros(3, numOfCams);
imageDirection = {[2328,2337],[2339,2364], [2367,2385]};

for i = 1:numOfCams
%     get the name, find the range
    fileName = C{i}.fileName;
     [~,name,~] = fileparts(fileName);
    num = str2double( regexp(name, '\d+', 'match') );
    
    if( (num >= imageDirection{1}(1) && num <= imageDirection{1}(2))  ||  (num >= imageDirection{3}(1) && num <= imageDirection{3}(2)) )
        orientation(:,i) = [orientation2 ];        
    else 
        orientation(:,i) = [orientation1];        
    end
    
end




% do the quantization

% allDirections = [ [-1, 0, 0]', [-1/sqrt(2), 0, -1/sqrt(2) ]',[0,0,-1]',[1/sqrt(2), 0, -1/sqrt(2)]' ];

% numberOfDirections = size( allDirections, 2 );

numberOfDirections = 6;
allDirections = zeros(3, numberOfDirections);
angle = pi/numberOfDirections;
for i = 1:numberOfDirections
    allDirections(:,i) = [ cos( pi + angle * (i-1) ), 0, sin( pi + angle * (i-1) ) ];
    assert( abs(norm(allDirections(:,i), 2 )-1)<0.0000001 )
end

for i = 1 : numOfCams
   angleValue = zeros(numberOfDirections,1);
   
   normedPlaneDir = planeParam(1:3)/norm(planeParam(1:3));
   
  
       for j = 1:numberOfDirections
           angleValue(j) = computeAngles( allDirections(:,j), C{i}.R, orientation( :, i), normedPlaneDir  );
       end
       
       [~,idx] = max(angleValue);
       
       direction = C{i}.R' * allDirections(:,idx);
       
       direction = direction - (direction'*normedPlaneDir)*normedPlaneDir;
       direction = direction/norm(direction);
       orientation(:,i) = direction;   
end

if( nargin == 3 )
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

end


function angleValue = computeAngles(direction, R, ori, planeParam)

% switch( directionId(i) )
%         case 1
%             direction = [-1 0 0]';
%         case 2
%             direction = [-1/sqrt(2), 0, -1/sqrt(2) ]';
%         case 3
%             direction = [0,0,-1]';
%         case 4
%             direction = [1/sqrt(2), 0, -1/sqrt(2)]';
%     end
    
    direction = R' * direction;
    
    direction = direction - (direction'*planeParam)*planeParam;    
    direction = direction/norm(direction);
    
    angleValue = abs(direction' * (ori/norm(ori)));     % note computing the absolute value
    
end







