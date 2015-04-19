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
        if yTr(i) == yTr(j) % && yTr(i)==1 
            Sset(i,j) = 1;
        end
        if ( yTr(i) ~= yTr(j) )
            Dset(i,j) = 1;
        end
    end
end

L = rand(3,D);
%L = eye(3,D)
L = L/norm(L);

err = 1e9;
iter = 0;

while( err>1e-5  && iter<10 )
    F = G_hinge(xTr, L, Sset, Dset);
    F_next = F;
    L_next = L;
    grad = Grad_hinge(xTr, L, Sset, Dset);
    sear = 1e-9;
    while sear < 1e5
        L_temp = L - sear*grad;
        L_temp = L_temp/norm(L_temp);
        F_temp = G_hinge(xTr, L_temp, Sset, Dset);
        if ( F_temp < F_next  )
            F_next = F_temp;
            L_next = L_temp;
        end
        sear = sear * 2;
    end
    err = F - F_next;
    disp([ num2str(iter), '  F_pre: ' ,num2str(F), ' F_next: ', num2str(F_next), ' error: ', num2str(err) ]);
    L = L_next;
    F = F_next;
    iter = iter+1;
end

subplot(1,2,1);
x = L*xTr;
plot3( x(1, find(yTr==1) )', x(2, find(yTr==1) )', x(3, find(yTr==1) )' , 'ro' );hold on;
plot3( x(1, find(yTr~=1) )', x(2, find(yTr~=1) )', x(3, find(yTr~=1) )' , 'b*' );hold on;

subplot(1,2,2);
x = L*xTe;
plot3( x(1, find(yTe==1) )', x(2, find(yTe==1) )', x(3, find(yTe==1) )' , 'ro' );hold on;
plot3( x(1, find(yTe~=1) )', x(2, find(yTe~=1) )', x(3, find(yTe~=1) )' , 'b*' );hold on;


addpath('../func');
figure(2);
ClassAVGPlot(xTrain, yTrain, xTest, yTest, eye( size(xTest, 2), size(xTest, 2) ), 'b');
ClassAVGPlot(xTrain, yTrain, xTest, yTest, L, 'r');

    sum = 0;
    for i = 1 : size(xTr, 2)
        for j = 1 : size(xTr, 2)
            if ( Sset(i,j) )
                sum = sum + norm( L*( xTr(:,i)-xTr(:,j) ) ).^2;
            end
        end
    end
    disp(sum);