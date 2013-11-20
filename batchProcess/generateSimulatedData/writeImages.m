function writeImages(camera, imageFilePath)

for i = 1:numel(camera)
    [~, name, ext] = fileparts(camera(i).name);
    outputName = fullfile(imageFilePath, [name, ext]);    
    imwrite(camera(i).image, outputName);
end