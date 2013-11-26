function visualization(numOfCameras, discretizedLevel, timeLimit, workingPathHead, taskName)
if(nargin == 0)
    numOfCameras = 60;
    discretizedLevel = 50;
    timeLimit = 30000;
    workingPathHead = '../';
    taskName = 'task';
end

workingDir = fullfile(workingPathHead, ...
    [taskName, '_time', num2str(timeLimit)], [taskName, '_', num2str(numOfCameras)]);

h = figure();


% plot camera
matFileFull = fullfile(workingDir, [taskName, num2str(numOfCameras),'.mat']);
 load(matFileFull, 'pointsPos', 'camera', 'points3D');

% draw all the cameras and 3d points

figure(h); clf;
draw3dPoints(points3D, h); xlabel('x'), ylabel('y'), zlabel('z');
hold on; drawAllCams(camera, h);


% plot ground truth
groundTruthFile = fullfile(workingDir, 'GT3DPoints.mat');
if( exist(groundTruthFile, 'file'))
   gt = load(groundTruthFile); 
%    cost = computeCost(gt.points3D');
%    fprintf(1, 'cost of groundTruth is: %f\n', cost ); 
    
    figure(h); hold on; plot3(gt.points3D(1,:), gt.points3D(2,:), gt.points3D(3,:), 'r*','MarkerSize',5); hold off;      
    
end

% plot the gmst results
computedPosFileName = fullfile(workingDir, 'computedPos.mat');
load(computedPosFileName)
for i = 1: size(camConnect,1)
        pts = calculatedPos(:, camConnect(i,:));
        figure(h); hold on; plot3(pts(1,:), pts(2,:), pts(3,:), 'g*--','MarkerSize',5); hold off;
end

view([-135.1,23.5]); set(h, 'Units', 'Normalized', 'OuterPosition', [0 0 1 1]);
zoom(1.2);
saveas(h,  fullfile(workingDir, ['output_', num2str(timeLimit), '.jpg']))

