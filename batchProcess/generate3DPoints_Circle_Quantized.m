function [points3D, orientation] = generate3DPoints_Circle_Quantized(numOfCameras, noiseLevel, visualize)

if(nargin < 3)
    visualize = false;
end


rotationAxis = rand(3,1); rotationAxis = rotationAxis/norm(rotationAxis);

theVector = null(rotationAxis')*3; 
theVector = theVector(:,1) + rand(3,1)*2;

theta = linspace(0,pi,numOfCameras);

points3D = zeros(3,numOfCameras);
for i = 1:numel(theta)
    points3D(:,i) = rodrigues_rot(theVector,rotationAxis, theta(i)); 
end

% 

orientation = zeros(6, numOfCameras);
orientation(1:3, 1:end-1) = points3D(:,2:end) - points3D(:,1:end-1); orientation(1:3,end) = orientation(1:3, end-1);
orientation(4:6, 2:end) = points3D(:,1:end-1) - points3D(:,2:end); orientation(4:6, 1) = orientation(4:6,2);

% quantize the orientation -----------------------------------------------------------------
rotationAngle = [0,1/4*pi,1/2*pi,3/4*pi];
quantizationDir = null(rotationAxis'); quantizationDir = quantizationDir(:,1);
allDir = zeros(3, 4);
for i = 1:4
   allDir(:,i) =  rodrigues_rot(quantizationDir, rotationAxis, rotationAngle(i)); 
   allDir(:,i) = allDir(:,i)/norm(allDir(:,i));
end
dotValue = zeros(4, size(orientation,2));

for j = 1:2
    for i = 1:4
        dotValue(i,:) = abs( allDir(:,i)' * orientation((j-1)*3+1:j*3,: ) );
    end
    [~, idx] = max(dotValue, [], 1);
    orientation( (j-1)*3+1:j*3, :) = allDir(:, idx);
end
% ------------------------------------------------------------------------------------------

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








