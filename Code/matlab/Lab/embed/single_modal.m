%% LMNN Demo
%
% This demo tests various flavors of LMNN
%% Initialize path anc clear screen
clear
close all
clc

addpath '../func';
cd '../../lmnn';
setpaths;
%%run independent
mAP = zeros(9,3,3);
ac  = zeros(9,3,3);
%Lt = zeros(9, 3);
linxTr = [], linyTr = [];
linxTe = [], linyTe = [];
for modal = 1 : 9
    for feature = 1 : 1
    %% Load data 
    [xTrain, yTrain, xTest, yTest] = load_feature('SS_NoSS', modal, 1:3);

    %xTrain = xTrain*256;
    %xTest = xTest*256;

    yTrain( find(yTrain~= 1) ) = 0;
    yTest ( find(yTest ~= 1) ) = 0;

    D = size(xTrain, 2);

    %wave = 1;
    xTr = xTrain(:, :)'; yTr = yTrain';
    xTe = xTest (:, :)'; yTe = yTest';


    L0=pca(xTr)';

    [L,~] = lmnn2(xTr, yTr,3,L0,'maxiter',1000,'quiet',1,'outdim',3,'mu',0.5,'validation',0.2,'earlystopping',25,'subsample',0.3);

    
    [~,~,~,mAP(modal, feature, 1), ac(modal, feature, 1) ] = ClassAVGPR(xTrain, yTrain, xTest, yTest, eye( size(xTrain,2), size(xTrain,2) ) );
    [~,~,~,mAP(modal, feature, 2), ac(modal, feature, 2) ] = ClassAVGPR(xTrain, yTrain, xTest, yTest, L0(1:3, :)  );
    [~,~,~,mAP(modal, feature, 3), ac(modal, feature, 3) ] = ClassAVGPR(xTrain, yTrain, xTest, yTest, L );
%     for i = 1 : 3
%         subplot(2,2,i);
%         h = legend('Euclidean', 'PCA', 'LMNN'); hold on;
%         set(h,'box','off')
%     end
    linxTr = [linxTr; L*xTr];
    linxTe = [linxTe; L*xTe];
    
    linyTr = yTr;
    linyTe = yTe;    
    end
end


L0=pca(linxTr)';
errPCA=knncl(L0(1:3,:),linxTr, linyTr, linxTe, linyTe,1);fprintf('\n');
subplot(1,2,1);
scat(L0*linxTr,3, linyTr);
title(['PCA Training (Error: ' num2str(100*errPCA(1),3) '%)']);
noticks;box on;
subplot(1,2,2);
scat(L0*linxTe, 3, linyTe);
title(['PCA Test (Error: ' num2str(100*errPCA(2),3) '%)'])
noticks;box on;
drawnow

[L,~] = lmnn2(linxTr, linyTr, 3, 'diagonal', 1, 'maxiter',10000,'quiet',1, 'mu',0.5,'validation',0.2,'earlystopping',25,'subsample',1.0);
errL=knncl(L, linxTr, linyTr, linxTe, linyTe, 1);fprintf('\n');
%% Plotting LMNN embedding
figure('Name','LMNN');
subplot(1,2,1);
scat(L*linxTr, 3, linyTr);
title(['LMNN Training (Error: ' num2str(100*errL(1),3) '%)'])
noticks;box on;
drawnow
subplot(1,2,2);
scat(L * linxTe, 3, linyTe);
title(['LMNN Test (Error: ' num2str(100*errL(2),3) '%)'])
noticks;box on;
drawnow

figure(6);
ClassAVGPlot(linxTr', linyTr', linxTe', linyTe', eye( size(linxTr, 1), size(linxTr, 1) ), 'color', 'b', 'title', 'Sunspot' );
ClassAVGPlot(linxTr', linyTr', linxTe', linyTe', L0(1:3, :), 'color', 'g', 'title', 'Sunspot' );
ClassAVGPlot(linxTr', linyTr', linxTe', linyTe', L, 'color', 'r', 'title', 'Sunspot');
for i = 1 : 3
    subplot(2,2,i);
    h = legend('Euclidean', 'PCA', 'LMNN'); hold on;
    set(h,'box','off')
end
