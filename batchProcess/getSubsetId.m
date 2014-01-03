function allsubsetId = getSubsetId(numOfCameras, numOfCamPerSubset)


allId = randperm( numOfCameras);


numOfSubset = ceil( numOfCameras/numOfCamPerSubset );

for i = 1: numOfSubset-1
    allsubsetId{i} = allId(  [(i-1)*numOfCamPerSubset+1: i*numOfCamPerSubset]   );
end

allsubsetId{numOfSubset} = allId( end-numOfCamPerSubset+1 : end);
