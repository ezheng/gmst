function saveComputedPos(verticesFileName, outputPath, discretizedLevel,cams, near, far, subsetId)

% verticesFileName = '58inst3480.clu.vertices';

computedNodes = dlmread(verticesFileName);
camConnect = floor(computedNodes/discretizedLevel) + 1;
camConnect = camConnect';
theNodes = mod( unique(computedNodes(:)), discretizedLevel) + 1;
if(nargin < 7)
    calculatedPos = getComputed3DPos(cams, theNodes, discretizedLevel, near, far);
else
    calculatedPos = getComputed3DPos(cams(subsetId), theNodes, discretizedLevel, near(subsetId), far(subsetId));
end

save(fullfile(outputPath, 'computedPos.mat'), 'camConnect', 'calculatedPos');