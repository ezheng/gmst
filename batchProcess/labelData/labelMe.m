function labelMe()

workingDir = 'F:\Enliang\matlab\GMST_subprob\gmst\batchProcess\labelData\realData\brook3';
targetName = 'brook3';
addpath 'F:\Enliang\library_64\export_fig';
% -------------------------------------------
fullNVMFileName = fullfile(workingDir, [targetName, '.nvm']);

fullDataFileName = fullfile(workingDir, [targetName, '.mat']);
if(~exist(fullDataFileName, 'file'))
    [standardCams, points3D] = extractNVMData(fullNVMFileName);
else
    load(fullDataFileName)
end

outputDir = fullfile(workingDir, 'peopleDetection_manualLabel');
if(~exist(outputDir, 'dir'))
    mkdir(outputDir);
end

for i = 1:numel(standardCams)
% for i = 1
%     save i.mat i;
    name = standardCams(i).fileName;
    img = imread(name);
    h = figure(1);
    imshow(img);
%     PedPosition(i,1:4): 
%     1 is left x; 
%     2 is right x; 
%     3 is top y;
%     4 is bottom y.
    rectId = 1;
    PedPosition = [];
    while(true)
        rect = getrect(h);
        if(rect(3) ~= 0 && rect(4) ~= 0)
            PedPosition = [PedPosition; [rect(1), rect(1) + rect(3), rect(2), rect(2) + rect(4) ]];
        end
        str = input('input: ', 's');
        if( all(str == 'd') && ~isempty(str))
            break;
        end
       
    end
    
      
    
    [~,name,~] = fileparts(standardCams(i).fileName);
    save( fullfile(outputDir, [name, '.mat'] ), 'PedPosition');    
   
    if(~isempty(PedPosition))
        x1 = PedPosition(:,1);   x2 = PedPosition(:,2);
        y1 = PedPosition(:,3);   y2 = PedPosition(:,4);
        
        figure(h); hold on;
        line([x1 x1 x2 x2 x1]', [y1 y2 y2 y1 y1]', 'color', 'y', 'linewidth', 4, 'linestyle', '-');
        hold off;
    end
    outputDirFileName = fullfile(outputDir, [name, '.jpg']);
    figure(h);
    export_fig(outputDirFileName);
    
end



