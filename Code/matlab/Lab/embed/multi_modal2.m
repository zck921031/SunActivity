%% mmLMNN
% Solve: Min_{A_i \in \mathbb{S}_+^dim , i=1..9} 
% f(A) = sum_{( i,j) \in S } ( d_A(x_i-x_j)^2 ) ) + sum_{( i,j) \in D } max( 0, 1 + d_A(x_i-x_j)^2 - d_A(x_i-x_k)^2 )

%% Initialize path anc clear screen
clear
close all
addpath '../func';
%% Load data 
% {'Sunspot', 'Coronal Hole', 'Flare'}
Concept = 'Flare';
[xTrain, yTrain, xTest, yTest] = load_feature( Concept, 1:9, 1:3 );

%xTrain = xTrain*256;
%xTest = xTest*256;

%addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\feature\ox5';
%[xTrain, yTrain, xTest, yTest] =  load_feature_ox5( 1:3 );

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
disp('Learning initial metric with LMNN ...');
%L0 = eye(D,D);

%part = [ 1, 16; 16+1, 16+200; 16+200+1, 16+200+59 ];
[L,~] = lmnn(xTr, yTr,3,L0,'maxiter', 20000, 'quiet', 0, 'outdim', D, 'mu',0.5,'validation',0.0,'earlystopping',25,'subsample',0.3, 'Lpart', part, 'stepgrowth', 1.05 );
%[L,~] = lmnn4(xTr, yTr,3,L0,'maxiter', 1500, 'quiet', 0, 'outdim', D, 'mu',0.5,'validation',0.0,'earlystopping',25,'subsample',0.3, 'Lpart', part, 'stepgrowth', 1.05);
%[L, Det] = lmnn3(xTr, yTr,3,L0,'maxiter', 50000,'quiet', 0,'outdim',27, 'mu',0.5,'validation',0.0,'earlystopping',25,'subsample',0.3, 'modal', 9, 'stepgrowth', 1.01);
%[L,~] = lmnn2(xTr, yTr,3,L0,'maxiter',1000,'quiet',1,'outdim',3,'mu',0.5,'validation',0.0,'earlystopping',25,'subsample',0.3, 'modal', 9, 'stepgrowth', 1.05);

%% KNN classification with 3D LMNN metric
errL=knncl(L,xTr, yTr,xTe,yTe,1);fprintf('\n');


%% Final Results
disp('Dimensionality Reduction Demo:');
%disp(['Under wave: ', num2str(wave)]);
disp(['1-NN Error for raw (high dimensional) input is : ',num2str(100*errRAW(2),3),'%']);
disp(['1-NN Error after PCA in 3d is : ',num2str(100*errPCA(2),3),'%']);
disp(['1-NN Error after LMNN in 3d is : ',num2str(100*errL(2),3),'%']);


% addpath '../lab/func';
% figure(6);
% mAP = [];
% [~,~,~,mAP(1)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, eye( size(xTrain,2), size(xTrain,2) ), 'color', 'b', 'title', Concept );
% [~,~,~,mAP(2)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, L0(1:3*9, :), 'color', 'g', 'title', Concept );
% [~,~,~,mAP(3)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, L, 'color', 'r', 'title', Concept );
% for i = 1 : 3
%     subplot(2,2,i);
%     h = legend('Euclidean', 'PCA', 'algo2'); hold on;
%     set(h,'box','off')
% end

cd ../lab/embed;

weight = [];
for modal = 1 : 9
    weight(modal) = norm( L( 3*(modal-1)+1 : 3*modal, 125*(modal-1)+1 : 125*modal ) );
end
bar(weight);

save mmLMNN_flare_5k  L Det;
% csvwrite( ['mmLMNN_' Concept '.txt'], L );

