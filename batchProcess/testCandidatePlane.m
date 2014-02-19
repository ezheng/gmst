function camConnect = testCandidatePlane(cams, points3D, B, offset)
if(nargin == 3)
     offset = 0;
end

% 1) find a plane, plot the dots pattern on the plane
numOfCams = numel(cams);


%  figure(1); plot3(allPoints3D(1:10:end,1), allPoints3D(1:10:end,2), allPoints3D(1:10:end,3), '*'); axis equal;

%  PCA does not work well
% [direction,~,~] = pca_by_svd(allPoints3D);
% center = [0,-1,1]';
% points=[ center+direction(:,1),  center+direction(:,2),  center+direction(:,3)]; % using the data given in the question
% figure(1); hold on;
% fill3(points(1,:),points(2,:),points(3,:),'r'); hold off;


B(4) = B(4) + offset;

cams = [cams{:}];
camsC = [cams(:).C];    camsViewDir = [cams(:).ori];
t = -( B(1:3)' * camsC + ones(1,numOfCams) * B(4))./(B(1:3)' * camsViewDir);
pointsIntersect = camsC + camsViewDir .* repmat(t, 3, 1);
% figure(1); hold on; plot3(pointsIntersect(1,:), pointsIntersect(2,:), pointsIntersect(3,:), 'r*' ); axis equal;

weightMatrix = zeros(numOfCams, numOfCams);
for i = 1:numOfCams
   for j = (i+1):numOfCams 
       weightMatrix(i, j) = (norm(pointsIntersect(:,i) - pointsIntersect(:,j))).^2;
   end
end
weightMatrix = weightMatrix';
[Tree, pred] = graphminspantree( sparse(weightMatrix) );

camConnect = [[1:numel(pred)]', pred'];
camConnect = camConnect( camConnect(:,2)~=0, :);

% plotPlanes(B);
% B(4) = B(4) +0.025 ;
% plotPlanes(B);


%# a plane is a*x+b*y+c*z+d=0
%# [a,b,c] is the normal. Thus, we have to calculate
%# d and we're set
% d = -point*normal'; %'# dot product for less typing


% 2) save and visualize the planes


% 3) try to analyze all points on the plane?

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

function [PC Y varPC] = pca_by_svd(X)
    %# PCA_BY_SVD
    %#   X      data matrix of size n-by-p where n<<p
    %#   PC     columns are first n principal components
    %#   Y      data projected on those PCs
    %#   varPC  variance along the PCs
    %#
    X0 = bsxfun(@minus, X, mean(X,1));     %# shift data to zero-mean
    [U,S,PC] = svd(X0,'econ');             %# SVD decomposition
    Y = X0*PC;                             %# project X on PC
    varPC = diag(S'*S)' / size(X,1);       %# variance explained
end

