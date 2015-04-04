%Flare None Coronal Hole
close all;
clear all;
clc;
Image = imread( ['../../AnnotationSystem/AnnotationSystem/img/', 'Coronal Hole' , '/', '2', '/0211.jpg'] );
figure(2);
imshow(Image);
[~,~,GB] = gaborfilter( rgb2gray(Image), 6, 90, 0.5, 0.5);

figure(3);
GB = uint8(GB);
imshow( GB );

mean( mean( GB ) )