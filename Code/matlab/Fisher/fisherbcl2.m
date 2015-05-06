%fisherbcl2.m
%N<D时，协方差矩阵奇异，因此求投影直线改用eig实现。
%fisher linear binary classifier
%written by zck
%data \in R^{ND}, N points in D dimensions, label \in {0,1}.
%class0 iff w'x+b > 0, class1 iff w'x+b < 0.
function [w, b] = fisherbcl(data, label)
    [N, D] = size(data);
    for i = 1 : N
        if label(i)~=0 && label(i)~=1
            error('input label must in {0, 1}');
        end
    end
    w1 = data( find( label~=1 ), : );
    w2 = data( find( label==1 ), : );
    m1=mean(w1);
    m2=mean(w2);
    wm1 = w1 - ones( size(w1, 1) , 1)*m1;
    wm2 = w2 - ones( size(w2, 1) , 1)*m2;    
    s1 = wm1' * wm1;
    s2 = wm2' * wm2;
    Sw=s1+s2;
    
    %计算fisher准则函数取极大值时的解w
%    w=inv(Sw)*(m1-m2)';
    
     [P, V] = eig(Sw);
     for i = 1 : D
         if ( abs(V(i,i))>1e-9 )
             V(i,i) = V(i,i)^-1;
         end
     end
     w = P*V*(P^-1)*(m1-m2)';
    
    %计算阈值w0
    ave_m1 = w'*m1';
    ave_m2 = w'*m2';
    b = -(ave_m1+ave_m2)/2;
    
    yp = (data*w+b) < 0;
	
    % disp( [ 'fisher训练正确率' , num2str(sum(yp == label)/N) ] );
    
end