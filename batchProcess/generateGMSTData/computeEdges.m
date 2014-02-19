function [edges, edgesCost] = computeEdges(nodes, discretizedLevel, orientation)

numOfNodes = size(nodes,2);
numOfCams = numOfNodes / discretizedLevel;

[xMesh, yMesh] = meshgrid(1:numOfNodes, 1:numOfNodes);
xMesh = xMesh(:); yMesh = yMesh(:);

diff = nodes(:,xMesh) - nodes(:,yMesh);

% if(nargin == 2)
    adjanceyMatrix = sum( diff .^2 , 1);
if(nargin == 3)
%     diff1 = abs(orientation(:,1)' * diff);
%     diff2 = abs(orientation(:,2)' * diff);
    if( size(orientation,2) <= 1)
        oneOrientation1 = repmat(orientation(:,1), 1, numOfCams);   
    else
        oneOrientation1 = orientation(1:3,:);
    end
    
    id = floor((xMesh-1)/discretizedLevel+1);
    oneOrientation1 = oneOrientation1(1:3, id);
    
    diff1 = cross( oneOrientation1 , diff);
    diff1 = sum(diff1.^2, 1);
    
    if(size(orientation,1) ==  6 )
%          oneOrientation2 = repmat(orientation(:,1), 1, numOfCams);     
         oneOrientation2 = orientation(4:6,:);
         oneOrientation2 = oneOrientation2(:, id);
         diff2 = cross( repmat(oneOrientation2(:,2), 1, size(diff,2)), diff);
         diff2 = sum(diff2.^2, 1);
         
         diff1 = min([diff1; diff2], [], 1);
    end        
     
    adjanceyMatrix = adjanceyMatrix + 20*diff1;
end

    
adjanceyMatrix = reshape(adjanceyMatrix, numOfNodes, numOfNodes);

mask = kron( eye(numOfCams), -ones(discretizedLevel)) + 1;
adjanceyMatrix = adjanceyMatrix .* mask;

% -----------------------------------------------------------------
% numberRemoved = round( size(adjanceyMatrix,1) * 0.95);
% for i = 1: size(adjanceyMatrix,1)
%     [~,idx] = sort( adjanceyMatrix(i, adjanceyMatrix(i,:)~=0), 'descend' );
%     adjanceyMatrix(i, idx(1:numberRemoved)) = 0;    
%     
% end
% adjanceyMatrixT = adjanceyMatrix'; 
% adjanceyMatrix(adjanceyMatrix == 0) = adjanceyMatrixT(adjanceyMatrix == 0);
% -----------------------------------------------------------------


mask = triu(ones(size(adjanceyMatrix)), 1);
adjanceyMatrix = adjanceyMatrix .* mask;
adjanceyMatrix = adjanceyMatrix';

edgesCost = nonzeros(adjanceyMatrix);   % remove the edges with 0 cost
idx = find(adjanceyMatrix);
[edges1, edges2] = ind2sub(size(adjanceyMatrix),idx);
edges = [edges2, edges1];

% adjacencyMatrix = 









