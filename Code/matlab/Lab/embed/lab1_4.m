%% this lab simply shows the result
clear;
close all;
clc;

Kg = 11;

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Fisher';
addpath '../func';


addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\feature\ox5';
[xTrain, yTrain, xTest, yTest] =  load_feature_ox5( 1:3 );

xTr = xTrain'; yTr = yTrain';
xTe = xTest'; yTe = yTest';

%% Euclidean
tic;
[err_Euclidean, det] = knncl([], xTr, yTr,xTe,yTe, Kg);
time = toc;
apr_Euclidean = [1-err_Euclidean(2), det.P, det.R, time];

%% PCA
L0=pca(xTr)';
tic;
[err_PCA, det] = knncl(L0(1:27, :), xTr, yTr,xTe,yTe, Kg);
time = toc;
apr_PCA = [1-err_PCA(2), det.P, det.R, time];

%% OASIS
load OASIS_ox5;
L_OASIS = L;
tic;
[err_OASIS, det] = knncl(L_OASIS, xTr, yTr,xTe,yTe, Kg);
time = toc;
apr_OASIS = [1-err_OASIS(2), det.P, det.R, time];

%% LDA
% yTrain( find(yTrain== -1) ) = 0;
% yTest ( find(yTest == -1) ) = 0;
% [W, B] = fisherbcl2(xTrain, yTrain);
% err_LDA(cc, 1) = 1 - fisher_test(xTrain, yTrain, W, B);
% err_LDA(cc, 2) = 1 - fisher_test(xTest, yTest, W, B);

%% NCA
L_NCA = load('NCA_OX5.txt');
tic;
[err_NCA, det] = knncl( L_NCA, xTr, yTr , xTe, yTe, Kg);
time = toc;
apr_NCA = [1-err_NCA(2), det.P, det.R, time];

%% DCA
load dca_ox5_1;
tic;
[err_DCA, det] = knncl( DCA, xTr, yTr , xTe, yTe, Kg);
time = toc;
arp_DCA = [1-err_DCA(2), det.P, det.R, time];


%% LMNN
load LMNN_ox5;
L_LMNN = L;
tic;
[err_LMNN, det] = knncl(L_LMNN, xTr, yTr,xTe,yTe, Kg);
time = toc;
apr_LMNN = [1-err_LMNN(2), det.P, det.R, time];

%% svm
Struct = svmtrainovr(xTrain, yTrain);
C = svmclassifyovr(Struct, xTrain);
err_svmOvR(1) = 1 - sum( C == yTrain ) / length(yTrain);
tic;
C = svmclassifyovr(Struct, xTest);
err_svmOvR(2) = 1 - sum( C == yTest ) / length(yTest);
svmtime = toc;

%% Me
load mmLMNN_ox5_3k;
L_me = L;
tic;
[err_me, det] = knncl(L_me, xTr, yTr,xTe,yTe, Kg);
time  = toc;
apr_me = [1-err_me(2), det.P, det.R, time];


cd 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\embed';
%% Bar
figure(1);

test_result = [];

test_result = [ test_result, err_Euclidean(2) ];
% test_result = [ test_result, err_PCA(:,2) ];
%test_result = [ test_result, err_LDA ];
test_result = [ test_result, err_NCA(2) ];
test_result = [ test_result, err_DCA(2) ];
test_result = [ test_result, err_OASIS(2) ];
test_result = [ test_result, err_LMNN(2) ];
test_result = [ test_result, err_me(2) ];
test_result = [ test_result, err_svmOvR(2) ];

%% show acc
%subplot(2,1,1);
b = test_result;
hold on;
for i=1:length( test_result )
b(i) = bar(i, 1-test_result(i) );
ch = get( b(i) , 'children' );
set(ch, 'FaceVertexCData', i );
end
title( ['ox5数据集 测试正确率 K=', num2str(Kg)]);
%set(gca,'XTickLabel', { ' ','Euclidean', ' ','DCA', ' ','SVMOvR', ' ','OASIS'  } );

set(gca,'XTickLabel', { ' ','Euclidean', 'NCA', 'DCA', 'OASIS', 'LMNN', 'Our method', 'SVMOvR'} );

ylabel('测试正确率');  %设置x轴和y轴的名称
legend('Euclidean', 'NCA', 'DCA', 'OASIS', 'LMNN', 'Our method', 'SVMOvR');

apr = [apr_Euclidean; apr_NCA; arp_DCA; apr_OASIS;  apr_LMNN; apr_me];

%% show time
figure(2);
%subplot(2,1,2);
test_result = [apr(:,4); svmtime];
b = test_result;
hold on;
for i=1:length( test_result )
b(i) = bar(i, test_result(i) );
ch = get( b(i) , 'children' );
set(ch, 'FaceVertexCData', i );
end
title( ['ox5数据集 测试时间 K=', num2str(Kg)]);
set(gca,'XTickLabel', { ' ','Euclidean', 'NCA', 'DCA', 'OASIS', 'LMNN', 'Our method', 'SVMOvR'} );
ylabel('测试时间(s)');  %设置x轴和y轴的名称
legend('Euclidean', 'NCA', 'DCA', 'OASIS', 'LMNN', 'Our method', 'SVMOvR');