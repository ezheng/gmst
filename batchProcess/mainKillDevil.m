%  prepare the camera parameters
% 
function conditionNum =  mainKillDevil(numOfCameras, discretizedLevel, timeLimit, workingPathHead, isDraw, weight)

% usage: mainKillDevil(20, 5, 20, '~/Enliang/matlab/gmst/', true)
if(nargin == 0)
    numOfCameras = 100; 
    discretizedLevel = 5;
    timeLimit = 200;
    workingPathHead = 'F:\Enliang\matlab\GMST_subprob\gmst';
    isDraw = true;
    weight = 15;
end

% close all
workingDir = fullfile(workingPathHead, ['task','_time',num2str(timeLimit)]...
    , ['task_', num2str(numOfCameras)] );
taskName = ['task', num2str(numOfCameras)];
knownOrder = false;
%timeLimit = 100;
%discretizedLevel = 10;
%near = 18; far = 28;
if(nargin == 4)
    isDraw = false;
end
% rng(150,'v5uniform');
%==========================================================================
if(knownOrder)
    cd ../cvx;
    cvx_setup;
    cd ../batchProcess;
end
addpath('./generateSimulatedData');
[near, far, orientation] = generateSimulatedData(numOfCameras, workingDir, taskName);


save(fullfile(workingDir, 'dataRangeNearFar.mat'), 'near', 'far');
rmpath('./generateSimulatedData');
% =========================================================================



% % ----------------------------------------------------------
matFileFull = fullfile(workingDir, [taskName,'.mat']);
% if(~exist( matFileFull, 'file'))
    measurement2DFileName = fullfile(workingDir, 'pointsPos.mat');
    nvmFileName = fullfile(workingDir, [taskName, '.nvm']);
    load(measurement2DFileName);
    [camera, points3D] = extractNVMData(nvmFileName);
    save(matFileFull);
% else
%     load(matFileFull, 'pointsPos', 'camera', 'points3D');
% end

% draw all the cameras and 3d points
if(isDraw)
    h = figure(); clf; 
    draw3dPoints(points3D, h); xlabel('x'), ylabel('y'), zlabel('z'); 
    hold on; drawAllCams(camera, h);
end

% if(knownOrder)
for iCamera = 1: numel(camera)
    cameraName = camera(iCamera).fileName;
    [ ~, name, ext] = fileparts(cameraName);
    T{iCamera} = str2double(name(isstrprop(name,'digit')));     
end
time = [T{:}];
time = time - min(time);
[~, sortId] = sort(time);
camera = camera(sortId);
time = (1:length(time)) - 1; 
id = 1;
for iCamera = 1 :numel(camera)
    
    C{id}.R = camera(iCamera).R(1:3,1:3);
  
    C{id}.P = camera(iCamera).K * [C{id}.R, camera(iCamera).T]; 
    C{id}.T = camera(iCamera).T;
    
%     C{id}.t = time(iCamera);
    C{id}.t = id;   % This is the order of each camera
    C{id}.K = camera(iCamera).K;

    C{id}.C = camera(iCamera).C;
    
%     C{iCamera}.m = pointsPos(C{iCamera}.t+1,:);
    C{id}.m = pointsPos(iCamera,:);
    if(C{id}.m(1) == 0 && C{id}.m(2) == 0)
        C{id}.detected = false;
    else
        C{id}.detected = true;
    end
    id = id +1;
end
C = getOrientation(C);
CC = [C{:}]; ind = ([CC.detected] == true);
C = C(ind);
save(fullfile(workingDir, 'C.mat'), 'C');



if(knownOrder)
    
    [Traj, Traj2] = TrajectoryReconstruction(C); % Reconstruction w/o predefined number of basis    
    Pts3d = Traj{1};
    range = 1:size(Pts3d,1);
    if(isDraw)
        figure(h);
        hold on; plot3(Pts3d(range,1), Pts3d(range,2), Pts3d(range,3), 'k*'); hold off;
    end
end

% perpendicularDir = computePerpendicularDir(orientation, C);
% L1pts = L1NormMethod(C, perpendicularDir);
% L1pts = L1NormMethod(C, orientation);
%  figure(h); hold on; plot3(L1pts(1,:), L1pts(2,:), L1pts(3,:), 'r*','MarkerSize',5); hold off;  
 
for i = 1:numel(C)    
    if(C{i}.detected == true)
        p = C{i}.C; p = [p, p + C{i}.ori * (near(i)+far(i)+3)/2];
        if(isDraw)
            hold on; plot3(p(1,:), p(2,:), p(3,:)); hold off;
        end
    end
end

addpath('generateGMSTData');
verticeFilePath = ['../','task','_time', num2str(timeLimit)];
verticesFileName = fullfile(verticeFilePath, [num2str(numel(C)), 'inst',...
    num2str(numel(C) * discretizedLevel), '.clu.vertices']);

generateGMSTData(workingDir, timeLimit, near, far, discretizedLevel, 'task',0,0,orientation, weight);
% generateGMSTData(workingDir, timeLimit, near, far, discretizedLevel, 'task');
rmpath('generateGMSTData');

% run gmst
% addpath( '../codigo_gmst/');
% dataFile = fullfile(['../', 'task', '_time',num2str(timeLimit), '/'], [num2str(numOfCameras), 'all', num2str(numOfCameras * discretizedLevel), '.dat']);
dataFile = fullfile( fullfile( workingPathHead, ['task', '_time',num2str(timeLimit)])  , [num2str(numOfCameras), 'all', num2str(numOfCameras * discretizedLevel), '.dat']);
% system(['..\codigo_gmst\gmst -all ', dataFile]);
% rmpath( '../codigo_gmst/');

saveComputedPos(verticesFileName, workingDir, discretizedLevel, C, near, far);

% save mat file
computedPosFileName = fullfile(workingDir, 'computedPos.mat');
load(computedPosFileName);
if(isDraw)      %plot the unordered results
    for i = 1: size(camConnect,1)
        pts = calculatedPos(:, camConnect(i,:));
        figure(h); hold on; plot3(pts(1,:), pts(2,:), pts(3,:), 'k*--','MarkerSize',5); hold off;
    end       
end

% camConnect_sequence = [[1:numel(C)-1]', [2:numel(C)]'];
[calculatedPos_refined, conditionNum] = ReconstructPointTrajectory_SumOfNorm_direction(C, camConnect, orientation, calculatedPos, weight);
return;
% calculatedPos_refined = ReconstructPointTrajectory_SumOfNorm_direction(C, camConnect, orientation);
if(isDraw)      %plot the unordered results
    for i = 1: size(camConnect,1)
        pts = calculatedPos_refined(:, camConnect(i,:));
        figure(h); hold on; plot3(pts(1,:), pts(2,:), pts(3,:), 'b*--','MarkerSize',5); hold off;
    end       
end

if(knownOrder)
    cost = computeCost(Pts3d); % The cost of results that are computed with given order
    fprintf(1, 'cost with known order is: %f\n', cost ); 
end

costComputedPos = computeCost(calculatedPos', camConnect);
fprintf(1, 'cost with unknown order is %f\n', costComputedPos);



groundTruthFile = fullfile(workingDir, 'GT3DPoints.mat');
if( exist(groundTruthFile, 'file'))
   gt = load(groundTruthFile); 
   cost = computeCost(gt.points3D');
   fprintf(1, 'cost of groundTruth is: %f\n', cost ); 
    if(isDraw)   
       figure(h); hold on; plot3(gt.points3D(1,:), gt.points3D(2,:), gt.points3D(3,:), 'r*','MarkerSize',5); hold off;      
    end    
end


