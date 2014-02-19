function [edges, edgesCost] = computePartialEdges(nodes, discretizedLevel, table)

table(logical(eye(size(table)))) = inf;
[~,tableIdx] = sort(table, 2);
tableIdx = tableIdx(:, 1:10);

numOfNodes = size(nodes, 2);
numOfCams = numOfNodes/discretizedLevel;

assert(numOfCams == size(tableIdx,1) );

numOfNeighbors = size(tableIdx, 2);
numOfEdges = numOfCams * numOfNeighbors * discretizedLevel * discretizedLevel;
edges = zeros( numOfEdges, 2 );
edgesCost = zeros( numOfEdges, 1);

for i = 1:numOfCams
    
    startNodes = [(i-1) * discretizedLevel+1 : i * discretizedLevel];
    endNodes = zeros(1, numOfNeighbors*discretizedLevel);
    for j = 1:numOfNeighbors
        endNodes( (j-1)*discretizedLevel+1: (j*discretizedLevel) ) = (tableIdx(i,j)-1)*discretizedLevel+1 : tableIdx(i,j)*discretizedLevel ;   
    end
    
    [xMesh, yMesh] = meshgrid( startNodes, endNodes);
    
    xMesh = xMesh(:); yMesh = yMesh(:);
    adjanceyMatrix = sum((nodes(:,xMesh) - nodes(:,yMesh) ).^2 , 1);
    
    offset = numOfNeighbors * discretizedLevel * discretizedLevel ;
    offset = ((i-1)*offset + 1) : (i * offset);
    edges( offset , 1) = xMesh;
    edges( offset , 2) = yMesh;
    
    edgesCost(offset) = adjanceyMatrix;        
end

edges = sort(edges, 2);

[edges, idx, ~] = unique(edges, 'rows');
edgesCost = edgesCost(idx);

% dist = sum((nodes(:,edges(:,1)) - nodes(:,edges(:,2)) ).^2 , 1); % dist
% should be equal to edgesCost




