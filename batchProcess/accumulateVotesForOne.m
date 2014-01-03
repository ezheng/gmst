function table = accumulateVotesForOne(camConnect, subsetId, table, imageId)


source = unique(camConnect);
myGraph =  sparse( camConnect(:,1), camConnect(:,2) ,1, numel(source), numel(source) );
myGraph = myGraph + myGraph';

idx = find(subsetId == imageId);

dist = graphshortestpath(myGraph, idx );
% realDist = zeros(1, size(table, 2));

table( subsetId(idx), subsetId ) =  dist;




















