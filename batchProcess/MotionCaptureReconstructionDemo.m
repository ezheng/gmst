function MotionCaptureReconstructionDemo(filename)

% Motion capture demo
% Paper: "3D Reconstruction of a Moving Point from a Series of 2D Projections (ECCV)"
% Authors: Hyun Soo Park, Takaaki Shiratori, Iain Matthews, and Yaser Sheikh
% Please refer to
% http://www.andrew.cmu.edu/user/hyunsoop/eccv2010/eccv_project_page.html

% This script reads motion capture data, generates a random camera trajectory, reconstructs point trajectories, and animate the motion.
% Ex> MotionCaptureReconstructionDemo('stand_data.mat')
% You can set amount of occlusion, frame rate, noise level of correspondences, and the number of basis.
% When you do not know the number of basis, you can set nBasis = -1. (Let the system decide.)

s = RandStream('mcg16807','Seed',0);
RandStream.setDefaultStream(s);

% filename = 'faint_data.mat';
filename = 'walk_data.mat';
% filename = 'stand_data.mat';

occlusion = 0.0; % 20% occlusion
framerate = 1; % 100% of full frame rate
noise_level = 0.0; % noise of image measurement
nBasis = 50; % the number of basis is determined by the cross validation.

Data = load(filename);
% 
start = 300; totalFrames = numel(Data.T);
Data.X = Data.X(:,start:totalFrames);
Data.Y = Data.Y(:,start:totalFrames);
Data.Z = Data.Z(:,start:totalFrames);
Data.T = Data.T(:,start:totalFrames); 
Data.T = Data.T - (start - 1);


% permutation = randperm(numel(Data.T));
% Data.T = Data.T(permutation);


X0 = Data.X(:,1:floor(1/framerate):end);
Y0 = Data.Y(:,1:floor(1/framerate):end);
Z0 = Data.Z(:,1:floor(1/framerate):end);
X0 = X0-mean(X0(:)) + rand(size(X0))*50;        % the data is normalized
% stdX = sqrt(sum(X0(:).^2)); X0 = X0/stdX*100;
% figure(1); clf; hold on; plot(X0(1,:));hold off;

% dctSignal = dct(X0(1,:));
% figure(2); plot(dctSignal);
% figure(1); hold on; plot(X0(1,permutation));hold off;

Y0 = Y0-mean(Y0(:))+120 + rand(size(X0))*100;
% stdY = sqrt(sum(Y0(:).^2)); Y0 = Y0/stdY*100;

Z0 = Z0-mean(Z0(:));
% stdZ = sqrt(sum(Z0(:).^2)); Z0 = Z0/stdZ*100;

T0 = Data.T(:,1:floor(1/framerate):end)-1;

