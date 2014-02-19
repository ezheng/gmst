function conditionNum =  computeConditionProb( camConnect, C )

numOfCams = size(camConnect,1) + 1;

matrix = zeros(numOfCams, numOfCams);

for i = 1:numOfCams-1
    theStart = camConnect(i,1);
    theEnd = camConnect(i,2);    
   
   matrix(theStart, theEnd) = C{theStart}.ori' * C{theEnd}.ori ;           
   matrix(theStart,theStart) = matrix(theStart, theStart) + 0.5;   
   matrix(theEnd, theEnd) = matrix(theEnd,theEnd) + 0.5;
end

matrix = matrix + matrix';
conditionNum = cond(matrix);






















