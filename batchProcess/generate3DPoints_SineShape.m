function [points3D, orientation] = generate3DPoints_SineShape(numOfCameras, noiseLevel, visualize)

if(nargin < 2)
    noiseLevel = 1;
end
if(nargin < 3)
    visualize = false;
end


x = linspace(0,2*pi, numOfCameras);
y = sin(x)*3;
z = zeros(size(y));


randRotationMatrix = RandOrthMat(3, 1e-12);

points3D = randRotationMatrix * [x;y;z];

% points3D = points3D - repmat(mean(points3D,2), 1, size(points3D,2));
% scale = sqrt( sum(points3D(:).^2));
% points3D = points3D./scale*4;



orientation = zeros(6, numOfCameras);
orientation(1:3, 1:end-1) = points3D(:,2:end) - points3D(:,1:end-1); orientation(1:3,end) = orientation(1:3, end-1);
orientation(4:6, 2:end) = points3D(:,1:end-1) - points3D(:,2:end); orientation(4:6, 1) = orientation(4:6,2);

orientation(1:3,:) = orientation(1:3,:)./ repmat(sqrt(sum(orientation(1:3,:).^2)), 3, 1);
orientation(4:6,:) = orientation(4:6,:)./ repmat(sqrt(sum(orientation(4:6,:).^2)), 3, 1);

if(visualize)
    figure(1);
     plot3(points3D(1,:), points3D(2,:), points3D(3,:), '*'); axis equal;
    hold on; 
    for i = 1:size(points3D,2)        
       
%         also plot the orientation 
        plot3( [points3D(1,i), points3D(1,i)+ 0.1*orientation(1,i)],[points3D(2,i), points3D(2,i)+ 0.1*orientation(2,i)],...
            [points3D(3,i), points3D(3,i)+ 0.1*orientation(3,i)]);
        plot3( [points3D(1,i), points3D(1,i)+ 0.1*orientation(4,i)],[points3D(2,i), points3D(2,i)+ 0.1*orientation(5,i)],...
            [points3D(3,i), points3D(3,i)+ 0.1*orientation(6,i)], 'r');
    end
    hold off;
end





