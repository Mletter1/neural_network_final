file_name = 'timevsspeed';
fileID = fopen(strcat(file_name, '.csv'));
C = textscan(fileID, '%f,%f,%f');
fclose(fileID);
celldisp(C);

mean(C{3});
