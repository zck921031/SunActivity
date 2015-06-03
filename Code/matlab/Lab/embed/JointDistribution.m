%% LMNN Demo
%
% This demo tests various flavors of LMNN
%% Initialize path anc clear screen
clear
close all

addpath '../func';
%% Load data 

% {'Sunspot', 'Coronal Hole', 'Flare',  '3'}
Concept = 'Flare';
[xTrain, yTrain, xTest, yTest] = load_feature( Concept, 1:9, 1:3);
D = 3;
% yTrain( find(yTrain== -1) ) = 0;
% yTest ( find(yTest == -1) ) = 0;
% 
% addpath 'C:\Users\zck\Desktop\zflower-ljq';
% [xTrain, yTrain, xTest, yTest] = load_feature_flower();

% addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\feature\ox5';
% [xTrain, yTrain, xTest, yTest] =  load_feature_ox5( 1:3 );
% D = size(xTrain, 2);

%wave = 1;
xTr = xTrain'; yTr = yTrain';
xTe = xTest'; yTe = yTest';


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

tic
[L,~] = lmnn2(xTr, yTr,3, L0,'maxiter', 50000, 'quiet', 0, 'outdim', D, 'mu', 0.5, 'validation', 0.0, 'earlystopping', 250, 'subsample', 0.3);
usedtime = toc;

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
disp(['1-NN Error for raw(high dimensional) input is : ',num2str(100*errRAW(2),3),'%']);
disp(['1-NN Error after PCA in 3d is : ',num2str(100*errPCA(2),3),'%']);
disp(['1-NN Error after LMNN in 3d is : ',num2str(100*errL(2),3),'%']);

cd ../lab/embed;

% figure(6);
% mAP = [];
% [~,~,~,mAP(1)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, eye( size(xTrain,2), size(xTrain,2) ), 'color', 'b', 'title', 'Sunspot' );
% [~,~,~,mAP(2)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, L0(1:3, :), 'color', 'g', 'title', 'Sunspot' );
% [~,~,~,mAP(3)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, L, 'color', 'r', 'title', 'Sunspot');
% for i = 1 : 3
%     subplot(2,2,i);
%     h = legend('Euclidean', 'PCA', 'LMNN'); hold on;
%     set(h,'box','off')
% end

% save 'LMNN_SS' L usedtime;