% data = [X0(1,:); Y0(1,:); Z0(1,:)];
% [u, s, v] = svd(data);
% data1 = [X0,Y0,Z0];
% [u1, s1, v1] = svd(data1);
% data2 = [X0(1:3,:)';Y0(1:3,:)';Z0(1:3,:)'];
% [u2, s2, v2] = svd(data2);



for i = 1:size(X0,2)
    DSG{i} = [X0(:,i), Y0(:,i), Z0(:,i)];
end

% Camera trajectory generation
% CameraTrajectory = rand(3, length(T0))*10+300;
CameraTrajectory = rand(3, length(T0))*100 + 200;
% CameraTrajectory(3,:) = CameraTrajectory(3,:) +100 ;
CameraTrajectory(2,:) = rand(1,length(T0)) * 600 - 300;
CameraTrajectory(3,:) = 0;
% K = [5e+3 0 500; 0 5e+3 500; 0 0 1];

% xxx = 80;
% T0(1:xxx) = randperm(xxx)-1;
% T0(1:xxx) = 0:xxx-1;

 figure(3); clf;
unseen = 0;
for iCamera = 1 : size(CameraTrajectory,2)
    
    % Camera parameter setting
    r3 = -CameraTrajectory(:,iCamera)'/norm(CameraTrajectory(:,iCamera));
    r1 = null(r3);  r1 = r1(:,1);
    r2 = Vec2Skew(r3) * r1;
    R = [r1'; r2'; r3];
    
%     C{iCamera}.P = K*R*[eye(3) -CameraTrajectory(:,iCamera)];
    C{iCamera}.P = R*[eye(3) -CameraTrajectory(:,iCamera)];
    C{iCamera}.t = T0(iCamera);
    % 2D projection
    m = C{iCamera}.P * [X0(:, (iCamera))'; Y0(:, (iCamera))'; Z0(:, (iCamera))'; ones(1,size(X0,1))];
    if  ~(all(m(3,:) > 0) )
        C{iCamera} = C{iCamera - 1};
        iCamera
        unseen = unseen + 1;
        continue;
    end
    C{iCamera}.m = [m(1,:)./m(3,:); m(2,:)./m(3,:)]';
    C{iCamera}.m = C{iCamera}.m + noise_level*randn(size(C{iCamera}.m));
    C{iCamera}.Pt3d = [X0(:, (iCamera))'; Y0(:, (iCamera))'; Z0(:, (iCamera))'];
    C{iCamera}.R = R;
    C{iCamera}.C = CameraTrajectory(:,iCamera);
    
%     figureHandle = figure(3); scale = 1; hold on
%     drawOneCam(C{iCamera}.R, C{iCamera}.C, scale, figureHandle)
    
end

fprintf(1, '%f percent cannot see\n', unseen/size(CameraTrajectory,2)*100);

% Occlusion
for iPoint = 1 : size(X0,1)
    pm = randperm(length(T0));
    for iOcc = 1 : floor(length(T0)*occlusion)
        C{pm(iOcc)}.m(iPoint,:) = [NaN NaN];
    end
end

% Reconstruct point trajectories
% randPermutation = randperm(numel(C));
randPermutation = 1:numel(C);
for i = 1:numel(randPermutation)
   newC{i} = C{randPermutation(i)};
end
C = newC;


[Traj] = TrajectoryReconstruction(C, nBasis); % Reconstruction w/o predefined number of basis
% for i = 1:13
%    Traj{i} = Traj{4}; 
% end
for i = 1 : size(Traj{1},1)
    ds = [];
    for j = 1 : length(Traj)
        ds = [ds; Traj{j}(i,:)];
    end
    DS{i} = ds;
end


for i = 1: numel(randPermutation)
    newDS{ randPermutation(i)} = DS{i};    
end
DS = newDS;


% Animate result
figure(3);hold on;
for i = 1 : length(DS)
    d = DSG{i};
    DSG{i} = repmat(d(1,:), size(d,1),1);
    if i == 1 
        h1 = DrawMocapHuman(DS{i}, 'b-x'); hold on
        h2 = DrawMocapHuman(DSG{i}, 'r-x'); hold on
    else
        DrawMocapHuman(DS{i}, 'b-x', h1); hold on
         DrawMocapHuman(DSG{i}, 'r-x', h2);hold on
    end
    grid on, axis on,  axis equal;
%     axis vis3d;
%     set(gcf, 'color', 'w');
    xlabel('x');
    ylabel('y');
    zlabel('z');
    axis([-300 300 -300 300 -50 80])
% axis equal
    drawnow
    pause(0.1);
end
hold off;

% figure(1);
% for i = 1 : length(DSG)
%     if i == 1 
%         h = DrawMocapHuman(DSG{i}, 'r-x'); hold on
%     else
%         DrawMocapHuman(DSG{i}, 'r-x', h);
%     end
%     grid on, axis off, axis vis3d, axis equal, set(gcf, 'color', 'w');
%     axis([-100 100 -100 100 -50 80])
% % axis equal
%     drawnow
%     pause(0.1);
% end


