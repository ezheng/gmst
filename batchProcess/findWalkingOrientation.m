function [orientation, B] = findWalkingOrientation(C,points3D)

allPoints3D = [points3D.pos];
allPoints3D = reshape(allPoints3D, 3, []);
allPoints3D = allPoints3D';

[B, ~, ~] = ransacfitplane(allPoints3D', 0.1);


% --------------------------------------- fit a line to camera centers
cams = [C{:}];
camCenters = [cams.C];


[V, ~, ~] = ransacfitline(camCenters, 0.2);

lineDir = V(:,1) - V(:,2);
lineDir = lineDir/norm(lineDir);

planeDir = B(1:3)/norm(B(1:3));

orientation = lineDir - dot(planeDir, lineDir) * lineDir;
orientation = orientation/norm(orientation);



















