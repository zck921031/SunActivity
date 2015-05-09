%

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';
addpath 'C:\Users\zck\Desktop\metric learning\code\dca';

[ xTr, yTr, xTe, yTe ] = load_feature_nus500();

xTr = xTr'; yTr = yTr';
xTe = xTe'; yTe = yTe';
yTr = yTr - 7;
yTe = yTe - 7;
[B, DCA, newData]=DCA_func(xTr, yTr, ones(2,2) );

errRAW=knncl(DCA, xTr, yTr, xTe,yTe, 1);
disp( ['Euclidean knn train error rate: ' num2str(errRAW(1) ) ] );
disp( ['Euclidean knn test  error rate: ' num2str(errRAW(2) ) ] );

%save result_knn errRAW;

