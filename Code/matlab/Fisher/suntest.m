%% LMNN Demo
%
% This demo tests various flavors of LMNN
%% Initialize path anc clear screen
clear
close all

%% Load data 
%Mt = csvread('../../demo1/demo1/data.txt');
Mt = csvread('../../../data/TrainSet/data2.txt');

[N, D] = size(Mt);
D = D-2;

D = 255;

train_i = 0;
test_i = 0;
train = [];
train_label = [];
test = [];
test_label = [];
for i=1:N
    %������
    if ( Mt(i,1)~=1 ) 
        Mt(i,1)=0;
    end
    if ( 1<=i&&i<=20 || 63<=i&&i<=77 || 102<=i&&i<=108 || 138<=i&&i<=168 )
    %if (i<=218 || 684<=i&&i<=802 || 925<=i&&i<=1524 )
        train_i = train_i+1;
        train( train_i , :) = Mt(i, 2:D+1);
        train_label( train_i , 1) = Mt(i, 1);
    else
        %if ( 546<=i&&i<=646 || 848<=i&&i<=897 || 1544<=i&&i<=1746 )
        test_i = test_i+1;
        test(test_i, :) = Mt(i, 2:D+1);
        test_label(test_i,1) = Mt(i, 1);
        %end
    end
end%��ԭʼ���ݷ�Ϊtrain����test��

[w,b] = fisherbcl(train, train_label);
yp = test*w+b<0;
disp( [ 'fisher��ȷ��', num2str(sum(yp == test_label)/size(test_label,1) ) ] );

%% lmnn
cd '../lmnn';
knn=3;  % we are optimizing ver the knn=3 nearest neighbors classifier
disp(['Automatic tuning of LMNN parameters for ' num2str(knn) '-NN classification.']); 

xTr = train'; yTr = train_label';
xTe = test'; yTe = test_label';
L0=pca(xTr)';
[L,~] = lmnn2(xTr, yTr,3,L0,'maxiter',1000,'quiet',1,'outdim',3,'mu',0.5,'validation',0.2,'earlystopping',25,'subsample',0.3);

cd '../fisher';
%% fisher
[w,b] = fisherbcl(train*L', train_label);
yp = test*L'*w + b < 0;
disp( [ 'after lmnn, fisher��ȷ��', num2str(sum(yp == test_label)/size(test_label,1) ) ] );



