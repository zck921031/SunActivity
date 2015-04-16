%
clc;
clear;
close all;
addpath '../JointDistributionLMNN';
[xTrain, yTrain, xTest, yTest] = load_feature_colorHist('CH_NoCH');

xTr = xTrain'; yTr = yTrain';
xTe = xTest'; yTe = yTest';

[D, N] = size(xTr);
Sset = zeros(N,N);
Dset = zeros(N,N);
for i = 1 : N
    for j = 1 : N
        if ( yTr(i) == yTr(j) && yTr(i)==1 )
            Sset(i,j) = 1;
        end
        if ( yTr(i) ~= yTr(j) )
            Dset(i,j) = 1;
        end
    end
end

L = rand(3,D);

err = 1e9;
iter = 0;

while( err>1e-5  && iter<100 )
    F = G(xTr, L, Sset, Dset);
    F_next = F;
    L_next = L;
    grad = Grad(xTr, L, Sset, Dset);
    sear = 1e-9;
    while sear < 1e5
        L_temp = L - sear*grad;
        L_temp = L_temp/norm(L_temp);
        F_temp = G(xTr, L_temp, Sset, Dset);
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
plot3( x(1, find(yTr==1) )', x(2, find(yTr==1) )', x(3, find(yTr==1) )' , 'o' );hold on;
plot3( x(1, find(yTr~=1) )', x(2, find(yTr~=1) )', x(3, find(yTr~=1) )' , '*' );hold on;

subplot(1,2,2);
x = L*xTe;
plot3( x(1, find(yTe==1) )', x(2, find(yTe==1) )', x(3, find(yTe==1) )' , 'o' );hold on;
plot3( x(1, find(yTe~=1) )', x(2, find(yTe~=1) )', x(3, find(yTe~=1) )' , '*' );hold on;
