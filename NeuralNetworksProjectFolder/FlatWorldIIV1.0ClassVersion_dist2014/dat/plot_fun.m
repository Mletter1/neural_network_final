file_name = 'timevsspeed';
fileID = fopen(strcat(file_name, '.csv'));
C = textscan(fileID, '%f,%f,%f,,');
fclose(fileID);
celldisp(C);

[m, n] = size(C{1});

E = zeros(m, n);

errorbar(C{1}, C{3}, E);
title(file_name);
xlabel('speed');
ylabel('simulation life');
