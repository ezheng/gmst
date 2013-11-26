function [camera, near, far ] = generateCamPoses_Inplane(numOfCameras, points3D)

% generate rotation and translation

centerOfPoints = mean(points3D, 2);
radius = sum((points3D - repmat(centerOfPoints, 1, size(points3D,2))).^2, 1);
radius = sqrt(radius);
maxRadius = max(radius);

% find the camera with 10 times this radius
focalLength = 1024;
K = [focalLength, 0 512; 0 focalLength 384; 0 0 1];

cameraC = rand(3,numOfCameras);
cameraC = cameraC - 0.5;

% project it on the same plane
  % project the 3d points onto the same plane with other 3d points.
[u, s, ~] = svd(points3D);    
cameraC = cameraC - u(:,3) * (u(:,3)' * cameraC);  

% -----------------------------------------------------------------
% scale = sqrt(sum(cameraC.^2, 1));
% cameraC = cameraC ./ repmat( scale, 3, 1 ); %normalize
% cameraC = cameraC * (maxRadius * 10) + rand(size(cameraC)) * 2 *maxRadius;
cameraC = cameraC ./ repmat(sqrt(sum(cameraC .^2 ,1)), 3,1);
cameraC = repmat(centerOfPoints, 1, size(cameraC, 2)) + cameraC .* (maxRadius * 2 + rand(size(cameraC)) * maxRadius);

% project again
cameraC = cameraC - u(:,3) * (u(:,3)' * cameraC);  

imageWidth = K(1,3) * 2;
imageHeight = K(2,3) * 2;

near = zeros(numOfCameras, 1);
far = zeros(numOfCameras, 1);

for i = 1:numOfCameras
    r3 = -cameraC(:,i)/norm(cameraC(:,i));
    r1 = null(r3'); r1= r1(:,1);
    r2 = cross(r3, r1);
    R = [r1'; r2'; r3'];
    C = cameraC(:,i);
    
    camera(i).C = C;
    camera(i).R = R;
    camera(i).P = K * [R, -R*C];
    m = camera(i).P * [points3D(:,i); 1];
    
%     p2d = [ R, -R*C ] * [points3D(:,i);1];
%     p2d = p2d(3);
%     near(i) = p2d*0.7;
%     far(i) = p2d*1.3;
    p2d = norm( points3D(:,i) - C);
    near(i) = p2d * 0.7;
    far(i) = p2d * 1.3;
    
%   camera(i).T = T;
    camera(i).measure = [m(1,:)./m(3,:); m(2,:)./m(3,:)]';
    
    if(m(3,:) < 0)
        fprintf('warning: 3d points behind camera %d\n', i);
    end
    if(camera(i).measure(1) < 1 || camera(i).measure(2) < 1 || ...
            camera(i).measure(1) > imageWidth || camera(i).measure(2) > imageHeight)
        fprintf('warning: 3d points out of image %d\n', i);
    end
    camera(i).name = sprintf('%04d.jpg',i );
    
    camera(i).pt3D = points3D(:,i);        
    camera(i).focalLength = focalLength; 
    camera(i).quarternion = matrix2quaternion(R); 
    camera(i).pos = camera(i).C; 
    camera(i).distortion = [0 0];   
    
end


for i = 1:numOfCameras
    img = double(zeros(imageHeight, imageWidth, 3));
    m = round(camera(i).measure);
    startX = m(1) - 10; endX = m(1) + 10;
    startY = m(2) - 10; endY = m(2) + 10;
    startX(startX < 1 ) = 1;
    startY(startY < 1 ) = 1;
    endX(endX>imageWidth) = imageWidth;
    endY(endY>imageHeight) = imageHeight;
    
    img(startY:endY, startX:endX ) = 1.0;
    camera(i).image = img;
end

