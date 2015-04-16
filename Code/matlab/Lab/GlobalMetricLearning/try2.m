%
clc;
clear;
close all;
addpath '../JointDistributionLMNN';
[xTrain, yTrain, xTest, yTest] = load_feature_colorHist('SS_NoSS');

xTr = xTrain'; yTr = yTrain';
xTe = xTest'; yTe = yTest';

[D, N] = size(xTr);
Sset = zeros(N,N);
Dset = zeros(N,N);
for i = 1 : N
    for j = 1 : N
        if ( yTr(i) == yTr(j) )
            Sset(i,j) = 1;
        end
        if ( yTr(i) ~= yTr(j) )
            Dset(i,j) = 1;
        end
    end
end

L = rand(3,D);

addpath 'C:\Users\zck\Desktop\metric learning\code\code_Metric_online(MMC)';
A = opt(xTrain, Sset, Dset, 100);
[v, r] = eig(A);
L = v*sqrt(r);


subplot(1,2,1);
x = xTr;
plot3( x(1, find(yTr==1) )', x(2, find(yTr==1) )', x(3, find(yTr==1) )' , 'o' );hold on;
plot3( x(1, find(yTr~=1) )', x(2, find(yTr~=1) )', x(3, find(yTr~=1) )' , '*' );hold on;

subplot(1,2,2);
x = L*xTr;
plot3( x(1, find(yTe==1) )', x(2, find(yTe==1) )', x(3, find(yTe==1) )' , 'o' );hold on;
plot3( x(1, find(yTe~=1) )', x(2, find(yTe~=1) )', x(3, find(yTe~=1) )' , '*' );hold on;
