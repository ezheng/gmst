%  prepare the camera parameters
% 
function mainKillDevil_realData( timeLimit, workingPathHead, isDraw)

%numOfCameras = 100;
% close all;
workingPathHead = '/home/ezheng/Enliang/matlab/gmst/';
taskName = 'brook3';
timeLimit = 500;

knownOrder = true;

discretizedLevel = 30;
near = 5; far = 12;
isDraw = true;   
rng default;
originalDir = fullfile(workingPathHead, 'realData', taskName);
% ============================================================
matFileFull = fullfile(originalDir, [taskName,'.mat']);
if(~exist( matFileFull, 'file'))
    measurement2DFileName = fullfile(originalDir, 'pointsPos.mat');
    nvmFileName = fullfile(originalDir, [taskName, '.nvm']);
    load(measurement2DFileName);
    [camera, points3D] = extractNVMData(nvmFileName);
    save(matFileFull);
    elsel
    load(matFileFull, 'pointsPos', 'camera', 'points3D');
end

% % ----------------------------------------------------------
numOfCams = numel(camera);
if(~exist(  fullfile( workingPathHead, [taskName, '_time',num2str(timeLimit)] ),'dir')  )
    mkdir( fullfile( workingPathHead, [taskName, '_time',num2str(timeLimit) ]) );
end
workingDir = fullfile( workingPathHead, [taskName, '_time',num2str(timeLimit)], [taskName, '_', num2str(numOfCams) ]) ;
copyfile( originalDir , workingDir );

% % ----------------------------------------------------------

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

numOfCameras = numel(C);
near = ones(1, numOfCameras) * near;
far = ones(1,numOfCameras) * far;

for i = 1:numel(C)    
    if(C{i}.detected == true)
        p = C{i}.C; p = [p, p + C{i}.ori * (near(i)+far(i))/2];
        if(isDraw)
            hold on; plot3(p(1,:), p(2,:), p(3,:)); hold off;
        end
    end
end

addpath('generateGMSTData');
generateGMSTData(workingDir, timeLimit, near, far, discretizedLevel);
rmpath('generateGMSTData');

% run gmst
% addpath( '../codigo_gmst/');

% gmstFilePath = 
dataFile = fullfile( fullfile( workingPathHead, [taskName, '_time',num2str(timeLimit)])  , [num2str(numOfCameras), 'all', num2str(numOfCameras * discretizedLevel), '.dat']);
system(['../codigo_gmst/gmst -all ', dataFile]);
% rmpath( '../codigo_gmst/');


verticesFileName = fullfile(fullfile( workingPathHead, [taskName, '_time',num2str(timeLimit)]), ...
    [num2str(numel(C)), 'inst', num2str(numel(C) * discretizedLevel), '.clu.vertices']);
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


