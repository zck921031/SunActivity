%

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';

[ xTr, yTr, xTe, yTe ] = load_feature_nus500();
%[ xTr, yTr, xTe, yTe ] = load_feature_single(1);

xTr = xTr'; yTr = yTr';
xTe = xTe'; yTe = yTe';
[D,N] = size(xTr);
[L, Det] = lmnn2(xTr, yTr,3, eye(D,D), 'maxiter', 1000, 'quiet', 0, 'outdim', D, 'mu',0.5,'validation',0.0,'earlystopping',25,'subsample',0.3, 'stepgrowth', 1.01);
errRAW=knncl(L, xTr, yTr, xTe,yTe, 1);
disp( ['Euclidean knn train error rate: ' num2str(errRAW(1) ) ] );
disp( ['Euclidean knn test  error rate: ' num2str(errRAW(2) ) ] );

%save result_lmnn L;

