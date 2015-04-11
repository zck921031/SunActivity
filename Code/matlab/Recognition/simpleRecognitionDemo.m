%
clc;
clear all;
close all;
%addpath('../FeatureExtraction');
addpath('../Fisher');
%% Load: data, label
%load data.mat;
Mt = csvread('C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\data.txt');
seq = find( abs( Mt(:,1) )==1 );
Mt = Mt(seq, :);
[N, D] = size(Mt);
D = D-1;

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
    if i <= 900
    %if ( 1<=i&&i<=20 || 63<=i&&i<=77 || 102<=i&&i<=108 || 138<=i&&i<=168 )
        %(i<=218 || 684<=i&&i<=802 || 925<=i&&i<=1524 )
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


%% Read a image
imagenames = {'20110809_032451_4096_0094.jpg', '20110809_032447_4096_0131.jpg', '20110809_032425_4096_0171.jpg', ...
    '20110809_032432_4096_0193.jpg', '20110809_032450_4096_0211.jpg', '20110809_032457_4096_0304.jpg', '20110809_032453_4096_0335.jpg', ...
    '20110809_032506_4096_1600.jpg', '20110809_032520_4096_1700.jpg'};

%C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\img\None\93
wavecnt = 9;

images = {};
for k = 1 : wavecnt
    imagename = ['C:\Users\zck\Documents\GitHub\SunActivity\data\regional annotation\img\', imagenames{k}];
    image_tmp = imread( imagename );
    images{k} = image_tmp;
end

%% lmnn
cd '../lmnn';
setpaths;
knn=3;  % we are optimizing ver the knn=3 nearest neighbors classifier
disp(['Automatic tuning of LMNN parameters for ' num2str(knn) '-NN classification.']); 

xTr = train'; yTr = train_label';
xTe = test'; yTe = test_label';
L0=pca(xTr)';

[L,~] = lmnn2(xTr, yTr,3,L0,'maxiter',1000,'quiet',1,'outdim',3,'mu',0.5,'validation',0.2,'earlystopping',25,'subsample',0.3);


%% fisher
%data = data*L';
cd '../fisher';
[w,b] = fisherbcl(train*L', train_label);

%% Test 
figure(2);
imshow(images{3});
for i = 768 : 256 : 3500
for j = 768 : 256 : 3500
    x = [];
    for k = 1 : wavecnt
        image = images{k}(j:j+256, i:i+256,:);
        x = [x, FeatureExtraction(image)];    
    end
    cl = x*L'*w + b;
    [i, j, cl]
    if ( cl < 0  )
        rectangle('Position',[i,j, 256, 256],'edgecolor','y');
    else
        rectangle('Position',[i,j, 256, 256],'edgecolor','b')
    end
end
end
    
    
    
    