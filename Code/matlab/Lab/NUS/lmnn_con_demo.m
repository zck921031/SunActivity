%
clc;
clear;

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';

[ xTr, yTr, xTe, yTe ] = load_feature();
xTr = xTr'; yTr = yTr';
xTe = xTe'; yTe = yTe';

tic
L0=pca(xTr)';

[L,~] = lmnn2(xTr, yTr, 3, L0, 'maxiter', 300, 'quiet', 0,'outdim', 10, 'mu', 0.5, 'validation', 0.0, 'earlystopping', 25, 'subsample', 0.3, 'modal', 9, 'stepgrowth', 1.05);

usedtime = toc;

errRAW = knncl( L, xTr, yTr, xTe, yTe, 1);
disp( ['lmnn_con knn train error rate: ' num2str(errRAW(1) ) ] );
disp( ['lmnn_con knn test  error rate: ' num2str(errRAW(2) ) ] );
disp( ['Elapsed time is ' num2str(usedtime) ' seconds' ] );

save result_lmnn_con L errRAW usedtime;

