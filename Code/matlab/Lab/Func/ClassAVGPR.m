%输入训练集，测试集，变换矩阵L，线条颜色
%返回Precision-Recall值
function [ P, R, F1, mAP, accuracy ] = ClassAVGPR(xTrain, yTrain, xTest, yTest, L)
    xTr = L*xTrain';
    xTe = L*xTest';
    dist =  zeros( size(xTest, 1), 1);
    distN = zeros( size(xTest, 1), 1);
    for i = 1 : size(xTest, 1)
        for j = 1 : size(xTrain, 1)
            if ( 1 == yTrain(j) )
                dist(i) = dist(i) + sum( ( (xTe(:,i) - xTr(:,j) ) ).^2 );
            else
                distN(i) = distN(i) + sum( ( (xTe(:,i) - xTr(:,j) ) ).^2 );
            end
        end
    end
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
            FP = FP+1;
            TN = TN-1;
        end
        accuracy = max(accuracy, (TP+TN)/(TP+FP+FN+TN) );
%         for i = 1 : size(xTest, 1)
%             dis = dist(i);
%             if dis<=d(k)    %检索为正类
%                 if ( 1 == yTest(i) )
%                     TP = TP+1;
%                 else
%                     FP = FP+1;
%                 end
%             else    %检索为负类                
%                 if ( 1 == yTest(i) )
%                     FN = FN+1;
%                 else
%                     TN = TN+1;
%                 end                
%             end
%         end
        R(k) = TP/(TP+FN);  %Recall
        P(k) = TP/(TP+FP);  %Precision
        F1(k)= 2*P(k)*R(k) / ( P(k)+R(k) ); %F1 value
    end
    
%     Right = 0;
%     Psize = sum( yTrain == 1 );
%     Nsize = sum( yTrain ~= 1 );
%     for i = 1 : size(xTest, 1)
%         if ( dist(i)/Psize < distN(i)/Nsize )
%             c = 1;
%         else
%             c = 0;
%         end
%         if ( c==yTest(i) )
%             Right = Right+1;
%         end
%     end
%     disp( [ '类平均法正确率: ', num2str( Right/ size(xTest, 1) ) ] );

end