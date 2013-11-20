function findOptimal()

load C.mat;

cams = C(1:3);
level = 10;
theta = [1:level];

% ======================================
numOfcams = numel(cams);

for startCam = 1:numOfcams-1
   edgeWeights = inf(level, level, numOfcams  );   
   
   for endCam = startCam + 1 : numOfcams
        [meshStart, meshEnd] = meshgrid(1:level, 1:level);
        meshStart = meshStart(:)'; meshEnd = meshEnd(:)';
        
        Pos1 = repmat(cams{startCam}.C, 1, numel(meshStart) ) + kron( meshStart , cams{startCam}.ori );
        Pos2 = repmat(cams{endCam}.C, 1, numel(meshEnd)) +   kron( meshEnd , cams{endCam}.ori );
        dis = sum((Pos1 - Pos2).^2,1);
        dis = reshape(dis, level, level); dis = dis';
        edgeWeights( :, :, endCam) = dis;        
%         edgeWeights() =1 ;       
   end
   C{startCam}.edgeWeights = edgeWeights;   
end





% for firstCam = 1:numOfcams
%     for secCam = (firstCam + 1):numOfcams 
%         for thirdCam = (secCam + 1):numOfcams
%              cost_1st_3rd = C{firstCam}.edgeWeights(:,:,thirdCam);
%              cost_1st_2nd = C{firstCam}.edgeWeights(:,:,secCam);
%              cost_2nd_3rd = C{secCam}.edgeWeights(:,:,thirdCam);
%                 
%              for startId = 1:level
%                  for endId = 1:level
%                      c_1_3 = cost_1st_3rd(startId, endId);
%                      c_1_2 = cost_1st_2nd(startId,:)';
%                      c_2_3 = cost_2nd_3rd( :,   endId);
%                  end
%              end
%              
%         end
%     end
% end
















