% clc;

clear;

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\func';
% {'Sunspot', 'Coronal Hole', 'Flare'}
% Concept = 'Flare';
% [ xTrain, yTrain, xTest, yTest ] = load_feature( Concept, 1:9, 1:3);

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\feature\ox5';


%p = randperm( size(xTrain,1) );
%xTrain = xTrain( p(1:30), : );
%yTrain = yTrain( p(1:30) );

% xTr = xTr'; yTr = yTr';
% xTe = xTe'; yTe = yTe';

%[D, N] = size(xTrain);
cd 'C:\Users\zck\Desktop\MKL';
addpath( genpath(pwd) );

par = smksvm_parameter_ox5;

training_data = {};
for i = 1 : 3
    [xTrain, yTrain, xTest, yTest] = load_feature_ox5( i );
yTrain = double( yTrain==1 );
yTest = double( yTest==1 );
yTrain( find(yTrain==0) ) = -1;
yTest ( find(yTest ==0) ) = -1;
    training_data{i}.X = xTrain;
    training_data{i}.y = double( yTrain );
    training_data{i}.ind=( 1 : length(xTrain) );
    testing_data{i}.X = xTest;
    testing_data{i}.y = double (yTest);
end
tic

% svmStruct1=svmtrain( xTrain, yTrain, 'showplot',false );
% C = svmclassify(svmStruct1, xTrain, 'showplot',false) ;
% disp( ['train正确率是' num2str(  sum( C==yTrain ) / size(xTrain,1) ) ] );
% C = svmclassify(svmStruct1, xTest, 'showplot',false) ;
% disp( ['test 正确率是' num2str(  sum( C==yTest ) / size(xTest,1) ) ] );


model = smksvm_train(training_data, par );
output = smksvm_test(testing_data, model);
bj=mean(sign(output.dis)==testing_data{1}.y);
bj

usedtime = toc;

disp( ['Elapsed time is ' num2str(usedtime) ' seconds' ] );
cd 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\embed';