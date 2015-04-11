%% LMNN+Fisher Demo
%% Initialize path anc clear screen
clear
clc
close all

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Fisher';
%% Load data 
%Mt = csvread('../../demo1/demo1/data.txt');
Mt = csvread('C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\data.txt');

[N, D] = size(Mt);
D = D-1;

%D = 48;

train_i = 0;
test_i = 0;
train = [];
train_label = [];
test = [];
test_label = [];
for i=1:N
    %二分类
    if ( Mt(i,1)~=1 )
        Mt(i,1)=0;
    end
    if i < 350
    %if ( 1<=i&&i<=20 || 63<=i&&i<=77 || 102<=i&&i<=108 || 138<=i&&i<=168 )
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
end%将原始数据分为train集和test集

[w,b] = fisherbcl(train, train_label);
yp = test*w+b<0;
disp( [ 'fisher正确率', num2str(sum(yp == test_label)/size(test_label,1) ) ] );

%% lmnn
cd '../lmnn';
%install;
setpaths;
knn=3;  % we are optimizing ver the knn=3 nearest neighbors classifier
disp(['Automatic tuning of LMNN parameters for ' num2str(knn) '-NN classification.']); 

xTr = train'; yTr = train_label';
xTe = test'; yTe = test_label';
L0=pca(xTr)';

[w,b] = fisherbcl(train*L0(1:3, :)', train_label);
yp = test*L0(1:3, :)'*w + b < 0;
disp( [ 'after pca, fisher正确率', num2str(sum(yp == test_label)/size(test_label,1) ) ] );

[L,~] = lmnn2(xTr, yTr, 6, L0,'maxiter',1000,'quiet',1,'outdim',3,'mu',0.5,'validation',0.2,'earlystopping',25,'subsample',0.3);
testerr=knncl(L,xTr,yTr,xTe,yTe,3,'train',0);
fprintf('testing error=%2.4f\n',testerr);

figure(1);
subplot(1,2,1);
scat(L0*xTr, 3, yTr);
noticks;box on;
drawnow
subplot(1,2,2);
scat(L0*xTe, 3, yTe);

figure(2);
subplot(1,2,1);
scat(L*xTr, 3, yTr);
noticks;box on;
drawnow
subplot(1,2,2);
scat(L*xTe, 3, yTe);


cd '../fisher';
%% fisher
[w,b] = fisherbcl(train*L', train_label);
yp = test*L'*w + b < 0;
disp( [ 'after lmnn, fisher正确率', num2str(sum(yp == test_label)/size(test_label,1) ) ] );



