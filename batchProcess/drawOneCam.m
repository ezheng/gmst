function drawOneCam(R, C, scale, figureHandle)

% R = eye(3); C = [0.5,0,0]'; scale = 1; figureHandle = 1;
if(numel(R) == 16)
   R = R(1:3,1:3); 
end

if(nargin <3)
    scale = 1;
end

localCoord = [0,0,0; 1,1,1; -1,1,1; 0,0,0; -1,-1,1; 1,-1,1;...
              0,0,0; 1,1,1; 1,-1,1; 0,0,0; -1,-1,1; -1,1,1  ].* scale;
localCoord = localCoord';
worldCoord = R' * localCoord + repmat(C, 1, size(localCoord,2));


figure(figureHandle);
hold on
plot3(worldCoord(1,:), worldCoord(2,:), worldCoord(3,:), 'r'); 
axis equal; 
hold off




