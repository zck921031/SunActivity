%
clc;
clear all;
close all;
addpath('../FeatureExtraction');
%% Load: data, label
load train.mat;

wavecnt = 2;
%% simpleTrainAlgorithm
label = (label ~= 1);
data = data(:, 1:32*wavecnt);
seq = randperm(195);
K = 195;
[w, b] = fisherbcl2( data( seq(1:K), : ), label( seq(1:K) ) );
%yp = ( data( seq(K+1:195), : ) *w+b) < 0;
%disp( [ 'fisherÑµÁ·ÕýÈ·ÂÊ' , num2str(sum(yp == label(seq(K+1:195)) )/(195-K) ) ] );


%% Read a image
imagenames = {'20110809_032451_4096_0094.jpg', '20110809_032447_4096_0131.jpg', '20110809_032425_4096_0171.jpg', ...
    '20110809_032432_4096_0193.jpg', '20110809_032450_4096_0211.jpg', '20110809_032457_4096_0304.jpg', '20110809_032453_4096_0335.jpg', ...
    '20110809_032506_4096_1600.jpg', '20110809_032520_4096_1700.jpg'};

%C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\img\None\93

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

xTr = data'; yTr = label';
%xTe = test'; yTe = test_label';
L0=pca(xTr)';

[L,~] = lmnn2(xTr, yTr,3,L0,'maxiter',1000,'quiet',1,'outdim',3,'mu',1.0,'validation',0.2,'earlystopping',25,'subsample',1.0);


subplot(1,2,1);
scat(L0*xTr,3,yTr);
title('PCA Training');
noticks;box on;
subplot(1,2,2);
scat(L0*xTr,3,yTr);

%% fisher
data = data*L';
cd '../fisher';
[w,b] = fisherbcl(data, label);

%% Test 
figure(2);
imshow(images{wavecnt});
for i = 768 : 512 : 2816
for j = 768 : 512 : 2816-512*2
    x = [];
    for k = 1 : wavecnt
        image = images{k}(i:i+512, j:j+512,:);
        x = [x, FeatureExtraction(image)];    
    end
    cl = x*L'*w + b;
    [i, j, cl]
    if ( cl < 0  )
        rectangle('Position',[i,j, 512, 512]);
    end
end
end
    
    
    
    