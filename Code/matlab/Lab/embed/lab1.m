%% this lab simply shows the result
clear;
close all;
clc;


addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Fisher';
addpath '../func';


CCS = {'Sunspot', 'Coronal Hole', 'Flare'};
err_OASIS = zeros(length(CCS), 2);
err_Euclidean = zeros(length(CCS), 2);
err_PCA = zeros(length(CCS), 2);
err_LDA = zeros(length(CCS), 2);
err_DCA = zeros(length(CCS), 2);
err_me = zeros(length(CCS), 2);
err_LMNN = zeros(length(CCS), 2);
err_svm = zeros(length(CCS), 2);
for cc = 1 : length(CCS)    
    Concept = CCS{cc};
    [xTrain, yTrain, xTest, yTest] = load_feature( Concept, 1:9, 1:3);
    xTr = xTrain'; yTr = yTrain';
    xTe = xTest'; yTe = yTest';
    
    %% Euclidean
    err_Euclidean(cc, :) = knncl([], xTr, yTr,xTe,yTe,1);
    
    %% PCA
    L0=pca(xTr)';
    err_PCA(cc, :) = knncl(L0(1:27, :), xTr, yTr,xTe,yTe,1);
    
    %% OASIS
    switch cc
        case 1
            load OASIS_SS;
        case 2
            load OASIS_CH;
        case 3
            load OASIS_FL;            
    end
    L_OASIS = L;    
    err_OASIS(cc, :) = knncl(L_OASIS, xTr, yTr,xTe,yTe,1);
    
    %% LDA
    yTrain( find(yTrain== -1) ) = 0;
    yTest ( find(yTest == -1) ) = 0;
    [W, B] = fisherbcl2(xTrain, yTrain);
    err_LDA(cc, 1) = 1 - fisher_test(xTrain, yTrain, W, B);
    err_LDA(cc, 2) = 1 - fisher_test(xTest, yTest, W, B);
    
    
    %% DCA
    yTrain( find(yTrain== -1) ) = 2;
    yTest ( find(yTest == -1) ) = 2;
    switch cc
        case 1
            load dca_SS_1;
        case 2
            load dca_CH_1;
        case 3
            load dca_FL_1;
    end
    err_DCA(cc, :) = knncl( DCA, xTr, yTrain', xTe, yTest', 1);
    
    %% LMNN
     switch cc
        case 1
            load LMNN_Sunspot;
        case 2
            load 'LMNN_Coronal Hole';
        case 3
            load LMNN_Flare;
    end
    L_LMNN = L;    
    err_LMNN(cc, :) = knncl(L_LMNN, xTr, yTr,xTe,yTe,1);
    
    %% svm
    svmStruct1=svmtrain( xTrain, yTrain, 'showplot',false );
    C = svmclassify(svmStruct1, xTrain, 'showplot',false) ;
    err_svm(cc, 1) = 1 - sum( C==yTrain ) / size(xTrain,1) ;
    C = svmclassify(svmStruct1, xTest, 'showplot',false) ;
    err_svm(cc, 2) = 1 - sum( C==yTest ) / size(xTest,1);
    C = svmclassify(svmStruct1, xTest, 'showplot',false) ;
    disp( ['svm train正确率是' num2str(  err_svm(cc, 1) ) ] );
    disp( ['svm test 正确率是' num2str(  err_svm(cc, 2) ) ] );
    
    %% Me    
    switch cc
        case 1
            load mmLMNN_Sunspot;
            load mmLMNN_SS_2k;
        case 2
            load 'mmLMNN_Coronal Hole';
            load 'mmLMNN_CH_2k';
        case 3
            load mmLMNN_Flare;
            load mmLMNN_flare_2k;
    end
    L_me = L;
    err_me(cc, :) = knncl(L_me, xTr, yTr,xTe,yTe,1);
end

cd 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\embed';
%% Bar
figure(1);

test_result = [];
%test_result = test_result + 1e-4;

test_result = [ test_result, err_Euclidean(:,2) ];
% test_result = [ test_result, err_PCA(:,2) ];
test_result = [ test_result, err_LDA(:,2) ];
test_result = [ test_result, err_DCA(:,2) ];
test_result = [ test_result, err_svm(:,2) ];
test_result = [ test_result, err_OASIS(:,2) ];
test_result = [ test_result, err_LMNN(:,2) ];
test_result = [ test_result, err_me(:,2) ];
bar(test_result);
title( '分类错误率');
set(gca,'XTickLabel', {'Sunspot','Coronal Hole','Flare' } ) %设置x轴所代表大时间
ylabel('Test Error')  %设置x轴和y轴的名称
legend('Euclidean', 'LDA', 'DCA', 'SVM', 'OASIS', 'LMNN', 'Our method');




