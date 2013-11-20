function [edges, edgesCost] = computeEdges(nodes, discretizedLevel)

numOfNodes = size(nodes,2);
numOfCams = numOfNodes / discretizedLevel;

[xMesh, yMesh] = meshgrid(1:numOfNodes, 1:numOfNodes);
xMesh = xMesh(:); yMesh = yMesh(:);
adjanceyMatrix = sum((nodes(:,xMesh) - nodes(:,yMesh) ).^2 , 1);
adjanceyMatrix = reshape(adjanceyMatrix, numOfNodes, numOfNodes);

mask = kron( eye(numOfCams), -ones(discretizedLevel)) + 1;
adjanceyMatrix = adjanceyMatrix .* mask;

mask = triu(ones(size(adjanceyMatrix)), 1);
adjanceyMatrix = adjanceyMatrix .* mask;
adjanceyMatrix = adjanceyMatrix';

edgesCost = nonzeros(adjanceyMatrix);
idx = find(adjanceyMatrix);
[edges1, edges2] = ind2sub(size(adjanceyMatrix),idx);
edges = [edges2, edges1];

% adjacencyMatrix = 









