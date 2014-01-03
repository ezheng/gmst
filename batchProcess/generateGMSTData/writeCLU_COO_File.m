function writeCLU_COO_File(cams, numOfCams, near, far, discretizedLevel, outputPath, timeLimit, taskName)

% load C.mat;
% numOfCams = 58;
% discretizedLevel = 40;
% near = 0; far = 12;
% outputPath = 'F:\Enliang\virtualMachine\shareFolder\gmst\data\';
% timeLimit is the time threshold (in seconds) for the gmst application 
% -----------------------------------------------------

% cams = C(1:numOfCams);
outputFileName =fullfile(outputPath, [num2str(numOfCams),'inst', num2str(numOfCams * discretizedLevel) , '.clu']);

nodes = getNodes3dPos(cams, discretizedLevel, near, far);
nodes = nodes * 100;
[edges, edgesCost] = computeEdges(nodes, discretizedLevel);
edgesCost = round(edgesCost);
% first write cluster ID

clusterID = [1:numOfCams];
clusterID = repmat( clusterID, discretizedLevel,1);
clusterID = clusterID(:);

% write edge
edgesAndCost = [edges-1, edgesCost];

addpath('F:\Enliang\matlab\GMST_subprob\gmst\batchProcess\mex\build64\Debug');
% edgesAndCost = 
removeEdges(int32(clusterID), int32(edgesAndCost), outputFileName);
rmpath('F:\Enliang\matlab\GMST_subprob\gmst\batchProcess\mex\build64\Debug');

% fid = fopen(outputFileName,'w');
% assert(fid>0)
% fprintf(fid,'%d\n', clusterID);
% fprintf(fid, '\n');
% tic
% for i=1:size(edgesAndCost, 1)
%     fprintf(fid, '%d ', edgesAndCost(i,:));
%     fprintf(fid, '\n');
% end
% toc
% fclose(fid);

% tic
% dlmwrite(outputFileName, int64(edgesAndCost), 'delimiter', ' ', '-append', 'precision', '%d');
% toc
 
% % %----------------------------------- 
outputFileName = fullfile(outputPath, [num2str(numOfCams),'inst', num2str(numOfCams * discretizedLevel) , '.coo']);
nodes = nodes';
pos = nodes(:,[1,3]);
pos = pos - min(pos(:));
pos = round(pos/max(pos(:)) * 10000);
fid = fopen(outputFileName, 'w');
assert(fid > 0);
for i = 1:size(pos, 1)
    fprintf(fid, '%d ', pos(i,:));
    fprintf(fid, '\n');
end
fclose(fid);


% --------------------------------------
fid = fopen(fullfile(outputPath, [num2str(numOfCams), 'all', num2str(numOfCams * discretizedLevel), '.dat']), 'w');
cluFileName = [num2str(numOfCams),'inst', num2str(numOfCams * discretizedLevel) , '.clu'];
fprintf(fid, '%s 1 %d ../%s', cluFileName, timeLimit,taskName);
fclose(fid);








