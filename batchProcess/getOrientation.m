function cameras = getOrientation(cameras)

% get the orientation 

for i = 1:numel(cameras)
    principalPoint = [cameras{i}.K(1,3), cameras{i}.K(2,3) ];
    pixelPos = cameras{i}.m;
    
    orientationLocal = [ pixelPos - principalPoint, cameras{i}.K(1,1)]';
%     orientationLocal = orientation/norm(orientation);
    
    inverseTransform = [cameras{i}.R', - cameras{i}.R' * cameras{i}.T];
    
    orientation = inverseTransform * ([orientationLocal; 1] - [0;0;0;1] );
    
%     assert(orientation(4) == 0)
    
    cameras{i}.ori = orientation/ norm(orientation);

end











