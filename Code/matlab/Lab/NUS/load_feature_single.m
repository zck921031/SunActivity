%% NUS-WIDE data set, 
function [xTrain, yTrain, xTest, yTest] = load_feature_single(k)

    addpath 'C:\Users\zck\Documents\GitHub\SunActivity\data\多模态数据集\NUS\OBJECT\low level features';
    if k == 1
        xTrain = load('Train_CH.txt');
        xTest = load('Test_CH.txt');
    else if k == 2            
        xTrain = load('Train_CORR.txt');
        xTest = load('Test_CORR.txt');
    else if k == 3
        xTrain = load('Train_EDH.txt');
        xTest = load('Test_EDH.txt');
    else if k == 4
        xTrain = load('Train_WT.txt');
        xTest = load('Test_WT.txt'); 
        end
      end
     end
    end
    load 'C:\Users\zck\Documents\GitHub\SunActivity\data\多模态数据集\NUS\OBJECT\ground truth\nuslabel.mat';
    yTrain = label(1 : length(xTrain) );
    yTest  = label( length(xTrain)+1 : length(xTrain)+length(xTest) );
end