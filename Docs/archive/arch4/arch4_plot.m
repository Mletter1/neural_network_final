file_name = 'direction_data';
fileID = fopen(strcat(file_name, '.csv'));
C = textscan(fileID, '%f,%f');
fclose(fileID);
celldisp(C);


plot(C{1}, C{2});
title('Max Intensity Receptor Index Comparison');
xlabel('Implemented Neuron');
ylabel('Internal Function');
