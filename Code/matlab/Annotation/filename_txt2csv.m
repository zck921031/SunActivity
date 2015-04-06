
clc;
clear all;
close all;
%filenames = textread('C:\Users\zck\Documents\GitHub\SunActivity\data\regional annotation\ImageNameByDay.txt','%s');
fp = fopen('C:\Users\zck\Documents\GitHub\SunActivity\data\regional annotation\ImageNameByDay.txt', 'r');
Names = {};
cnt = 0;
while ( ~feof(fp)  )
    str = fgets(fp);
    if '#' ~= str(1)
       tmp = regexp(str, ',', 'split');
       cnt = cnt+1;
       for i = 1 : 9
        s = tmp{i};
        s(s==10) = '';
        Names{cnt, i} = s;
       end       
    end
end
fclose(fp);
save filenames.mat Names;