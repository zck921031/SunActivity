%

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Fisher';

clear ;
close all;
clc;

[ xTrain, yTrain, xTest, yTest ] = load_feature_nus500();


%% LDA
yTrain = yTrain - 8;
yTest = yTest  - 8;
[W, B] = fisherbcl2(xTrain, yTrain);
err_LDA(1) = 1 - fisher_test(xTrain, yTrain, W, B);
err_LDA(2) = 1 - fisher_test(xTest, yTest, W, B);

disp( ['LDA train error rate: ' num2str( err_LDA(1) ) ] );
disp( ['LDA test  error rate: ' num2str( err_LDA(2) ) ] );

%save result_lmnn L;

