file_name = 'eye_data_data_arch2';
fileID = fopen(strcat(file_name, '.csv'));
C = textscan(fileID, '%f,%f');
fclose(fileID);
celldisp(C);


plot(C{1}, C{2});
title('RMS vs lifetime');
xlabel('lifetime');
ylabel('RMS');
