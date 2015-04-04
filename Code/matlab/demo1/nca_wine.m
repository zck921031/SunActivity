clear all;
clc;
trainset = csvread('..\..\data\wine\wine_train.data');
train_data = trainset(:,2:14);
train_label = trainset(:,1);
[n, dim_temp] = size(trainset);

%NCA
x = train_data;
label = uint8( train_label );
[n, d] = size(x);
A = 0.01*eye(d);

p = zeros(n,n);
P = zeros(n,1);
s = zeros(n,n);
for i = 1 : n
    fm = 0;
    for j = 1 : n
        if i ~= j
            s(i,j) = exp( - sum( ( A*( x(i,:)-x(j,:) )' ).^2 ) );
            fm = fm + s(i,j);
        end
    end
    for j = 1 : n
        if i ~= j
            p(i,j) = s(i,j) / fm;            
        else
            p(i,j) = 0;
        end
        if label(i) == label(j)
            P(i) = P(i) + p(i,j);
        end
    end
end
F = sum( P(i) );
disp(F);

G = zeros(d,d);
for i = 1 : n
    for k = 1 : n
        u = ( x(i,:)-x(k,:) )';
        G = G + P(i) * p(i,k)*u*u';
        if label(i) == label(k)
            G = G - p(i,k)*u*u';
        end
    end
end
