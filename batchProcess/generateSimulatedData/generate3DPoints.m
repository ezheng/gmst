function points3D = generate3DPoints(numOfCameras, noiseLevel, visualize)

if(nargin < 2)
    noiseLevel = 1;
end
if(nargin < 3)
    visualize = false;
end

L1 = rand(3,1);
L1 = L1/ norm(L1);

% fixPoint = [0, 0, 0]';
 L2 = null(L1'); L2 = L2(:,1);

%theta = linspace(0,1, numOfCameras);
thetaFirstHalf = linspace(0,1,round(numOfCameras/2));
thetaSecondHalf = linspace(-0.5,0.5, numOfCameras - round(numOfCameras/2)+1);

points3D = L1 * thetaFirstHalf;
% points3D = points3D + ones(size(points3D)) .* noiseLevel;
newPoints3D = points3D(:,end);
newPoints3D = repmat(newPoints3D, 1, size(thetaSecondHalf, 2 )) +  L2 * thetaSecondHalf ;
points3D = [points3D(:,(1:end-1)), newPoints3D];

% startPoint = points3D(:,end);

points3D = points3D - repmat(mean(points3D,2), 1, size(points3D,2));
scale = sqrt( sum(points3D(:).^2));
points3D = points3D./scale*10;

if(visualize)
    figure(1);
    plot3(points3D(1,:), points3D(2,:), points3D(3,:), '*');    
end
