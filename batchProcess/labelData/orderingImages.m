function orderingImages(  )

workingDir = 'F:\Enliang\matlab\GMST_subprob\gmst\realData\franklin';
targetName = 'franklin';

% read all images, and the feature points
matFileName = fullfile(workingDir, [targetName, '.mat']);
load(matFileName);
pointsPosFileName = fullfile(workingDir, ['pointsPos.mat']);

% put in one big image

numOfCams = numel(camera);

[~,name,ext] = fileparts(camera(1).fileName);

img = imread( fullfile( workingDir, 'image', [name,ext]  ) );
width = size(img, 2);
height = size(img, 1);

width = width/8;
height = height/8;


numOfRows = 12;
numOfCols = 12;
pointsPos = round(pointsPos/8);

allImgs = zeros(height*numOfRows, width * numOfCols, 3);


for row = 1:numOfRows
   for col = 1:numOfCols
        imageId = (row-1)*numOfCols + col;
        
        if(imageId > numel(camera))
            break;
        end
        [~,name,ext] = fileparts(camera(imageId).fileName);
        img = imread( fullfile( workingDir, 'image', [name,ext]  ) );
        img = imresize(img, 1/8);
        
        minL = max(pointsPos(imageId,2)-5, 1); maxR = min(height, pointsPos(imageId,2)+5);
        minT = max(pointsPos(imageId,1)-5, 1); maxB = min(width, pointsPos(imageId,1)+5);
        
        img( minL:maxR, minT:maxB,:) = 0;
        figure(1);imagesc(img);axis equal;
        startRow =(row-1)*height+1; endRow = row*height;
        startCol =(col-1)*width+1; endCol = col*width;
        allImgs( startRow:endRow, startCol:endCol,:) = img;
        
   end
end

figure(); imagesc(allImgs/255); axis equal;

addpath F:\Enliang\library_64\export_fig
export_fig('x.pdf');

% do ordering manually








