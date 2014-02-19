function cost =  usingPlane(C,points3D, h)

offset = [-2:0.01:2];
% offset = 0.01
cost = zeros(numel(offset), 1);
conditionNum = zeros(numel(offset), 1);
for i = 1:numel(offset)
    allPoints3D = [points3D.pos];
    allPoints3D = reshape(allPoints3D, 3, []);
    allPoints3D = allPoints3D';
    
%     if(i == 1)
%         [B, ~, ~] = ransacfitplane(allPoints3D', 0.1);
%     end   
%     
%     camConnect = testCandidatePlane(C, points3D, B, offset(i) );
    while(true)
        randNumSeries = randperm(137);
        camConnect = [[2:numel(C)]; randNumSeries(1:136)]';
        if( all(camConnect(:,1) ~= camConnect(:,2) ))
            break;
        end
    end
    conditionNum(i) =  computeConditionProb( camConnect, C );
    
    resultsRefine = ReconstructPointTrajectory_SumOfNorm(C, camConnect);
    cost(i) = computeCost(resultsRefine', camConnect);
end

figure(); 
plot(cost, conditionNum, '*'); 
for i = 1:numel(cost)
    text(cost(i), conditionNum(i), num2str(i));
end

for i = 1: size(camConnect,1)
        pts = resultsRefine(:, camConnect(i,:));
        figure(h); hold on; plot3(pts(1,:), pts(2,:), pts(3,:), 'k*--','MarkerSize',5); hold off;
end       

end



function plotPlanes(B)
    %# create x,y
    [xx,yy]=ndgrid(-3:0.5:2,-4:0.5:2);
    normal = B(1:3); d = B(4);
    %# calculate corresponding z
    z = (-normal(1)*xx - normal(2)*yy - d)/normal(3);
    %# plot the surface
    figure(1); hold on;
    surf(xx,yy,z);
    hold off;
end