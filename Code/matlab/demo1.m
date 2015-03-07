%This script is written for testing
clc;
clear all;
close all;
image = read_image_by_path('..\..\Image Annotation\SampleData');
cm_feature = get_color_moment_feature_by_image(image);
disp(cm_feature);
[sizeA, sizeB, sizeD] = size(image);

figure(1);
for i=1:sizeD
    subplot(2,5,i);
    imshow( image(:,:,i) );
end

texture = get_texture_feature_by_image(image);
disp(texture);