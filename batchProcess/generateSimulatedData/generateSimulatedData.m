function [near, far, orientation] = generateSimulatedData(numOfCameras, workingPath, taskName)


if(~exist(workingPath, 'dir'))
    mkdir(workingPath);
end

% The number of cameras is the same to the number 3d points

[points3D, singleOrientation, orientation] = generate3DPoints(numOfCameras, 1, true);
% [points3D, orientation] = generate3DPoints_multipleLanes(numOfCameras, 1, false);
% [points3D,orientation] = generate3DPoints_oneLanes(numOfCameras, 1, true);
% [points3D, orientation] = generate3DPoints_Circle(numOfCameras, 1, true);
% [points3D, orientation] = generate3DPoints_Circle_Quantized(numOfCameras, 1, true);
% [points3D, orientation] = generate3DPoints_Xshape(numOfCameras, 1, true);
% [points3D, orientation] = generate3DPoints_SineShape(numOfCameras, 1, true);

save(fullfile(workingPath, 'GT3DPoints.mat'), 'points3D');

% generate camera poses and 2d measures, and images
%[camera, near, far] = generateCamPoses(numOfCameras, points3D);

% ----------------------------------------
singleOrientation = orientation(1:3,1);
% ----------------------------------------
[camera, near, far] = generateCamPoses_Inplane(numOfCameras, points3D,singleOrientation);

% write NVM files
NVMFileName = fullfile(workingPath, [taskName, '.nvm']);
writeNVM(NVMFileName, camera, []);
allMeasure = [camera.measure];
pointsPos = reshape(allMeasure, 2, []);
pointsPos = pointsPos';
save(fullfile(workingPath, 'pointsPos.mat'), 'pointsPos');

% write images
imageFilePath = fullfile(workingPath, 'image');
if(~exist( imageFilePath, 'dir'))
    mkdir(imageFilePath);
end
writeImages(camera, imageFilePath);

% write a pcdb file
pcdbFileName = fullfile(workingPath, 'pcdb.txt');
writePCDBFile(pcdbFileName, workingPath);














