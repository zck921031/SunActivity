%

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';

[ xTr, yTr, xTe, yTe ] = load_feature();
%[ xTr, yTr, xTe, yTe ] = load_feature_single(1);

xTr = xTr'; yTr = yTr';
xTe = xTe'; yTe = yTe';
errRAW=knncl([], xTr, yTr, xTe,yTe, 1);
disp( ['Euclidean knn train error rate: ' num2str(errRAW(1) ) ] );
disp( ['Euclidean knn test  error rate: ' num2str(errRAW(2) ) ] );

%save result_knn errRAW;

