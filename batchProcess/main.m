%  prepare the camera parameters
% 
close all; clear; 
% workingDir = 'F:\Enliang\matlab\singleView_NSFM\data\jp';
% taskName = 'jp';
% workingDir = 'F:\Enliang\matlab\singleView_NSFM\data\brook3';
% taskName = 'brook3';
numOfCameras = 100;
workingDir = fullfile('/home/ezheng/Enliang/matlab/gmst/data/', ['task_', num2str(numOfCameras)] );
taskName = ['task', num2str(numOfCameras)];
knownOrder = false;
timeLimit = 100;
discretizedLevel = 10;
near = 18; far = 28;
isDraw = true;

rng('default');

%==========================================================================
% cd('./generateSimulatedData');
addpath('./generateSimulatedData');
generateSimulatedData(numOfCameras, workingDir, taskName);
rmpath('./generateSimulatedData');
% cd('..');

% % ----------------------------------------------------------
matFileFull = fullfile(workingDir, [taskName,'.mat']);
% if(~exist( matFileFull, 'file'))
    measurement2DFileName = fullfile(workingDir, 'pointsPos.mat');
    nvmFileName = fullfile(workingDir, [taskName, '.nvm']);
    load(measurement2DFileName);
    [camera, points3D] = extractNVMData(nvmFileName);
    save(matFileFull);
% else
%     load(matFileFull);
% end

if(isDraw)
    h = figure(); clf; 
    draw3dPoints(points3D, h); xlabel('x'), ylabel('y'), zlabel('z');
    hold on; drawAllCams(camera, h); hold off;
end

% if(knownOrder)
for iCamera = 1: numel(camera)
    cameraName = camera(iCamera).fileName;
    [ ~, name, ext] = fileparts(cameraName);
    T{iCamera} = str2double(name(isstrprop(name,'digit')));     
end
time = [T{:}];
xx = min(time);
time = time - min(time);
[~, sortId] = sort(time);
% pointsPos = pointsPos(time+1,:);
camera = camera(sortId);
time = (1:length(time)) - 1; 
id = 1;
for iCamera = 1 :numel(camera)
    
    C{id}.R = camera(iCamera).R(1:3,1:3);
  
    C{id}.P = camera(iCamera).K * [C{id}.R, camera(iCamera).T]; 
    C{id}.T = camera(iCamera).T;
    
%     C{id}.t = time(iCamera);
    C{id}.t = id;
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
    % for i = 1:30
    if(C{i}.detected == true)
        p = C{i}.C; p = [p, p + C{i}.ori * 30];
        if(isDraw)
            hold on; plot3(p(1,:), p(2,:), p(3,:)); hold off;
        end
    end
end

% cd 'generateGMSTData';
addpath('./generateGMSTData');
verticeFilePath = '../codigo_gmst/plot/';
verticesFileName = fullfile(verticeFilePath, [num2str(numel(C)), 'inst',...
    num2str(numel(C) * discretizedLevel), '.clu.vertices']);
generateGMSTData(workingDir, timeLimit, near, far, discretizedLevel);
rmpath('./generateGMSTData');

% run gmst
cd '../codigo_gmst/';
dataFile = fullfile('../data/', [num2str(numOfCameras), 'all', num2str(numOfCameras * discretizedLevel), '.dat']);
system(['./gmst -all ', dataFile]);
cd '../batchProcess/';
saveComputedPos(verticesFileName, workingDir, discretizedLevel, C, near, far);

% save mat file
computedPosFileName = fullfile(workingDir, 'computedPos.mat');

load(computedPosFileName);

if(isDraw)
    for i = 1: size(camConnect,1)
        pts = calculatedPos(:, camConnect(i,:));false
        figure(h); hold on; plot3(pts(1,:), pts(2,:), pts(3,:), 'g*--','MarkerSize',5); hold off;
    end
end
% Pts3d = Traj2{1};
% range = 1:size(Pts3d,1);
% figure(h); hold on; plot3(Pts3d(range,1), Pts3d(range,2), Pts3d(range,3), 'g*'); hold off;
% fprintf('black is cvx; green is DCT');
if(knownOrder)
 cost = computeCost(Pts3d);
end

groundTruthFile = fullfile(workingDir, 'GT3DPoints.mat');
if( exist(groundTruthFile, 'file'))
   gt = load(groundTruthFile); 
   cost = computeCost(gt.points3D');
    if(isDraw)   
       figure(h); hold on; plot3(gt.points3D(1,:), gt.points3D(2,:), gt.points3D(3,:), 'r*--','MarkerSize',5); hold off;
    end
end


% Id = 57;
% img = imread(camera(Id).fileName);
% groundTruth = pointsPos(Id,:);
% recoveredResults = C{Id}.P * [Pts3d(Id,:)';1];
% recoveredResults = recoveredResults(1:2)/recoveredResults(3);
% plotProjectedPoints(img, groundTruth, recoveredResults)



% t1 = 23; t2 = t1 + 10;
% 
%  F = fundfromcameras(C{t1}.P, C{t2}.P);
%  img1 = imread(camera(t1).fileName);
%  img2 = imread(camera(t2).fileName);
%  vgg_gui_F(img2, img1, F);
 
 
 