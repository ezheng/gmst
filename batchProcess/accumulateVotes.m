function table = accumulateVotes(table, camConnect, subsetId)

source = unique(camConnect);
myGraph =  sparse( camConnect(:,1), camConnect(:,2) ,1, numel(source), numel(source) );
myGraph = myGraph + myGraph';


for i = 1:numel(source)
    dist = graphshortestpath(myGraph, source(i));
    realDist = zeros(1, size(table, 2));
    
    realDist(subsetId) = dist;    
    table( subsetId(i) ,:) = table( subsetId(i), : ) + realDist;
end






















