clc;
close all;
clear all;

X1 = normrnd(50,10,[100,1]);
Y1 = normrnd(50,15,[100,1]);
L1 = zeros(100,1);
x1=[X1, Y1];
X2 = normrnd(5 ,20,[100,1]);
Y2 = normrnd(0 ,30,[100,1]);
x2=[X2, Y2];
L2 = ones(100,1);
data = [x1;x2];
label = [L1; L2];



[w, b] =fisherbcl(data, label);
%[w, b] =fisher0(data, label);
w
b
figure(1);
plot( x1(:,1), x1(:,2), '*' );
hold on;
plot( x2(:,1), x2(:,2), 'o' );
hold on;
%画出取极大值时的解w
x = [-40:0.1:80];
y = ( - b - x*w(1) )/w(2);
plot(x,y,'g');
hold on;
y = data*w+b;


