function perpendicularDir = computePerpendicularDir(orientation, C)

C= [C{:}];
camCenters = [C.C];

% estimate the plane orientation

% get perpendicularDir within the plane

[B, ~, ~] = ransacfitplane( camCenters, 0.00001);


nullSpace = null(orientation');
B(1:3) = B(1:3)/norm(B(1:3))

perpendicularDir = nullSpace(:,1) - nullSpace(:,1)'*B(1:3) * B(1:3);

perpendicularDir = perpendicularDir/norm(perpendicularDir);