%  prepare the camera parameters
% 
function mainKillDevil(numOfCameras, discretizedLevel, timeLimit, workingPathHead, isDraw)

% usage: mainKillDevil(20, 5, 20, '~/Enliang/matlab/gmst/', true)


workingDir = fullfile(workingPathHead, ['task','_time',num2str(timeLimit)]...
    , ['task_', num2str(numOfCameras)] );
taskName = ['task', num2str(numOfCameras)];
knownOrder = true;
%timeLimit = 100;
%discretizedLevel = 10;
%near = 18; far = 28;
if(nargin == 4)
    isDraw = false;
end
rng default;
%==========================================================================
if(knownOrder)
    cd ../cvx;
    cvx_setup;
    cd ../batchProcess;
end
addpath('./generateSimulatedData');
[near, far] = generateSimulatedData(numOfCameras, workingDir, taskName);
save(fullfile(workingDir, 'dataRangeNearFar.mat'), 'near', 'far');
rmpath('./generateSimulatedData');
% =========================================================================



% % ----------------------------------------------------------
matFileFull = fullfile(workingDir, [taskName,'.mat']);
if(~exist( matFileFull, 'file'))
    measurement2DFileName = fullfile(workingDir, 'pointsPos.mat');
    nvmFileName = fullfile(workingDir, [taskName, '.nvm']);
    load(measurement2DFileName);
    [camera, points3D] = extractNVMData(nvmFileName);
    save(matFileFull);
else
    load(matFileFull, 'pointsPos', 'camera', 'points3D');
end

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

for i = 1:numel(C)    
    if(C{i}.detected == true)
        p = C{i}.C; p = [p, p + C{i}.ori * (near(i)+far(i))/2];
        if(isDraw)
            hold on; plot3(p(1,:), p(2,:), p(3,:)); hold off;
        end
    end
end

addpath('generateGMSTData');
verticeFilePath = ['../','task','_time', num2str(timeLimit)];
verticesFileName = fullfile(verticeFilePath, [num2str(numel(C)), 'inst',...
    num2str(numel(C) * discretizedLevel), '.clu.vertices']);

generateGMSTData(workingDir, timeLimit, near, far, discretizedLevel, 'task');
rmpath('generateGMSTData');

% run gmst
% addpath( '../codigo_gmst/');
dataFile = fullfile(['../', 'task', '_time',num2str(timeLimit), '/'], [num2str(numOfCameras), 'all', num2str(numOfCameras * discretizedLevel), '.dat']);
system(['../codigo_gmst/gmst -all ', dataFile]);
% rmpath( '../codigo_gmst/');

saveComputedPos(verticesFileName, workingDir, discretizedLevel, C, near, far);

% save mat file
computedPosFileName = fullfile(workingDir, 'computedPos.mat');
load(computedPosFileName);
if(isDraw)      %plot the unordered results
    for i = 1: size(camConnect,1)
        pts = calculatedPos(:, camConnect(i,:));
        figure(h); hold on; plot3(pts(1,:), pts(2,:), pts(3,:), 'g*--','MarkerSize',5); hold off;
    end       
end

if(knownOrder)
    cost = computeCost(Pts3d); % The cost of results that are computed with given order
    fprintf(1, 'cost with known order is: %f\n', cost ); 
end

costComputedPos = computeCost(calculatedPos');
fprintf(1, 'cost with unknown order is %f\n', costComputedPos);

groundTruthFile = fullfile(workingDir, 'GT3DPoints.mat');
if( exist(groundTruthFile, 'file'))
   gt = load(groundTruthFile); 
   cost = computeCost(gt.points3D');
   fprintf(1, 'cost of groundTruth is: %f\n', cost ); 
    if(isDraw)   
       figure(h); hold on; plot3(gt.points3D(1,:), gt.points3D(2,:), gt.points3D(3,:), 'r*--','MarkerSize',5); hold off;      
    end    
end


