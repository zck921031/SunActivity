%
clc;
clear;

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\func';
addpath 'C:\Users\zck\Desktop\metric learning\code\dca';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';

% {'Sunspot', 'Coronal Hole', 'Flare'}
%Concept = 'Sunspot';
%[ xTrain, yTrain, xTest, yTest ] = load_feature( Concept, 1:9, 1:3);

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\feature\ox5';
[xTrain, yTrain, xTest, yTest] =  load_feature_ox5( 1:3 );

xTr = xTrain'; yTr = yTrain';
xTe = xTest'; yTe = yTest';

[D, N] = size(xTr);

%yTr( find(yTr ~= 1) ) = 2;
%yTe( find(yTe ~= 1) ) = 2;

tic

%[B, DCA, newData] = DCA_func(xTr, yTr, [0,1; 0,1] );
[B, DCA, newData] = DCA_func(xTr, yTr, ones(5,5) );

errDCA = knncl(DCA, xTr, yTr, xTe,yTe, 1);
disp(['1-NN Error after DCA is : ', num2str(100*errDCA(2), 3), '%'] );

usedtime = toc;

disp( ['Elapsed time is ' num2str(usedtime) ' seconds' ] );

save dca_ox5_1 DCA usedtime;

