%
% GenFeature.m
clc;
clear all;
close all;
addpath (pwd);
concept = { 'Flare'; 'Coronal Hole'; 'None'};
count = [57, 42, 96];
waves = {'0094'; '0131'; '0171'; '0193'; '0211'; '0304'; '0335'; '1600'; '1700'};
test = [];

% for c = 1:3
%     figure(c+3);
%      for i = 1 : 9
%          subplot(3, 3, i);
%          %title([concept{c}, num2str(i)]);
%          imagename = ['../../AnnotationSystem/AnnotationSystem/img/', concept{1} , '/', num2str(1), '/', waves{i}, '.jpg'];
%          imshow(imread( imagename) );
%      end
% end

data = [];
label = [];
cnt = 0;
for c = 1:3
    for i = 1 : count(c)
        cnt = cnt + 1;
        x = [];
        disp( ['Processing ', num2str(cnt), ' images' ]);
        for j = 1 : 9
            imagename = ['../../AnnotationSystem/AnnotationSystem/img/', concept{c} , '/', num2str(i), '/', waves{j}, '.jpg'];
            Image = imread( imagename );
            x = [x, FeatureExtraction(Image)];
        end
        data = [data; x];
        label = [label; c];
    end
end

save train.mat data label;
