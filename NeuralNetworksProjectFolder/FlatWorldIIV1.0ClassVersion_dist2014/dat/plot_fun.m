file_name = 'timevsspeed_arch2data';
fileID = fopen(strcat(file_name, '.csv'));
C = textscan(fileID, '%f,%f,%f');
fclose(fileID);
celldisp(C);


X = unique(C{1});
[m, n] = size(X);
X = X(2:m, :);
b = C{3};
b = b(2:end, :);
Y = transpose(reshape(C{3}, [], 20));

<<<<<<< HEAD
errorbar(C{1}, C{3}, E);hold on
title('');
=======
M = mean(Y, 2);
E = std(Y, 1, 2);


errorbar(X, M, E);
title('Architecture 2 Life Time vs Speed');
>>>>>>> FETCH_HEAD
xlabel('speed');
ylabel('simulation life');
