function runOnPartialGraph(workingDir, timeLimit, near, far, discretizedLevel, table, C, numOfCameras, isDraw,h)

 addpath('generateGMSTData');
 generateGMSTDataPartial(workingDir, timeLimit, near, far, discretizedLevel, 'task', table);
 rmpath('generateGMSTData');
        
 verticeFilePath = ['../','task','_time', num2str(timeLimit)];
 verticesFileName = fullfile(verticeFilePath, [num2str(numel(C)), 'inst',...
 num2str(numel(C) * discretizedLevel), '.clu.vertices']);
 
% run gmst
dataFile = fullfile(['../', 'task', '_time',num2str(timeLimit), '/'], [num2str(numOfCameras), 'all', num2str(numOfCameras * discretizedLevel), '.dat']);
tic;
system(['..\\codigo_gmst\\gmst.exe -all ', dataFile])   %windows
fprintf(1,'it takes %f second\n', toc);

% rmpath( '../codigo_gmst/');
saveComputedPos(verticesFileName, workingDir, discretizedLevel, C, near, far);

computedPosFileName = fullfile(workingDir, 'computedPos.mat');
load(computedPosFileName);
if(true)      %plot the unordered results
    for i = 1: size(camConnect,1)
        pts = calculatedPos(:, camConnect(i,:));
        figure(h); hold on; plot3(pts(1,:), pts(2,:), pts(3,:), 'b*--','MarkerSize',5); hold off;
    end
end