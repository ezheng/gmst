function generateGMSTData(workingDir, timeLimit, near, far, discretizedLevel)


% workingDir = 'F:\Enliang\matlab\singleView_NSFM\data\task_100';

load(fullfile(workingDir, 'C.mat'));
numOfCams = numel(C);

% outputPath = '/home/ezheng/Enliang/matlab/gmst/data/';
outputPath = fullfile(workingDir, '..');
% ===================================================================
cams = C(1:numOfCams);
writeCLU_COO_File(cams, numOfCams, near, far, discretizedLevel, outputPath, timeLimit);

