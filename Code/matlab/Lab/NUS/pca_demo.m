%

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';

[ xTr, yTr, xTe, yTe ] = load_feature();
xTr = xTr'; yTr = yTr';
xTe = xTe'; yTe = yTe';

L0=pca(xTr)';
errRAW=knncl( L0(1:10, :) , xTr, yTr, xTe,yTe, 1);
disp( ['pca knn train error rate: ' num2str(errRAW(1) ) ] );
disp( ['pca knn test  error rate: ' num2str(errRAW(2) ) ] );
save result_pca errRAW;

