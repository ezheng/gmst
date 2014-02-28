function [points3D, orientation] = generate3DPoints(numOfCameras, noiseLevel, visualize)

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

orientation = [ repmat(L1, 1, size(points3D,2)-1), repmat(L2,1, size(newPoints3D,2))];
orientation = [orientation];
% ----
% idx1 = size(orientation,2) - floor(size(newPoints3D,2)/2);
% orientation(4:6, idx1) = L1;
% if( mod(size(newPoints3D,2),2) == 0)
%     orientation(4:6,idx1+1) = L1;
% end
% ----
points3D = [points3D(:,(1:end-1)), newPoints3D];

% startPoint = points3D(:,end);

points3D = points3D - repmat(mean(points3D,2), 1, size(points3D,2));
scale = sqrt( sum(points3D(:).^2));
points3D = points3D./scale*10;

if(visualize)
    figure(1);
     plot3(points3D(1,:), points3D(2,:), points3D(3,:), '*'); axis equal;
    hold on; 
    for i = 1:size(points3D,2)        
       
%         also plot the orientation 
        plot3( [points3D(1,i), points3D(1,i)+ 0.1*orientation(1,i)],[points3D(2,i), points3D(2,i)+ 0.1*orientation(2,i)],...
            [points3D(3,i), points3D(3,i)+ 0.1*orientation(3,i)]);
%         plot3( [points3D(1,i), points3D(1,i)+ 0.1*orientation(4,i)],[points3D(2,i), points3D(2,i)+ 0.1*orientation(5,i)],...
%             [points3D(3,i), points3D(3,i)+ 0.1*orientation(6,i)], 'r');
    end
    hold off;
end

% singleOrientation = [L1, L2];

% orientation = [L1,L2; L1,L2];

