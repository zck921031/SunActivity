%% LMNN Demo
%
% This demo tests various flavors of LMNN
%% Initialize path anc clear screen
clear
close all

%% Load data 
%load data/segment.mat;
[xTrain, yTrain, xTest, yTest] = load_feature_lbp('CH_NoCH');

xTrain = xTrain*256;
xTest = xTest*256;
yTrain( find(yTrain== -1) ) = 0;
yTest ( find(yTest == -1) ) = 0;

D = size(xTrain, 2);

%wave = 1;
xTr = xTrain(:, :)'; yTr = yTrain';
xTe = xTest (:, :)'; yTe = yTest';


%install;
addpath '../../fisher';
cd '../../lmnn';
setpaths;
figh=figure ('Name','PCA');
clc;
rand('seed',1);

%% KNN classification error before metric learning  
errRAW=knncl([],xTr, yTr,xTe,yTe,1);fprintf('\n');
fprintf('\n')
errRAW
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


%% Gradient boosted LMNN (gbLMNN)
fprintf('\n')
disp('Learning nonlinear metric with GB-LMNN ... ')
embed=gb_lmnn(xTr,yTr,3,L,'ntrees',200,'verbose',false);

%% KNN classification error after metric learning using gbLMNN
errGL=knncl([],embed(xTr), yTr,embed(xTe),yTe,1);fprintf('\n');
figure('Name','GB-LMNN');
subplot(1,2,1);
scat(embed(xTr),3,yTr);
title(['GB-LMNN Training (Error: ' num2str(100*errGL(1),3) '%)'])
noticks;box on;
drawnow
subplot(1,2,2);
scat(embed(xTe),3,yTe);
title(['GB-LMNN Test (Error: ' num2str(100*errGL(2),3) '%)'])
noticks;box on;
drawnow


%% Final Results
disp('Dimensionality Reduction Demo:');
%disp(['Under wave: ', num2str(wave)]);
disp(['1-NN Error for raw聽(high dimensional) input is : ',num2str(100*errRAW(2),3),'%']);
disp(['1-NN Error after PCA in 3d is : ',num2str(100*errPCA(2),3),'%']);
disp(['1-NN Error after LMNN in 3d is : ',num2str(100*errL(2),3),'%']);
disp(['1-NN Error after gbLMNN in 3d is : ',num2str(100*errGL(2),3),'%']);

addpath('../fisher');

[w, b] = fisherbcl( xTrain, uint8( yTrain ) );
disp( [ 'Raw, fisher测试正确率' , num2str(sum( ( ( xTest * w + b ) < 0 ) == yTest )/(size(xTest,1) ) ) ] );

[w, b] = fisherbcl( xTrain * L0(1:3, :)', uint8( yTrain ) );
disp( [ 'After PCA, fisher测试正确率' , num2str(sum( ( ( xTest * L0(1:3, :)'* w + b ) < 0 ) == yTest )/(size(xTest,1) ) ) ] );

[w, b] = fisherbcl( xTrain*L', uint8( yTrain ) );
disp( [ 'After lmnn, fisher测试正确率' ,  num2str(sum( ( ( xTest * L' * w + b ) < 0 ) == yTest )/(size(xTest,1) ) ) ] );


