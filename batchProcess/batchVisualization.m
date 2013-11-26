function batchVisualization()

visualization(60, 50, 100, '../', 'task');
visualization(60, 50, 500, '../', 'task');
visualization(60, 50, 1000, '../', 'task');
visualization(60, 50, 2000, '../', 'task');
visualization(60, 50, 3000, '../', 'task');
visualization(60, 50, 4000, '../', 'task');
visualization(60, 50, 5000, '../', 'task');
visualization(60, 50, 10000, '../', 'task');
visualization(60, 50, 20000, '../', 'task');
visualization(60, 50, 30000, '../', 'task');

system('tar -zcvf ../result.tar.gz ../task_time*/task*/*.jpg');

