function [Traj, Traj2] = TrajectoryReconstruction(C)

% Trajectory reconstruction algorithm from 2D projections
% Paper: "3D Reconstruction of a Moving Point from a Series of 2D Projections (ECCV)"
% Authors: Hyun Soo Park, Takaaki Shiratori, Iain Matthews, and Yaser Sheikh
% Please refer to http://www.andrew.cmu.edu/user/hyunsoop/eccv2010/eccv_project_page.html
% 
% This function takes cells of camera projection information C and prior weight parameter w for selecting the number of basis.
% 
% INPUT:
% Each cell C contains camera projection matrix, time instance, and 2D correspondences.
%     C{i}.P: 3 x 4 projection matrix
%     C{i}.t: time instance that the image is taken
%     C{i}.m: p x 2 matrix where p is the number of correspondences, i.e, m = [x(1) y(1); ... ; x(p) y(p)]
%     if there is a missing correspondence, you should set the corresponding point as [x y] = [NaN NaN]
% K is the number of basis (optional, default = automatic selection -- let the algorithm decide)
%
% 
% OUTPUT:
% Traj is a set of the reconstructed trajectories.
% Each Traj{i} are F x 3 matrix, i.e, [X Y Z] where F is the number of time step specified by dT
% Beta is a set of the coeffient of a trajectory.
% Beta{i} is K x 3 dimensional vector, [beta_x beta_y beta_z], of the i-th trajectory where 1 <= i <= p. 
% 
% Written by Hyun Soo Park
% hyunsoop@cs.cmu.edu


% Find sequence length
sequence_length = 0;
for iCamera = 1 : length(C)
    if (sequence_length < C{iCamera}.t)
        sequence_length = C{iCamera}.t;
    end
end
sequence_length = ceil(sequence_length)+1;

nPoints = size(C{1}.m,1);

for iPoint = 1 : nPoints
    measurements = [];  P = [];  time_instants = [];
    for iCamera = 1 : length(C)
        if ~isnan(C{iCamera}.m(iPoint,1))
            P{end+1} = C{iCamera}.P;
            time_instants(end+1) = C{iCamera}.t;
            measurements(end+1,:) = C{iCamera}.m(iPoint,:);
%             pt3d(end+1, :) = C{iCamera}.Pt3d(:,1);            
        end
    end
    if(iPoint == 1)   
        
        [~,~,pts, pts2] = ReconstructPointTrajectory(P, time_instants, measurements, sequence_length, 2);
        Traj{iPoint} = [pts(1:3:end), pts(2:3:end), pts(3:3:end)];       
        Traj2{iPoint} = [pts2(1:3:end), pts2(2:3:end), pts2(3:3:end)];       
    else
        Traj{iPoint} = Traj{1};       
        Traj2{iPoint} = Traj2{1};
    end
end

end
