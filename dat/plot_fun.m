file_name = 'timevsspeed-second';
fileID = fopen(strcat(file_name, '.csv'));
C = textscan(fileID, '%f,%f');
fclose(fileID);
celldisp(C);
A = unique(C{1});
B = reshape(C{2}, [22 20]);

D = mean(B, 2);
E = std(B, 0, 2);
errorbar(A, D, 3*E);
title(file_name);
xlabel('speed');
ylabel('simulation life');
