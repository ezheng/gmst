function [beta, jectory, pts, pts2] = ReconstructPointTrajectory(P, time_instance, measurement, sequence_length, K)

QQ = []; QQ = sparse(QQ);
qq = [];
Theta = [];
 smallTheta = [];

for iCamera = 1 : length(P)
    p = P{iCamera};
    skew = Vec2Skew([measurement(iCamera,:)';1]);
    Q = skew*p(:,1:3);
    q = skew*p(:,4);
    Q = Q(1:2,:);
    q = -q(1:2,:);
    QQ = blkdiag(QQ, Q);
    qq = [qq; q];
    theta = IDCT_continuous(sequence_length, time_instance(iCamera));
%     theta = theta(1:K);
    Theta = [Theta; blkdiag(theta, theta, theta)];
%      smallTheta = [smallTheta; blkdiag(theta(1:K),theta(1:K),theta(1:K))];
end
% beta = (QQ*Theta)\qq;
%  beta = (QQ* smallTheta)\qq;
beta = [];
jectory = [];

% D = QQ * Theta;
% X = qq;
% addpath F:\Enliang\temp\trajectories\spams-matlab\build;
% param.lambda = 1000; param.mode = 1;
% tic;
% result = mexLasso(X,D,param);
% toc
% A = full(result); sum(A==0)
% pts2 = Theta * A;
pts2 = [];
% ======================================================
%  use cvxopt
numOfCameras = length(P);



fid = fopen( 'log.txt', 'w');
currentMin = inf;
tic

    
    
    permMatrix = eye( numOfCameras );
%     idx = randperm(numOfCameras);
    idx = 1:numOfCameras;
%     fprintf(fid, 'Iter:%05d ', i);
    fprintf(fid, 'Idx: ');
    fprintf(fid, '%d ', idx);
    permMatrix = permMatrix(idx, :);
    
    cvx_begin
    variable x(numOfCameras * 3);
    %     variable permMatrix(numOfCameras, numOfCameras);
    reorderX = reshape(x,  3, numOfCameras);
    reorderX = reorderX * permMatrix;
    
    %     startP = 1; endP = numOfCameras-1;
    for i = 1:numOfCameras-1
        %         ss = endP - startP + 1;
        %         sub(1:3, startP:endP) = repmat(reorderX(:, i ), 1, ss) - reorderX(:, (i+1):end );
        %         startP = endP + 1;
        %         endP = startP + (numOfCameras - 1 - i-1);
        sub = reorderX(:,2:end) - reorderX(:, 1:(end-1) );
    end
    
  
    
    sub = sum(  (   sum(square(sub))      )       );

    minimize(  sub );
    subject to
    norm(QQ * x - qq,2) <=1;
    cvx_end
    pts = x;
    
    if(currentMin > cvx_optval)
        currentMin = cvx_optval;
    end
    fprintf(fid, 'cvx_opt: %f, Min: %f\n', cvx_optval, currentMin);

toc
% ======================================================
fclose(fid);







