function annotate()

% workingDirWithoutTask = 'F:\Enliang\matlab\GMST_subprob\gmst\batchProcess\labelData\realData\brook3';
workingDir = 'F:\Enliang\matlab\GMST_subprob\gmst\realData\mixOneLane2\';
targetName = 'mixOneLane2';
% --------------------------------------------------------------------------------------
fullNVMFileName = fullfile(workingDir, [targetName, '.nvm']);

fullDataFileName = fullfile(workingDir, [targetName, '.mat']);
% if(~exist(fullDataFileName, 'file'))
    [camera, points3D] = extractNVMData(fullNVMFileName);
% else
%     g = load(fullDataFileName); 
%     camera = g.camera; points3D = g.points3D;
% end

% allImages = dir(fullfile(workingDir, 'image', '*.jpg'));


% pointsPos = zeros( numel(allImages), 2);
duplication = zeros( numel(camera), 1);
pointsPos = [];

fprintf(1, 'left button -- select points\n' );
fprintf(1, 'middle button -- descelect all points\n');
fprintf(1, 'right button -- done with current image\n');
for i = 1:numel(camera)
   fullImgName = camera(i).fileName;
   
   img = imread(fullImgName);
   figure(1);
   imagesc(img); axis equal
   
   newPoints = [];
   mm = 0;   
   while(true)
       if( ~isempty(newPoints) )
           hold on;  plot( newPoints(:,1), newPoints(:,2), 'b*' ,  'MarkerSize',10 );   hold off;       
       else
           imagesc(img); axis equal;
       end
       [x,y,button] = ginput(1);
       if(button == 3)
           break;
       elseif(button == 2)
           mm = 0;
           newPoints = [];
           continue;
       end
       mm = mm + 1;
       newPoints = [newPoints; [x, y]];       
   end 
   pointsPos = [pointsPos;newPoints];
   duplication(i) = mm;
end
save xxx.mat;
idx = 1;
newCamera = struct('K', [], 'R', [], 'C', [], 'T', [], 'fileName', []);
newCamera = repmat(newCamera, size(pointsPos, 1), 1 );
for i = 1:size(duplication,1)
    newCamera(idx:idx+duplication(i)-1) = camera(i);
    idx = idx + duplication(i);
end
camera = newCamera;

save(fullfile(workingDir, 'pointsPos.mat'), 'pointsPos');
save(fullfile(workingDir, [targetName, '.mat']), 'pointsPos', 'camera', 'points3D' );



