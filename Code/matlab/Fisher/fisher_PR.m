%输入训练集，测试集，变换矩阵L，线条颜色
%返回Precision-Recall值
%fisher LDA 算法
function [ P, R, F1, mAP, accuracy, ac2 ] = fisher_PR(xTest, yTest, w, b)
    
    dist = xTest * w + b;
    ac2 = sum( (dist<0) == yTest  ) / size( xTest, 1 );
    [d, idx] = sort(dist);
    P = zeros( size(xTest, 1), 1 );
    R = zeros( size(xTest, 1), 1 );
    F1= zeros( size(xTest, 1), 1 );
    TP=0;   FP=0;
    FN=sum( yTest==1 );    TN=sum( yTest~=1 );
    mAP = 0;
    accuracy = 0;
    
    for k = 1 : size(d, 1)
        if ( yTest( idx(k) ) == 1 )
            TP = TP+1;
            FN = FN-1;
            mAP = mAP + TP/( (TP+FP)*(TP+FN) );
        else            
                %disp( idx(k) );                
            FP = FP+1;
            TN = TN-1;
        end
        accuracy = max(accuracy, (TP+TN)/(TP+FP+FN+TN) );
        
        R(k) = TP/(TP+FN);  %Recall
        P(k) = TP/(TP+FP);  %Precision
        F1(k)= 2*P(k)*R(k) / ( P(k)+R(k) ); %F1 value
        
        
    end
    
end