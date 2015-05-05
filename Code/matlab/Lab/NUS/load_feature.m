%% NUS-WIDE data set, 
function [xTrain, yTrain, xTest, yTest] = load_feature()
    load nus-dataset;
    idx = find(yTrain < 6);
    xTrain = xTrain( idx, : );
    yTrain = yTrain( idx );
    
    idx = find(yTest < 6);
    xTest = xTest( idx, : );
    yTest = yTest( idx );
end

% function [xTrain, yTrain, xTest, yTest] = load_feature()
%     addpath 'C:\Users\zck\Documents\GitHub\SunActivity\data\多模态数据集\NUS\OBJECT\low level features';
%     a=load('Train_CH.txt'); b=load('Train_CM55.txt'); c=load('Train_CORR.txt'); d=load('Train_EDH.txt'); e=load('Train_WT.txt');
%     xTrain =  [ a(:, 1:end-1), b(:, 1:end-1), c(:, 1:end-1), d(:, 1:end-1), e(:, 1:end-1)];
%     a=load('Test_CH.txt');  b=load('Test_CM55.txt');  c=load('Test_CORR.txt');  d=load('Test_EDH.txt');  e=load('Test_WT.txt');
%     xTest  =  [ a(:, 1:end-1), b(:, 1:end-1), c(:, 1:end-1), d(:, 1:end-1), e(:, 1:end-1)];
%     load 'C:\Users\zck\Documents\GitHub\SunActivity\data\多模态数据集\NUS\OBJECT\ground truth\nuslabel.mat';
%     yTrain = label(1 : length(xTrain) );
%     yTest  = label( length(xTrain)+1 : length(xTrain)+length(xTest) );
% end