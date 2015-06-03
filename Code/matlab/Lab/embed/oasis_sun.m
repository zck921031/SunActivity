%
clc;
clear;

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\func';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\oasis';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';

% {'Sunspot', 'Coronal Hole', 'Flare'}
Concept = 'Sunspot';
[ xTrain, yTrain, xTest, yTest ] = load_feature( Concept, 1:9, 1:3);
% cd 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\NUS';

% addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\feature\ox5';
% [xTrain, yTrain, xTest, yTest] =  load_feature_ox5( 1:3 );

xTr = xTrain'; yTr = yTrain';
xTe = xTest'; yTe = yTest';

[D, N] = size(xTr);

tic
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\NUS';
parms.use_matlab = 1;
parms.do_save = 0;
L = eye(D,D);
obj.W = L;
for cc = 1 : 10
    parms.num_steps = 1e4;
    obj = oasis(xTr', yTr', parms, obj.W );
    
    L = LU(obj.W);

    errRAW = knncl( [], L*xTr, yTr, L*xTe, yTe, 1);
    disp( ['pair_sto_con knn train error rate: ' num2str(errRAW(1) ) ] );
    disp( ['pair_sto_con knn test  error rate: ' num2str(errRAW(2) ) ] );
end
usedtime = toc;

disp( ['Elapsed time is ' num2str(usedtime) ' seconds' ] );

%save OASIS_ox5 L usedtime;

