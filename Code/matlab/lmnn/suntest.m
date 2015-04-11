%% LMNN Demo
%
% This demo tests various flavors of LMNN
%% Initialize path anc clear screen
clear
close all
%install;
setpaths;
figh=figure ('Name','PCA');
clc;
rand('seed',1);

%% Load data 
%load data/segment.mat;
Mt = csvread('C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\data.txt');
seq = find( abs( Mt(:,1) )==1 );
Mt = Mt(seq, :);

[N, D] = size(Mt);
D = D-1;
seqD = [1, 2+16*1 : 1+16*6];
Mt = Mt(:, seqD);D=size(Mt,2)-1;

%D = 32;

train_i = 0;
test_i = 0;
train = [];
train_label = [];
test = [];
test_label = [];
for i=1:N
    %二分类
    if ( Mt(i,1)~=1 ) 
        Mt(i,1)=0;
    end
    if i <= 600
    %if ( 1<=i&&i<=20 || 63<=i&&i<=77 || 102<=i&&i<=108 || 138<=i&&i<=168 )
        %(i<=218 || 684<=i&&i<=802 || 925<=i&&i<=1524 )
        train_i = train_i+1;
        train( train_i , :) = Mt(i, 2:D+1);
        train_label( train_i , 1) = Mt(i, 1);
    else
        %if ( 546<=i&&i<=646 || 848<=i&&i<=897 || 1544<=i&&i<=1746 )
        test_i = test_i+1;
        test(test_i, :) = Mt(i, 2:D+1);
        test_label(test_i,1) = Mt(i, 1);
        %end
    end
end%将原始数据分为train集和test集
wave = 1;
xTr = train(:, :)'; yTr = train_label';
xTe = test (:, :)'; yTe = test_label';


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
disp(['Under wave: ', num2str(wave)]);
disp(['1-NN Error for raw聽(high dimensional) input is : ',num2str(100*errRAW(2),3),'%']);
disp(['1-NN Error after PCA in 3d is : ',num2str(100*errPCA(2),3),'%']);
disp(['1-NN Error after LMNN in 3d is : ',num2str(100*errL(2),3),'%']);
disp(['1-NN Error after gbLMNN in 3d is : ',num2str(100*errGL(2),3),'%']);

addpath('../fisher');

[w, b] = fisherbcl( train, uint8(train_label) );
disp( [ 'Raw, fisher测试正确率' , num2str(sum( ( ( test*w+b) < 0 ) == test_label )/(size(test,1) ) ) ] );

[w, b] = fisherbcl( train*L', uint8(train_label) );
disp( [ 'After lmnn, fisher测试正确率' , num2str(sum( ( ( test*L'*w+b) < 0 ) == test_label )/(size(test,1) ) ) ] );


