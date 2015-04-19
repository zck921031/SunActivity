%% LMNN Demo
%
% This demo tests various flavors of LMNN
%% Initialize path anc clear screen
clear
close all

addpath '../func';
%% Load data 
%[xTrain, yTrain, xTest, yTest] = load_feature('Flare_NoFlare', 1:9, 1:3);
%[xTrain, yTrain, xTest, yTest] = load_feature_sift('SS_NoSS');
[xTrain, yTrain, xTest, yTest] = load_feature('SS_NoSS', 1:9, 1:3);
%[xTrain, yTrain, xTest, yTest] = load_feature_lbp('SS_NoSS');

%xTrain = xTrain*256;
%xTest = xTest*256;

yTrain( find(yTrain== -1) ) = 0;
yTest ( find(yTest == -1) ) = 0;

D = size(xTrain, 2);

%wave = 1;
xTr = xTrain(:, :)'; yTr = yTrain';
xTe = xTest (:, :)'; yTe = yTest';


cd '../../lmnn';
setpaths;
figh=figure ('Name','PCA');
clc;

%% KNN classification error before metric learning  
errRAW=knncl([],xTr, yTr,xTe,yTe,1);fprintf('\n');
fprintf('\n')
%% KNN classification after PCA
L0=pca(xTr)';
errPCA=knncl(L0(1:3,:),xTr, yTr,xTe,yTe,1);fprintf('\n');
subplot(1,2,1);
scat(L0*xTr,3,yTr);
title(['PCA Training (Error: ' num2str(100*errPCA(1),3) '%)'])
noticks;box on;
subplot(1,2,2);
scat(L0*xTe,3,yTe);
title(['PCA Test (Error: ' num2str(100*errPCA(2),3) '%)'])
noticks;box on;
drawnow


%% Use LMNN for dimensionality reduction
fprintf('\n')
disp('Learning initial metric with LMNN ...')
[L,~] = lmnn2(xTr, yTr,3,L0,'maxiter',1000,'quiet',1,'outdim',3,'mu',0.5,'validation',0.2,'earlystopping',25,'subsample',0.3);

%% KNN classification with 3D LMNN metric
errL=knncl(L,xTr, yTr,xTe,yTe,1);fprintf('\n');

%% Plotting LMNN embedding
figure('Name','LMNN');
subplot(1,2,1);
scat(L*xTr,3,yTr);
title(['LMNN Training (Error: ' num2str(100*errL(1),3) '%)'])
noticks;box on;
drawnow
subplot(1,2,2);
scat(L*xTe,3,yTe);
title(['LMNN Test (Error: ' num2str(100*errL(2),3) '%)'])
noticks;box on;
drawnow


%% Final Results
disp('Dimensionality Reduction Demo:');
%disp(['Under wave: ', num2str(wave)]);
disp(['1-NN Error for raw (high dimensional) input is : ',num2str(100*errRAW(2),3),'%']);
disp(['1-NN Error after PCA in 3d is : ',num2str(100*errPCA(2),3),'%']);
disp(['1-NN Error after LMNN in 3d is : ',num2str(100*errL(2),3),'%']);

figure(6);
mAP = [];
[~,~,~,mAP(1)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, eye( size(xTrain,2), size(xTrain,2) ), 'color', 'b', 'title', 'Sunspot' );
[~,~,~,mAP(2)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, L0(1:3, :), 'color', 'g', 'title', 'Sunspot' );
[~,~,~,mAP(3)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, L, 'color', 'r', 'title', 'Sunspot');
for i = 1 : 3
    subplot(2,2,i);
    h = legend('Euclidean', 'PCA', 'LMNN'); hold on;
    set(h,'box','off')
end

