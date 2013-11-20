function writeBachFile()

if(~exist('logs', 'dir'))
    mkdir('logs');
end

discretizedLevel = 49;
timeLimitRange = 499;

fileName = 'killDevilBatch';
fid = fopen(fileName, 'w');
assert(fid>0)

for i = 1 :10: 100
    timeLimit = timeLimitRange * i;    
    for numberOfCams = 50:10:100   
%     for numberOfCams = 200
        memoryRequirement = (numberOfCams ^ 2 *discretizedLevel ^ 2 * 3 * 2 * 8)*2/1024/1024/1024;
        memoryRequirement = ceil(memoryRequirement);
        if(memoryRequirement < 4)
            memoryRequirement = 8;
        elseif(memoryRequirement > 64)
            memoryRequirement = 64;
        end
            
        fprintf(fid, 'bsub -q week -o ../data_time%d/task_%d/log.%%J -M %d matlab -nodisplay -nojvm -nosplash -r "mainKillDevil(%d,%d,%d)"\n'...
         , timeLimit, numberOfCams, memoryRequirement ,numberOfCams, discretizedLevel, timeLimit);
    end
end

fclose(fid);
