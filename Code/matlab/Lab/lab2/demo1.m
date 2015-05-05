%
clc;
clear;
close all;

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\func';


% cd 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\Func';
% {'Sunspot', 'Coronal Hole', 'Flare'}
Concept = 'Sunspot';
[xTrain, yTrain, xTest, yTest] = load_feature( Concept, 1:9, 1:3 );
% cd 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\NUS';

yTrain( find(yTrain== -1) ) = 0;
yTest ( find(yTest == -1) ) = 0;

[N, D] = size(xTrain);

%wave = 1;
xTr = xTrain(:, :)'; yTr = yTrain';
xTe = xTest (:, :)'; yTe = yTest';
p = randperm(N);
xTr = xTr(:, p);
yTr = yTr( p );

cd '../../lmnn';
setpaths;
clc;

errRAW=knncl([],xTr, yTr,xTe,yTe,1);fprintf('\n');
fprintf('\n')
%% KNN classification after PCA
L0=pca(xTr)';
errPCA=knncl(L0(1:3,:),xTr, yTr,xTe,yTe,1);fprintf('\n');

L = L0;
err = [];
for cc = 20 : 1: size(xTr, 2)
    %% Use LMNN for dimensionality reduction
    %[L,~] = lmnn3(xTr, yTr,3,L0,'maxiter',2000,'quiet',1,'outdim',3*9,'mu',0.5,'validation',0.0,'earlystopping',25,'subsample',0.3, 'modal', 9, 'stepgrowth', 1.05);
    [L,~] = lmnn2(xTr(:, :), yTr, 3, L(1:3, :), 'maxiter', 100, 'quiet', 1, 'outdim', 3, 'mu',0.5,'validation',0.0,'earlystopping',25,'subsample',0.3, 'stepgrowth', 1.05);

    %% KNN classification with 3D LMNN metric
    errL=knncl(L, xTr(:,1:cc), yTr, xTe, yTe, 1);fprintf('\n');
    err(cc) = errL(2);
    %% Final Results
    disp( ['iters: ' num2str(cc) ] );
    disp(['1-NN Error for raw(high dimensional) input is : ',num2str(100*errRAW(2),3),'%']);
    disp(['1-NN Error after LMNN in 3d is : ',num2str(100*errL(2),3),'%']);
end
plot( err(20:end) ); hold on;


cd 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\lab2';

save result_temp L err;