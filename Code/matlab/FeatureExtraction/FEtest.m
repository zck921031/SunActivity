% Test FeatureExtraction.m
clc;
clear all;
close all;
concept = { 'Flare'; 'Coronal Hole'; 'None'};
test = [];

% for c = 1:3
%     figure(c+3);
%     for i = 1 : 9        
%         subplot(3, 3, i);
%         title([concept{c}, num2str(i)]);
%         imshow(imread( ['../../AnnotationSystem/AnnotationSystem/img/', concept{c} , '/', num2str(i), '/0193.jpg'] ) );
%     end
% end

for c = 1:3
    for i = 1 : 9
        figure(c+3);
        subplot(3, 3, i);
        Image = imread( ['../../AnnotationSystem/AnnotationSystem/img/', concept{c} , '/', num2str(i), '/0211.jpg'] );
        
        Hist = FeatureExtraction(Image);
        
        figure(c);
        subplot(3, 3, i);
        plot( Hist );
        title([concept{c}, num2str(i)]);
        
        test = [test; Hist];
    end
end
