function saveComputedPos(verticesFileName, outputPath, discretizedLevel,cams, near, far)

% verticesFileName = '58inst3480.clu.vertices';

computedNodes = dlmread(verticesFileName);
camConnect = floor(computedNodes/discretizedLevel) + 1;
camConnect = camConnect';
theNodes = mod( unique(computedNodes(:)), discretizedLevel) + 1;
calculatedPos = getComputed3DPos(cams, theNodes, discretizedLevel, near, far);

save(fullfile(outputPath, 'computedPos.mat'), 'camConnect', 'calculatedPos');