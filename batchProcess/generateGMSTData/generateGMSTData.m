function generateGMSTData(workingDir, timeLimit, near, far, discretizedLevel, taskName, subsetId, subProbTimeLimit, twoOrientation, weight)


% workingDir = 'F:\Enliang\matlab\singleView_NSFM\data\task_100';

load(fullfile(workingDir, 'C.mat'));
numOfCams = numel(C);

% outputPath = '/home/ezheng/Enliang/matlab/gmst/data/';

% ===================================================================
cams = C(1:numOfCams);

% subProbTimeLimit = 5;

if( nargin == 8)
    outputPath = fullfile(workingDir, '..', ['sub_time', num2str(subProbTimeLimit)] );
    if( ~exist(outputPath, 'dir'))
        mkdir(outputPath);
    end
    timeLimit = subProbTimeLimit;
    writeCLU_COO_File(cams(subsetId), numOfCams, near(subsetId), far(subsetId), discretizedLevel,...
        outputPath, timeLimit, fullfile(workingDir,'..','sub') );
elseif(nargin == 6)
    outputPath = fullfile(workingDir, '..');
%     writeCLU_COO_File(cams, numOfCams, near, far, discretizedLevel, outputPath, timeLimit, taskName);
    writeCLU_COO_File(cams, numOfCams, near, far, discretizedLevel, outputPath, timeLimit, fullfile( workingDir,'../..', taskName) );
elseif(nargin == 10)
    outputPath = fullfile(workingDir, '..');
%     writeCLU_COO_File(cams, numOfCams, near, far, discretizedLevel, outputPath, timeLimit, taskName);
    writeCLU_COO_File_orientated(cams, numOfCams, near, far, discretizedLevel, outputPath, timeLimit, fullfile( workingDir,'../..', taskName), twoOrientation, weight );
end
