function writePCDBFile(pcdbFileName, workingDir)


fid = fopen(pcdbFileName, 'w');
assert(fid>0);

fprintf(fid, '#DB_VERSION_0\n\n');
fprintf(fid, 'image DIR jpg 1 0\n');
fprintf(fid, 'image DEF 0 110 %s\n', fullfile(workingDir, 'image'));

fclose(fid);













