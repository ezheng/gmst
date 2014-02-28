function  points3D = ReconstructPointTrajectory_SumOfNorm(C, camConnect)

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

cvx_begin
    variable x(numOfCameras,1);
    pts = cameraCenterMatrix + directionMatrix .* repmat(x', 3,1);
    allSum = 0;
    for i = 1:size(camConnect, 1)        
        allSum = allSum + sum( square(  pts(:, camConnect(i,1) ) - pts(:, camConnect(i,2))  ));    
%          allSum = allSum + norm( pts(:, camConnect(i,1) ) - pts(:, camConnect(i,2) ) );
    end
    minimize allSum;
%     subject to 
%         x>=0;
cvx_end
    
points3D = cameraCenterMatrix + directionMatrix .* repmat(x', 3,1);


% -------------------------------------------------

% A = zeros(numOfCameras, numOfCameras);
% b = zeros(numOfCameras,1);
% for i = 1:size(camConnect,1)
%     id1 = camConnect(i,1); id2 = camConnect(i,2);
%     A( id1, id2) = -dot(C{id1}.ori, C{id2}.ori);
%     A( id2, id1) = A(id1,id2);
%     
%     b(id1) = b(id1) - dot(C{id1}.C - C{id2}.C, C{id1}.ori );
%     b(id2) = b(id2) - dot(C{id2}.C - C{id1}.C, C{id2}.ori );
% end
% 
% for i = 1:numel(C)
%     n = sum( A(i,:) ~= 0 );
%     A(i,i) = n;    
% end
% 
% xx = A\b;










