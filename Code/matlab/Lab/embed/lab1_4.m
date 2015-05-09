%% this lab simply shows the result
clear;
close all;
clc;


addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Fisher';
addpath '../func';


addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\feature\ox5';
[xTrain, yTrain, xTest, yTest] =  load_feature_ox5( 1:3 );

xTr = xTrain'; yTr = yTrain';
xTe = xTest'; yTe = yTest';

Kg = 3;
%% Euclidean
err_Euclidean = knncl([], xTr, yTr,xTe,yTe, Kg);

%% PCA
L0=pca(xTr)';
err_PCA = knncl(L0(1:27, :), xTr, yTr,xTe,yTe, Kg);

%% OASIS
load OASIS_ox5;
L_OASIS = L;
err_OASIS = knncl(L_OASIS, xTr, yTr,xTe,yTe, Kg);

%% LDA
% yTrain( find(yTrain== -1) ) = 0;
% yTest ( find(yTest == -1) ) = 0;
% [W, B] = fisherbcl2(xTrain, yTrain);
% err_LDA(cc, 1) = 1 - fisher_test(xTrain, yTrain, W, B);
% err_LDA(cc, 2) = 1 - fisher_test(xTest, yTest, W, B);


%% DCA
load dca_ox5_1;
err_DCA = knncl( DCA, xTr, yTr , xTe, yTe, Kg);

%% LMNN
load LMNN_ox5;
L_LMNN = L;
err_LMNN = knncl(L_LMNN, xTr, yTr,xTe,yTe, Kg);

%% svm
Struct = svmtrainovr(xTrain, yTrain);
C = svmclassifyovr(Struct, xTrain);
err_svmOvR(1) = 1 - sum( C == yTrain ) / length(yTrain);
C = svmclassifyovr(Struct, xTest);
err_svmOvR(2) = 1 - sum( C == yTest ) / length(yTest);

%% Me
load mmLMNN_ox5_5k;
L_me = L;
err_me = knncl(L_me, xTr, yTr,xTe,yTe, Kg);


cd 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\embed';
%% Bar
figure(1);

test_result = [];

test_result = [ test_result, err_Euclidean(2) ];
% test_result = [ test_result, err_PCA(:,2) ];
%test_result = [ test_result, err_LDA ];
test_result = [ test_result, err_DCA(2) ];
test_result = [ test_result, err_OASIS(2) ];
test_result = [ test_result, err_svmOvR(2) ];
test_result = [ test_result, err_LMNN(2) ];
test_result = [ test_result, err_me(2) ];

b = test_result;
hold on;
for i=1:length( test_result )
b(i) = bar(i, test_result(i) );
ch = get( b(i) , 'children' );
set(ch, 'FaceVertexCData', i );
end


title( 'ox5数据集 测试错误率');
%set(gca,'XTickLabel', { ' ','Euclidean', ' ','DCA', ' ','SVMOvR', ' ','OASIS'  } );

set(gca,'XTickLabel', { ' ','Euclidean',  'DCA', 'OASIS', 'SVMOvR', 'LMNN', 'Our method'} );

ylabel('Test Error');  %设置x轴和y轴的名称
legend('Euclidean', 'DCA', 'OASIS', 'SVMOvR', 'LMNN', 'Our method');

