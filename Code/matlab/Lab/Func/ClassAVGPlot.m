%输入训练集，测试集，变换矩阵L，线条颜色
%返回Precision-Recall值
function ClassAVGPlot(xTrain, yTrain, xTest, yTest, L, color)
    [ P, R, F1 ] = ClassAVGPR( xTrain, yTrain, xTest, yTest, L ); 
    N = size(P, 1);
    x = (1 : N) / N;
    subplot(2, 2, 1);
    plot(x, P, color); hold on;
    xlabel('ratio of retrieved to data'); ylabel('Precision');
    subplot(2, 2, 2);
    plot(x, R, color); hold on;
    xlabel('ratio of retrieved to data'); ylabel('Recall');
    subplot(2, 2, 3);
    xlabel('ratio of retrieved to data'); plot(x, F1,color); hold on;
    ylabel('F1');
    
    
end