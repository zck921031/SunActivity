%% mmLMNN
% Solve: Min_{A_i \in \mathbb{S}_+^dim , i=1..9} 
% f(A) = sum_{( i,j) \in S } ( d_A(x_i-x_j)^2 ) ) + sum_{( i,j) \in D } max( 0, 1 + d_A(x_i-x_j)^2 - d_A(x_i-x_k)^2 )

%% Initialize path anc clear screen
clear
close all
addpath '../func';
%% Load data 
% {'Sunspot', 'Coronal Hole', 'Flare'}
Concept = 'Sunspot';
[xTrain, yTrain, xTest, yTest] = load_feature( Concept, 1:9, 1:3 );

%xTrain = xTrain*256;
%xTest = xTest*256;

yTrain( find(yTrain== -1) ) = 0;
yTest ( find(yTest == -1) ) = 0;

D = size(xTrain, 2);

%wave = 1;
xTr = xTrain(:, :)'; yTr = yTrain';
xTe = xTest (:, :)'; yTe = yTest';


addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';
addpath '..\oasis';


parms.num_steps = 3e4;
parms.use_matlab = 1;
parms.do_save = 0;
obj = oasis(xTrain, yTrain, parms);
L = LU( obj.W );

%%  Results
errRAW=knncl([], xTr, yTr,xTe,yTe, 3);
disp(['3-NN Error for raw(high dimensional) input is : ',num2str(100*errRAW(2), 3),'%']);
errRAW=knncl(L, xTr, yTr,xTe,yTe, 3);
disp(['3-NN Error after oasis input is : ',num2str(100*errRAW(2), 3),'%']);


% figure(6);
% mAP = [];
% [~,~,~,mAP(1)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, eye( size(xTrain,2), size(xTrain,2) ), 'color', 'b', 'title', Concept );
% [~,~,~,mAP(2)] = ClassAVGPlot(xTrain, yTrain, xTest, yTest, L, 'color', 'r', 'title', Concept );
% for i = 1 : 2
%     subplot(2,2,i);
%     h = legend('Euclidean', 'oasis'); hold on;
%     set(h,'box','off')
% end
