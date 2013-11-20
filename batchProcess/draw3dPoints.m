function draw3dPoints(points3D, h)

samplingRate = 10;
sampledNumOfSamples = numel(1 : samplingRate : numel(points3D));
all3DPoints = zeros(3, sampledNumOfSamples);
id = 1;
for i = 1 : samplingRate: numel(points3D)
    all3DPoints(:,id) = points3D(i).pos;    
    id = id + 1;
end
figure(h);  
hold on;
plot3(all3DPoints(1,:), all3DPoints(2,:), all3DPoints(3,:), '*', 'MarkerSize',2); axis equal;
hold off;