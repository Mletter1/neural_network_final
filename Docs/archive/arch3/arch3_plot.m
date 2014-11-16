file_name = 'eye_data_data';
fileID = fopen(strcat(file_name, '.csv'));
C = textscan(fileID, '%f,%f');
fclose(fileID);
celldisp(C);


plot(C{1}, C{2});
title('RMS vs epoch');
xlabel('epoch');
ylabel('RMS');
