%% mmLMNN
% Solve: Min_{A_i \in \mathbb{S}_+^dim , i=1..9} 
% f(A) = sum_{( i,j) \in S } ( d_A(x_i-x_j)^2 ) ) + sum_{( i,j) \in D } max( 0, 1 + d_A(x_i-x_j)^2 - d_A(x_i-x_k)^2 )

%% Initialize path anc clear screen
clear
close all
addpath '../func';
%% Load data 
Concept = 'Flare';
load mmLMNN_Flare;
%[xTrain, yTrain, xTest, yTest] = load_feature('Flare_NoFlare', 1:9, 1:3);
%[xTrain, yTrain, xTest, yTest] = load_feature_sift('SS_NoSS');
[xTrain, yTrain, xTest, yTest] = load_feature([Concept '_No' Concept], 1:9, 1:3);
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
clc;

%% KNN classification error before metric learning  
errRAW=knncl([],xTr, yTr,xTe,yTe,1);fprintf('\n');
fprintf('\n')
%% KNN classification after PCA
L0=pca(xTr)';
errPCA=knncl(L0(1:3,:),xTr, yTr,xTe,yTe,1);fprintf('\n');


%% Use LMNN for dimensionality reduction
fprintf('\n')
disp('Learning initial metric with LMNN ...')
%[L,~] = lmnn3(xTr, yTr,3,L0,'maxiter',2000,'quiet',1,'outdim',3*9,'mu',0.5,'validation',0.0,'earlystopping',25,'subsample',0.3, 'modal', 9, 'stepgrowth', 1.05);

%% KNN classification with 3D LMNN metric
errL=knncl(L,xTr, yTr,xTe,yTe,1);fprintf('\n');


%% Final Results
disp('Dimensionality Reduction Demo:');
%disp(['Under wave: ', num2str(wave)]);
disp(['1-NN Error for raw (high dimensional) input is : ',num2str(100*errRAW(2),3),'%']);
disp(['1-NN Error after PCA in 3d is : ',num2str(100*errPCA(2),3),'%']);
disp(['1-NN Error after LMNN in 3d is : ',num2str(100*errL(2),3),'%']);


addpath '../lab/func';
figure(6);
mAP = [];
[~,~,~,mAP(1)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, eye( size(xTrain,2), size(xTrain,2) ), 'color', 'b', 'title', Concept );
[~,~,~,mAP(2)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, L0(1:3*9, :), 'color', 'g', 'title', Concept );
[~,~,~,mAP(3)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, L, 'color', 'r', 'title', Concept );
for i = 1 : 3
    subplot(2,2,i);
    h = legend('Euclidean', 'PCA', 'algo2'); hold on;
    set(h,'box','off')
end

cd ../lab/embed;
