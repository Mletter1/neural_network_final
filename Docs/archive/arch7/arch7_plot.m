file_name = 'timevsspeed';
fileID = fopen(strcat(file_name, '.csv'));
C = textscan(fileID, '%f,%f,%f');
fclose(fileID);
celldisp(C);


X = unique(C{1});
[m, n] = size(X);
b = C{3};
%b = b(2:end, :);
Y = transpose(reshape(C{3}, [], 20));

M = mean(Y, 2);
E = std(Y, 1, 2);


errorbar(X, M, E);
title('Architecture 7 Life Time vs Speed');
xlabel('speed');
ylabel('simulation life');
