function  points3D = ReconstructPointTrajectory_SumOfNorm_direction(C, camConnect, orientation, points3D, weight)

newOrientation = zeros(3, size(camConnect, 1));

for i = 1:size(camConnect, 1)
    dir = points3D(:, camConnect(i,1) ) - points3D( :, camConnect(i,2) );
    dir = dir/norm(dir);
    
    maxValue = zeros(2,1);
    twoOrientation = [ orientation(1:3, camConnect(i,1)),  orientation(1:3, camConnect(i,2))  ];
    
    for j = 1:2
        maxValue(j) = abs( dot( twoOrientation(:,j ), dir) );        
    end
    [~,idx] = max( maxValue );
    
    newOrientation(:,i) = twoOrientation(:, idx);
    
end
% ------------------------------------------------------------------
numOfCameras = numel(C);

cameraCenterMatrix = zeros( 3, numOfCameras);
directionMatrix = zeros(3, numOfCameras);


for i = 1: numOfCameras
   cameraCenterMatrix(:, i) = C{i}.C;
   
%    compute the direction:
%     K = C{i}.K; focalLength = (K(1,1) + K(2,2))/2;
%     measure = [C{i}.m(pointIdx, :)'; 1];
%     measure =  inv(K)*measure;
%     measure = [C{i}.R',C{i}.C]*[measure; 1];    
%     direction = measure - C{i}.C;
    direction = C{i}.ori;
    direction = direction/norm(direction);
    directionMatrix(:, i) = direction;    
end
% weight = 1/weight; weight = 0;
cvx_begin
    variable x(numOfCameras,1);
    pts = cameraCenterMatrix + directionMatrix .* repmat(x', 3,1);
    allSum = 0;
    for i = 1:size(camConnect, 1)        
        allSum = allSum + sum( square(  pts(:, camConnect(i,1) ) - pts(:, camConnect(i,2))  ));   
%         allSum = allSum 
        diff = cross( newOrientation(:,i), pts(:, camConnect(i,1) ) - pts(:, camConnect(i,2)) );
        allSum = allSum + weight * sum(square(diff));
%         allSum = weight * allSum + sum(square(diff));
%         allSum = sum(square(diff));
%          allSum = allSum + norm( pts(:, camConnect(i,1) ) - pts(:, camConnect(i,2) ) );
    end
    minimize allSum;
%     subject to 
%         x>=0;
cvx_end
    
points3D = cameraCenterMatrix + directionMatrix .* repmat(x', 3,1);

% -------------------------------------------------------------------------------------------------

A1 = zeros(numOfCameras, numOfCameras);
b1 = zeros(numOfCameras,1);
for i = 1:size(camConnect,1)
    id1 = camConnect(i,1); id2 = camConnect(i,2);
    
    A1( id1, id2) = -dot(C{id1}.ori, newOrientation(:,i)) * dot(C{id2}.ori, newOrientation(:,i)) ;
    A1( id2, id1) = A1(id1,id2);
    
    A1( id1, id1) = A1( id1, id1) + dot(C{id1}.ori, newOrientation(:,i)) ^2;
    A1( id2, id2) = A1( id2, id2) + dot(C{id2}.ori, newOrientation(:,i)) ^2;
%     A( id1, id2) = -dot(C{id1}.ori, C{id2}.ori);
%     A( id2, id1) = A(id1,id2);
    
    b1(id1) = b1(id1) - dot(C{id1}.C - C{id2}.C, newOrientation(:,i) ) * dot(C{id1}.ori, newOrientation(:,i));
    b1(id2) = b1(id2) - dot(C{id2}.C - C{id1}.C, newOrientation(:,i) ) * dot(C{id2}.ori, newOrientation(:,i));
end

A = zeros(numOfCameras, numOfCameras);
b = zeros(numOfCameras,1);
for i = 1:size(camConnect,1)
    id1 = camConnect(i,1); id2 = camConnect(i,2);
    A( id1, id2) = -dot(C{id1}.ori, C{id2}.ori);
    A( id2, id1) = A(id1,id2);
    
    b(id1) = b(id1) - dot(C{id1}.C - C{id2}.C, C{id1}.ori );
    b(id2) = b(id2) - dot(C{id2}.C - C{id1}.C, C{id2}.ori );
end

for i = 1:numel(C)
    n = sum( A(i,:) ~= 0 );
    A(i,i) = n;    
end

A2 = (weight+1)*A - weight*A1; b2 = (weight + 1)* b - weight* b1;
% ---------------------------------------------------------------------------------------------
xx = A2 \ b2;

points3D = cameraCenterMatrix + directionMatrix .* repmat(xx', 3,1);



