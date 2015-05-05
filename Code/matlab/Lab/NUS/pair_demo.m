%
clc;
clear;

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\GlobalMetricLearning';

% cd 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\Func';
% [ xTr, yTr, xTe, yTe ] = load_feature('Sunspot', 1:9, 1:3);
% cd 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\NUS';

[ xTr, yTr, xTe, yTe ] = load_feature();


xTr = xTr'; yTr = yTr';
xTe = xTe'; yTe = yTe';

tic
L=pca(xTr)';
L = L(1:30, :);

for i = 1 : 1000
    L = pair_sto(xTr, yTr, L, 3);
    errRAW = knncl( [], L*xTr, yTr, L*xTe, yTe, 1);
    disp( ['pair_sto_con knn train error rate: ' num2str(errRAW(1) ) ] );
    disp( ['pair_sto_con knn test  error rate: ' num2str(errRAW(2) ) ] );
    disp( ['norm: ' num2str(norm(L,2)) ] );
end

usedtime = toc;

disp( ['Elapsed time is ' num2str(usedtime) ' seconds' ] );



