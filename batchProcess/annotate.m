function annotate()

workingDirWithoutTask = 'F:\Enliang\matlab\singleView_NSFM\data\brook3';
workingDir = 'F:\Enliang\matlab\singleView_NSFM\data\brook3\image';

allImages = dir(fullfile(workingDir, '*.jpg'));


pointsPos = zeros( numel(allImages), 2);

for i = 1:numel(allImages)
   imgName = allImages(i).name;
   fullImgName = fullfile(workingDir, imgName);
   img = imread(fullImgName);
   figure(1);
   imagesc(img); axis equal
   [x,y,button] = ginput(1);
   if(button == 3)
      x = 0; y = 0; 
   end
   pointsPos(i, :) = [x, y];
    
end

save(fullfile(workingDirWithoutTask, 'pointsPos.mat'), 'pointsPos');
