function generateGMSTData(workingDir, timeLimit, near, far, discretizedLevel, taskName, subsetId)


% workingDir = 'F:\Enliang\matlab\singleView_NSFM\data\task_100';

load(fullfile(workingDir, 'C.mat'));
numOfCams = numel(C);

% outputPath = '/home/ezheng/Enliang/matlab/gmst/data/';
outputPath = fullfile(workingDir, '..');
% ===================================================================
cams = C(1:numOfCams);

if( nargin == 7)
    writeCLU_COO_File(cams(subsetId), numOfCams, near(subsetId), far(subsetId), discretizedLevel, outputPath, timeLimit, taskName);
else
    writeCLU_COO_File(cams, numOfCams, near, far, discretizedLevel, outputPath, timeLimit, taskName);
end
