function plotProjectedPoints(img, groundTruth, recoveredResults)

figure();
imagesc(img);axis equal; hold on
plot(groundTruth(1), groundTruth(2), '*r');
plot(recoveredResults(1), recoveredResults(2), 'ob');



hold off;






