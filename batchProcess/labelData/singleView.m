function singleView

% visualize the object position based on the window size. Beta is the
% parameter to be determined.

workingDir = 'F:\Enliang\data\singleView\brook2';
targetName = 'brook2';
outputTag = '_manualLabel';
% --------------------------------------------------------------------
addpath F:\Enliang\library_64\export_fig;
fullNVMFileName = fullfile(workingDir, [targetName,'.nvm']);
fullDataFileName = fullfile(workingDir, [targetName, '.mat']);
if(~exist(fullDataFileName, 'file'))
    [standardCams, points3D] = extractNVMData(fullNVMFileName);
else
    load(fullDataFileName)
end

% save output
outputDir = fullfile(workingDir, ['people3dScene', outputTag]);
if(~exist(outputDir, 'dir'))
    mkdir(outputDir);
end


for beta = 0.02:0.02:2    % beta is actually the physical size of the people in the model
    
    h = figure('Position', [-100, 00, 1600, 800]);
    view(-2,-30); zoom(2); campos([-17.0286 -503.6472 -278.6282]);
    drawAllCams(standardCams, h);
    draw3dPoints(points3D, h)       
    detectionFolder = fullfile(workingDir, ['peopleDetection',outputTag]);    
    
    load(fullfile(outputDir, [sprintf('%f', beta),'.mat']));
    t = -15:0.2:15;
    pt3d = repmat(middlePoint, 1, numel(t) ) + lineDir * t;
    hold on; plot3(pt3d(1,:), pt3d(2,:), pt3d(3,:) ); hold off;

%      allPeople3DPos = [];
    for camId = 1:numel(standardCams)

        K = standardCams(camId).K; R = standardCams(camId).R; T = standardCams(camId).T;        
%         figure(h+1); theImage = imread(standardCams(camId).fileName); imagesc(theImage);        
        invK = inv(K);
        R = R(1:3,1:3);        
        
        [~,name,~] = fileparts(standardCams(camId).fileName);
        load( fullfile(detectionFolder, [name, '.mat'] ));       
        numOfDetections = size(PedPosition,1);
        
        t = [-15:0.02:15];        
        
        for i = 1:numOfDetections
            pos2d = [mean(PedPosition(i,1:2)), mean(PedPosition(i, 3:4))]';            
            deltaX = 0;
            deltaY = abs(PedPosition(i,4) - PedPosition(i,3));
            depth = beta / norm(invK * [deltaX,deltaY,0]');
           
            people3dPos = compute3dPointsPos(pos2d, K, R, T, depth);
%             allPeople3DPos = [allPeople3DPos, people3dPos];
            hold on; plot3(people3dPos(1), people3dPos(2), people3dPos(3), 'ks', 'MarkerSize',4, 'MarkerFaceColor','k' ); hold off;      
        end
    end
%     outputDirFileName = fullfile(outputDir, [sprintf('%f', beta),'.mat']);
%     save(outputDirFileName, 'allPeople3DPos');
    outputDirFileName = fullfile(outputDir, [sprintf('%f', beta),'.jpg']);
    export_fig(outputDirFileName);
    close(h);
end

