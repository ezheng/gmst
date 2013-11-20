function drawAllCams(standardCams, h)



numOfCams = numel(standardCams);
% draw cams

for i = 1:numOfCams

    drawOneCam(standardCams(i).R, standardCams(i).C, 0.2, h);    
end
xlabel('x'); ylabel('y'); zlabel('z');