function writeNVM(outputFileName, camera, points3D)

numOfCams = numel(camera);

fid = fopen(outputFileName, 'w');
assert(fid>0)

fprintf(fid, 'NVM_V3\n\n');
fprintf(fid, '%d\n', numOfCams);

for i = 1:numOfCams
fprintf(fid, '%s %.12f %.12f %.12f %.12f %.12f %.12f %.12f %.12f %.12f %.12f\n',...
    camera(i).name, camera(i).focalLength, camera(i).quarternion, camera(i).pos, camera(i).distortion);
end

numOf3DPoints = numel(points3D);
fprintf(fid, '\n');
fprintf(fid, '%d\n', numOf3DPoints);

for i = 1:numOf3DPoints
    fprintf(fid, '%.12f %.12f %.12f ', points3D(i).pos);
    fprintf(fid, '%d %d %d ', points3D(i).rgb);
    
    numOfMeasure = size(points3D(i).measure, 1);
    fprintf(fid, '%d ', numOfMeasure);
    for j = 1: numOfMeasure
       fprintf(fid, '%d %d %.12f %.12f ', points3D(i).measure(j,:)); 
    end
    fprintf(fid, '\n');
end
fclose(fid);
