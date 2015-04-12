clc
clear all
close all

addpath 'C:\Users\zck\Desktop\大三下学期\Computer Vision\vlfeat-0.9.18-bin\vlfeat-0.9.18\toolbox';
vl_setup;

I1 = imread('C:\Users\zck\Documents\GitHub\SunActivity\Image Annotation\SampleData2\l0171.jpg') ;
I1=I1(1:920,:,:);
figure,image(I1) ;
Ia = single(rgb2gray(I1));
[fa,da] = vl_sift(Ia);
perm = randperm(size(fa,2));
sel = perm(1:50) ;
%sel = perm(1:size(fa,2)) ;
h1 = vl_plotframe(fa(:,sel));
h2 = vl_plotframe(fa(:,sel));
set(h1,'color','k','linewidth',3) ;
set(h2,'color','y','linewidth',2) ;

I2= imread('C:\Users\zck\Documents\GitHub\SunActivity\Image Annotation\SampleData2\l0171_1.jpg');
I2=I2(1:920,:,:);
figure,image(I2) ;
Ib = single(rgb2gray(I2)) ;
[fb,db] = vl_sift(Ib) ;
perm = randperm(size(fb,2)) ;
sel = perm(1:50) ;
h3 = vl_plotframe(fb(:,sel)) ;
h4 = vl_plotframe(fb(:,sel)) ;
set(h3,'color','k','linewidth',3) ;
set(h4,'color','y','linewidth',2) ;


[matches, scores] = vl_ubcmatch(da, db) ;
[scores, idx] = sort(scores,  'descend');
matches = matches(:,idx(1:10) );
%-------------匹配后将match的点进行连线------------%
II=[I1 I2];
figure,image(II)
h5=vl_plotframe(fa(:,matches(1,:)));
[m1,n1]=size(fb);
for i=1:n1
    fb(1,i)=fb(1,i)+1024;
end    
h6=vl_plotframe(fb(:,matches(2,:)));
[m,n]=size(matches);
for i=1:n
    line([fa(1,matches(1,i)) fb(1,matches(2,i))] ,[ fa(2,matches(1,i)) fb(2,matches(2,i))]);
end