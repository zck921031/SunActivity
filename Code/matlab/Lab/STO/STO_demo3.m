%% 批量随机梯度下降
clc;
clear;
close all;

islearning = 1;

addpath('../func');
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';

%[xTrain, yTrain, xTest, yTest] = load_feature('Sunspot', 1:9, 1:3);
[xTrain, yTrain, xTest, yTest] = load_feature('Coronal Hole', 1:9, 1:3);
%[xTrain, yTrain, xTest, yTest] = load_feature('flare', 1:9, 1:3);

xTrain = xTrain;
xTest = xTest;

xTr = xTrain'; yTr = yTrain';
xTe = xTest'; yTe = yTest';

d = 3;
[D, N] = size(xTr);
Sset = zeros(N,N);
Dset = zeros(N,N);
for i = 1 : N
    for j = 1 : N
        if yTr(i) == yTr(j) % && yTr(i)==1 
            Sset(i,j) = 1;
            if yTr(i)==1
                Sset(i,j) = 1;
            end
        end
        if ( yTr(i) ~= yTr(j) )
            Dset(i,j) = 1;
        end
    end
end

L = rand(d,D);
%L = eye(3,D)
L = L/norm(L);

iter = 0;

errRAW=knncl([], xTr, yTr,xTe,yTe, 3);
disp( ['Euclidean knn error rate: ' num2str(errRAW(2) ) ] );

while( islearning ~= 0 && iter<1000 )
%     F_next = F;
%     L_next = L;
    sp = min(N, 20);
    pi = randperm(N);
    pj = randperm(N);
    pk = randperm(N);
    grad = zeros( size(L) );
    for ii = 1 : sp
        for jj = 1 : sp
            i = pi(ii);
            j = pj(jj);
            if ( yTr(i) ~= yTr(j) )
                continue;
            end
            
            grad = grad + Grad_smooth_sto(xTr(:,i), xTr(:,j), L, 1);
                
            for kk = 1 : sp
                k = pk(kk);
                grad = grad + Grad_hinge_sto_triple( xTr(:,i), xTr(:,j), xTr(:,k), L);     
            end
            
        end
    end
	L = L - grad*1e-5;
    
    iter = iter+1;
    
    % [~,~,~,mAP,ac,ac2] = ClassAVGPR(xTrain, yTrain, xTest, yTest, L );
    % F = G_hinge2(xTr, L, Sset, Dset, 1, 1);
    
    if rem(iter, 10)== 0
        
        F = G_hinge2(xTr, L, Sset, Dset, 1, 1);
        [~,~,~,mAP,ac,ac2] = ClassAVGPR(xTrain, yTrain, xTest, yTest, L );
        disp( ['测试集 iters: ', num2str(iter),' F: ', num2str(F), ' norm: ', num2str( norm(L,2) ) , ' mAP: ' , num2str(mAP),' ac: ', num2str(ac) , ' ac2: ', num2str(ac2) ]  );
        errRAW=knncl(L, xTr, yTr,xTe,yTe, 3);
        disp( ['L.norm()=' num2str(norm(L,2))] );
        disp( ['knn error rate: ' num2str(errRAW(2) ) ] ); 
    end
end
errRAW=knncl([], xTr, yTr,xTe,yTe, 3);
disp( ['Euclidean knn error rate: ' num2str(errRAW(2) ) ] );

if 0 ~= islearning
    save L_try_sto L;
else
    load L_try_sto;
end

subplot(1,2,1);
x = L*xTr;
plot3( x(1, find(yTr==1) )', x(2, find(yTr==1) )', x(3, find(yTr==1) )' , 'ro' );hold on;
plot3( x(1, find(yTr~=1) )', x(2, find(yTr~=1) )', x(3, find(yTr~=1) )' , 'b*' );hold on;

subplot(1,2,2);
plot3( x(1, find(yTr==1) )', x(2, find(yTr==1) )', x(3, find(yTr==1) )' , 'go' );hold on;
plot3( x(1, find(yTr~=1) )', x(2, find(yTr~=1) )', x(3, find(yTr~=1) )' , 'g*' );hold on;
x = L*xTe;
plot3( x(1, find(yTe==1) )', x(2, find(yTe==1) )', x(3, find(yTe==1) )' , 'ro' );hold on;
plot3( x(1, find(yTe~=1) )', x(2, find(yTe~=1) )', x(3, find(yTe~=1) )' , 'b*' );hold on;


addpath('../func');
figure(2);
ClassAVGPlot(xTrain, yTrain, xTest, yTest, eye( size(xTest, 2), size(xTest, 2) ), 'b');
ClassAVGPlot(xTrain, yTrain, xTest, yTest, L, 'r');

    sum1 = 0;
    for i = 1 : size(xTr, 2)
        for j = 1 : size(xTr, 2)
            if ( Sset(i,j) )
                sum1 = sum1 + norm( L*( xTr(:,i)-xTr(:,j) ) ).^2;
            end
        end
    end
    disp(sum1);