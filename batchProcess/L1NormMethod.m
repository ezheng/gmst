function L1pts = L1NormMethod(C, orientation)


numOfCams = numel(C);
assert(numOfCams >= 1);
numOfEdges = (numOfCams) * (numOfCams-1)/2;

cameraCenterMatrix = zeros( 3, numOfCams);
directionMatrix = zeros(3, numOfCams);


for i = 1: numOfCams
   cameraCenterMatrix(:, i) = C{i}.C;

    direction = C{i}.ori;
    direction = direction/norm(direction);
    directionMatrix(:, i) = direction;    
end
edgeId = combnk(1:numOfCams,2);


cvx_begin
    variable S(numOfEdges,1);
    variable t(numOfCams,1);
    
    pts = cameraCenterMatrix + directionMatrix .* repmat(t', 3,1);
%     theNorm = norm(pts(:,edgeId(:,1))-pts(:,edgeId(:,2)),2 );
%         theNorm = sum( (pts(:,edgeId(:,1)) - pts(:,edgeId(:,2))).^2); theNorm = theNorm';
    theNorm = cvx(numOfEdges,1);
    theDotprod = cvx(numOfEdges,1);
    id = 1;
    for i = 1:numOfCams            
        for j = i+1:numOfCams
%             norm( pts(:,i)-pts(:,j) ,2) <= S(id);
%             theNorm(id) = norm( pts(:,i)-pts(:,j) ,2) - [ 0.5774    0.5774    0.5774] * (pts(:,i)-pts(:,j));
%             theNorm(id) = norm( pts(:,i)-pts(:,j) ,2);
                theNorm(id) = sum(( pts(:,i)-pts(:,j) ).^2);
%             theNorm = abs([ 0.5774    0.5774    0.5774] * (pts(:,i)-pts(:,j)));
            theDotprod(id) = ( orientation(:,1)' * (pts(:,i)-pts(:,j)) ).^2;
%             theDotprod(id+numOfEdges) = ( orientation(:,2)' * (pts(:,i)-pts(:,j)) ).^2;
            id = id + 1;
        end
    end 
    
    
%     minimize norm(S(1:numOfEdges),1) + 100* norm(S(numOfEdges+1:end), 1) ;
    minimize sum(theNorm) + 100* norm(S, 1) ;
    subject to
%         theNorm  <= S(1:numOfEdges);
%         theDotprod <= S(numOfEdges+1:2*numOfEdges)
          theDotprod  <= S;
%         theDotprod < 0.1;
        t>=0;
        
cvx_end

x = find(S<0.0000001);
% edgeId(x,:)
% 
G = sparse(edgeId(:,1),edgeId(:,2),S(1:size(edgeId,1)), numOfCams, numOfCams);
G = G';
[Tree, pred] = graphminspantree(G,'METHOD', 'Prim');

L1pts = cameraCenterMatrix + directionMatrix .* repmat(t', 3,1);

















