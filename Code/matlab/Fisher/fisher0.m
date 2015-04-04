function [w, b] = fisher0(data, label)
    [N, D] = size(data);
    for i = 1 : N
        if label(i)~=0 && label(i)~=1
            error('input label must in {0, 1}');
        end
    end
    w1 = data( find( label==0 ), : );
    w2 = data( find( label==1 ), : );
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %以下部分为fisher算法的实现
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %计算样本均值
    m1=mean(w1)';
    m2=mean(w2)';
    %s1、s2分别代表表示第一类、第二类样本的类内离散度矩阵
    s1=zeros(2);
    [row1,colum1]=size(w1);
    for i=1:row1
         s1 = s1 + (w1(i,:)'-m1)*(w1(i,:)'-m1)';
    end;
    s2=zeros(2);
    [row2,colum2]=size(w2);
    for i=1:row2
         s2 = s2 + (w2(i,:)' - m2)*(w2(i,:)' - m2)';
    end;
    %计算总类内离散度矩阵Sw
    Sw=s1+s2;
    %计算fisher准则函数取极大值时的解w
    w=inv(Sw)*(m1-m2);
    %计算阈值w0
    ave_m1 = w'*m1;
    ave_m2 = w'*m2;
    b = -(ave_m1+ave_m2)/2;
end