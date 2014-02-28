function [points3D, orientation] = generate3DPoints_Circle(numOfCameras, noiseLevel, visualize)

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

orientation = zeros(3, numOfCameras);
orientation(1:3, 1:end-1) = points3D(:,2:end) - points3D(:,1:end-1); orientation(1:3,end) = orientation(1:3, end-1);
% orientation(4:6, 2:end) = points3D(:,1:end-1) - points3D(:,2:end); orientation(4:6, 1) = orientation(4:6,2);

orientation(1:3,:) = orientation(1:3,:)./ repmat(sqrt(sum(orientation(1:3,:).^2)), 3, 1);
% orientation(4:6,:) = orientation(4:6,:)./ repmat(sqrt(sum(orientation(4:6,:).^2)), 3, 1);

if(visualize)
    figure(1);
    plot3(points3D(1,:), points3D(2,:), points3D(3,:), '*');    axis equal;
end








