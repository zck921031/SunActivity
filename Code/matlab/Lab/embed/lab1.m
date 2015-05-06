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
err_me = zeros(length(CCS), 2);
err_LMNN = zeros(length(CCS), 2);
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
    
    %% Me    
    switch cc
        case 1
            load mmLMNN_Sunspot;
        case 2
            load 'mmLMNN_Coronal Hole';
        case 3
            load mmLMNN_Flare;     
    end
    L_me = L;    
    err_me(cc, :) = knncl(L_me, xTr, yTr,xTe,yTe,1);
end

cd 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\embed';
%% Bar
figure(1);

test_result = [];
test_result = test_result + 1e-4;

test_result = [ test_result, err_Euclidean(:,2) ];
% test_result = [ test_result, err_PCA(:,2) ];
test_result = [ test_result, err_LDA(:,2) ];
test_result = [ test_result, err_OASIS(:,2) ];
test_result = [ test_result, err_LMNN(:,2) ];
test_result = [ test_result, err_me(:,2) ];
bar(test_result);
title( '���������');
set(gca,'XTickLabel', {'Sunspot','Coronal Hole','Flare' }) %����x���������ʱ��
ylabel('Test Error')  %����x���y�������
legend('Euclidean', 'LDA', 'OASIS', 'LMNN', 'Our method')  %����һ����ɫ�ͺ�ɫ�ֱ����ʲô

