%����ѵ���������Լ����任����L��������ɫ
%����Precision-Recallֵ
function [ P, R, F1 ] = ClassAVGPR(xTrain, yTrain, xTest, yTest, L)
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
    d = sort(dist);
    P = zeros( size(xTest, 1), 1 );
    R = zeros( size(xTest, 1), 1 );
    F1= zeros( size(xTest, 1), 1 );
    for k = 1 : size(d, 1)
        TP=0; FP=0; FN=0; TN=0;
        for i = 1 : size(xTest, 1)
            dis = dist(i);
            if dis<=d(k)    %����Ϊ����
                if ( 1 == yTest(i) )
                    TP = TP+1;
                else
                    FP = FP+1;
                end
            else    %����Ϊ����                
                if ( 1 == yTest(i) )
                    FN = FN+1;
                else
                    TN = TN+1;
                end                
            end            
        end
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
%     disp( [ '��ƽ������ȷ��: ', num2str( Right/ size(xTest, 1) ) ] );

end