%输入训练集，测试集，变换矩阵L，线条颜色
%返回Precision-Recall值
function [P, R, F1, mAP, ac, ac2] = fisher_plot(xTest, yTest, w, b, varargin)
    color = 'b';
    tt = '';
    if ( nargin == 5 )
        color = varargin{1};
    else
        for i = 1 : 2 : nargin-4
            if strcmp( varargin{i}, 'color' )
                color = varargin{i+1};
            end
            if strcmp( varargin{i}, 'title' )
                tt = varargin{i+1};
            end
        end
    end
    [ P, R, F1, mAP, ac, ac2 ] = fisher_PR( xTest, yTest, w, b ); 
    N = size(P, 1);
    x = (1 : N) / N;
    subplot(2, 2, 1);
    plot(x, P, color); hold on;
    xlabel('ratio of retrieved to data'); ylabel('Precision');
    title( [tt,' Precision-Retrieval ratio'] );
    
    subplot(2, 2, 2);
    plot(x, R, color); hold on;
    xlabel('ratio of retrieved to data'); ylabel('Recall');
    title( [tt, ' Recall-Retrieval ratio'] );
    
    subplot(2, 2, 3);
    plot(x, F1,color); hold on;
    xlabel('ratio of retrieved to data'); ylabel('F1');
    title([tt, ' F1-Retrieval ratio'] );
    
    
end