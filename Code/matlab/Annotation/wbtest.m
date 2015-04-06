%the script is written for test
clc;
clear;
close all;

load '../FeatureExtraction/train';
addpath( '../fisher' );
data = data(:, 1:16);
label = (label==1);
[w,b] = fisherbcl(data, label);

cd '../lmnn';
setpaths;
knn=3;  % we are optimizing ver the knn=3 nearest neighbors classifier
disp(['Automatic tuning of LMNN parameters for ' num2str(knn) '-NN classification.']); 

xTr = data'; yTr = label';
%xTe = test'; yTe = test_label';
L0=pca(xTr)';

[L,~] = lmnn2(xTr, yTr,3,L0,'maxiter',1000,'quiet',1,'outdim',3,'mu',1.0,'validation',0.2,'earlystopping',25,'subsample',1.0);


subplot(1,2,1);
scat(L0*xTr,3,yTr);
title('PCA Training');
noticks;box on;
subplot(1,2,2);
scat(L*xTr,3,yTr);

%% fisher
cd '../fisher';
[w,b] = fisherbcl(data*L', label);
