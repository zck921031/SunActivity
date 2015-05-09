% clc;

clear;

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\func';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\oasis';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';

% {'Sunspot', 'Coronal Hole', 'Flare'}
% Concept = 'Flare';
% [ xTrain, yTrain, xTest, yTest ] = load_feature( Concept, 1:9, 1:3);

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\feature\ox5';
[xTrain, yTrain, xTest, yTest] =  load_feature_ox5(1:3);

%p = randperm( size(xTrain,1) );
%xTrain = xTrain( p(1:30), : );
%yTrain = yTrain( p(1:30) );

% xTr = xTr'; yTr = yTr';
% xTe = xTe'; yTe = yTe';

[D, N] = size(xTrain);


tic

yTrain = yTrain==1;
yTest = yTest==1;
svmStruct1=svmtrain( xTrain, yTrain, 'showplot',false );
C = svmclassify(svmStruct1, xTrain, 'showplot',false) ;
disp( ['train正确率是' num2str(  sum( C==yTrain ) / size(xTrain,1) ) ] );
C = svmclassify(svmStruct1, xTest, 'showplot',false) ;
disp( ['test 正确率是' num2str(  sum( C==yTest ) / size(xTest,1) ) ] );

usedtime = toc;

disp( ['Elapsed time is ' num2str(usedtime) ' seconds' ] );
