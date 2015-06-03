%% mmLMNN
% Solve: Min_{A_i \in \mathbb{S}_+^dim , i=1..9} 
% f(A) = sum_{( i,j) \in S } ( d_A(x_i-x_j)^2 ) ) + sum_{( i,j) \in D } max( 0, 1 + d_A(x_i-x_j)^2 - d_A(x_i-x_k)^2 )

%% Initialize path anc clear screen
clear
close all
addpath '../func';
%% Load data 

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\feature\ox5';
[xTrain, yTrain, xTest, yTest] =  load_feature_ox5( 1:3 );

D = size(xTrain, 2);

xTr = xTrain(:, :)'; yTr = yTrain';
xTe = xTest (:, :)'; yTe = yTest';


cd '../../lmnn';
setpaths;
clc;

%% KNN classification error before metric learning  
errRAW=knncl([],xTr, yTr,xTe,yTe,1);fprintf('\n');
fprintf('\n')
%% KNN classification after PCA
L0=pca(xTr)';
errPCA=knncl(L0(1:3,:),xTr, yTr,xTe,yTe,1);fprintf('\n');


%% Use LMNN for dimensionality reduction
fprintf('\n')
disp('Learning initial metric with mmLMNN ...');
L0 = eye(D,D);

part = [ 1, 16, 1, 16; 16+1, 16+200, 16+1, 16+200; 16+200+1, 16+200+59, 16+200+1, 16+200+59 ];
tic
% [L, Det] = lmnn4(xTr, yTr, 3, L0,'maxiter', 10000, 'quiet', 0, 'mu',0.5,'validation',0.0,'earlystopping',25,'subsample',0.3, 'Lpart', part, 'stepgrowth', 1.01 );

[L, Det] = Algorithm2(xTr, yTr, 3, L0,'maxiter', 5000, 'quiet', 0, 'outdim', D, 'mu',0.5,'validation',0.0,'earlystopping',25,'subsample',0.3, 'Lpart', part, 'stepgrowth', 1.05, 'obj', 0);
% [L,~] = lmnn2(xTr, yTr,3,L0,'maxiter', 10000, 'quiet', 0, 'outdim', D, 'mu',0.5,'validation',0.0,'earlystopping',25,'subsample',0.3, 'Lpart', part, 'stepgrowth', 1.05);


time = toc;
disp(time);
%[L,~] = lmnn4(xTr, yTr,3,L0,'maxiter', 1500, 'quiet', 0, 'outdim', D, 'mu',0.5,'validation',0.0,'earlystopping',25,'subsample',0.3, 'Lpart', part, 'stepgrowth', 1.05);


%% KNN classification with 3D LMNN metric
errL=knncl(L,xTr, yTr,xTe,yTe,1);fprintf('\n');

%% Final Results
disp(['1-NN Error after mmLMNN is : ',num2str(100*errL(2),3),'%']);

cd ../lab/embed;

% weight = [];
% for modal = 1 : 9
%     weight(modal) = norm( L( 3*(modal-1)+1 : 3*modal, 125*(modal-1)+1 : 125*modal ) );
% end
% bar(weight);

% save mmLMNN_ox5_5k  L Det;

err = zeros(1,50);
for tim = 1 : 50
    if ( tim > length( Det.log.L ) )
        err(tim) = err(tim-1);
    else
        tmp = knncl(Det.log.L{tim}, xTr, yTr, xTe, yTe, 1); 
        err (tim) = tmp(2);
    end
end
plot( err, 'r' );  hold on;
plot( err, 'r*' );  hold on;

