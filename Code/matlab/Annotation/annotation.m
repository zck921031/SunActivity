
clc;
clear all;
close all;

%% Load Names: N*9 cells, Data, Label
load filenames;
load '../FeatureExtraction/train';
addpath('../FeatureExtraction');
addpath('../fisher');

%% train using fisher
label = (label ~= 1);
[w, b] = fisherbcl2( data, label );

%% annotation
images = {};
for k = 1 : 1
    imagename = ['C:\Users\zck\Documents\GitHub\SunActivity\data\regional annotation\img\', Names{1,k}];
    image_tmp = imread( imagename );
    images{k} = image_tmp;
end