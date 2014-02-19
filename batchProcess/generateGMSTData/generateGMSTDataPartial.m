function generateGMSTDataPartial(workingDir, timeLimit, near, far, discretizedLevel, taskName, table)

load(fullfile(workingDir, 'C.mat'));
numOfCams = numel(C);

% outputPath = '/home/ezheng/Enliang/matlab/gmst/data/';
outputPath = fullfile(workingDir, '..');
% ===================================================================
cams = C(1:numOfCams);

writeCLU_COO_FilePartial(cams, numOfCams, near, far, discretizedLevel, outputPath, timeLimit, taskName, table);























