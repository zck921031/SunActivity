%demo.m
clc;
close all;
clear all;

datasets = { 'corel5k' };
sets     = { 'test', 'train' };
impaths  = { 'C:\Users\zck\Desktop\metric learning\dataset\corel5k.20091111' };

[xTrain, yTrain, xTest, yTest] = load_feature(3);

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\Func';

mAP = []; ac=[]; ac2=[];

%% Euclidean: mAP = 0.284290608518864;
[~,~,~,mAP(1),ac(1),ac2(1)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, eye( size(xTrain,2), size(xTrain,2) ), 'color', 'b', 'title', 'corel5k' );


%% lmnn: 
xTr = xTrain(:, :)'; yTr = double( yTrain' );
xTe = xTest (:, :)'; yTe = double( yTest' );
cd '../../lmnn';
setpaths;
% clc;
%% KNN classification error before metric learning  
errRAW=knncl([],xTr, yTr,xTe,yTe,1);fprintf('\n');
fprintf('\n')
%% KNN classification after PCA
L0=pca(xTr)';
errPCA=knncl(L0(1:3,:),xTr, yTr,xTe,yTe,1);fprintf('\n');
[~,~,~,mAP(2),ac(2),ac2(2)]  = ClassAVGPlot(xTrain, yTrain, xTest, yTest, L0(1:10, :), 'color', 'g', 'title', 'corel5k' );


%% Use LMNN for dimensionality reduction
% fprintf('\n')
% disp('Learning initial metric with LMNN ...')
% [L,~] = lmnn2(xTr, yTr,3,L0,'maxiter', 200, 'quiet', 0, 'outdim', 3, 'mu',0.5,'validation',0.0,'earlystopping',25,'subsample', 0.1, 'stepgrowth', 1.08);
% 
% [~,~,~,mAP(3),ac(3),ac2(3)]  = ClassAVGPlot(xTrain, yTrain, xTest, yTest, L(1:3, :), 'color', 'r', 'title', 'corel5k' );

%% KNN classification with 3D LMNN metric
% errL=knncl(L,xTr, yTr,xTe,yTe,1);fprintf('\n');

%% Show Points
% figure('Name','PCA');
% subplot(1,2,1);
% scat(L0*xTr,3,yTr);
% title(['PCA Training (Error: ' num2str(100*errPCA(1),3) '%)'])
% noticks;box on;
% subplot(1,2,2);
% scat(L0*xTe,3,yTe);
% title(['PCA Test (Error: ' num2str(100*errPCA(2),3) '%)'])
% noticks;box on;
% drawnow

% figure('Name','LMNN');
% subplot(1,2,1);
% scat(L*xTr,3,yTr);
% title(['LMNN Training (Error: ' num2str(100*errL(1),3) '%)'])
% noticks;box on;
% drawnow
% subplot(1,2,2);
% scat(L*xTe,3,yTe);
% title(['LMNN Test (Error: ' num2str(100*errL(2),3) '%)'])
% noticks;box on;
% drawnow

%% Final Results
% disp('Dimensionality Reduction Demo:');
% disp(['1-NN Error for rawÂ (high dimensional) input is : ',num2str(100*errRAW(2),3),'%']);
% disp(['1-NN Error after PCA in 3d is : ',num2str(100*errPCA(2),3),'%']);
% disp(['1-NN Error after LMNN in 3d is : ',num2str(100*errL(2),3),'%']);

cd ../lab/corel5k;


addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\oasis';

parms.num_steps = 4e5;
parms.use_matlab = 1;
parms.do_save = 0;
obj = oasis(xTrain, yTrain, parms);

W = obj.W;
[V,D] = eig(0.5*(W+W'));
Loasis = ( V * sqrt( max(D, 0) ) )';
errOASIS=knncl( Loasis, xTr, yTr, xTe, yTe, 1);fprintf('\n');
[~,~,~,mAP(4),ac(4),ac2(4)]  = ClassAVGPlot(xTrain, yTrain, xTest, yTest, Loasis, 'color', 'y', 'title', 'corel5k' );
save OASIS_obj obj;

%% LDA fisher: mAP 0.389468521473775
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Fisher';
[w, b] = fisherbcl2(xTrain, yTrain);

errLDA=knncl( w', xTr, yTr, xTe, yTe, 1);fprintf('\n');
[~,~,~,mAP(5),ac(5),ac2(5)]  = ClassAVGPlot(xTrain, yTrain, xTest, yTest, w', 'color', 'r', 'title', 'LDA' );
disp( [ 'fisher²âÊÔÕýÈ·ÂÊ' , num2str(sum( ( ( xTest * w + b ) < 0 ) == yTest )/(size(xTest,1) ) ) ] );
