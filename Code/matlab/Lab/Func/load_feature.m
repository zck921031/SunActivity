%load feature
function [xTrain, yTrain, xTest, yTest, idxTr, idxTe] = load_feature(concept, modal, feature)
    disp( concept);
    disp( modal );
    disp( feature );
    
    cntTr = 0; cntTe = 0;
    idxTr = []; idxTe = [];
    
    waveNames = {'0094','0131','0171','0193','0211','0304','0335','1600','1700'};
    featureNames = {'colorHist', 'siftHist', 'lbpHist'};
    addpath(['C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\feature\', concept]);
    x = [];
    y = csvread('concept.txt');
    for i = 1 : max( size(modal) )
        waveName = waveNames{ modal(i) };
        for j = 1 : max( size( feature ) )
            featureName = featureNames{ feature(j)  };
            Name = [ waveName '.' featureName '.txt'  ];
            x = [x csvread(Name)];
        end
    end
    %rate = |train|/|test|.  Total number = (1 + 1/rate)*|train|
    rate = 1;
    
    h = tabulate(y);
    mp = zeros( size(x,1), 1 );
    for i = 1 : size(x,1)
        if h( find( h == y(i), 1) , 2 ) > 0
            h( find( h == y(i), 1) , 2 ) = h( find( h == y(i), 1) , 2 ) - (1.0 + 1.0/rate );
            mp(i) = 0;
            cntTr = cntTr + 1;
            idxTr(cntTr) = i;
        else
            mp(i) = 1;
            cntTe = cntTe + 1;
            idxTe(cntTe) = i;
        end
    end
    xTrain = x( find( mp == 0 ), :);
    yTrain = y( find( mp == 0 ) );
    xTest  = x( find( mp == 1 ), :);
    yTest  = y( find( mp == 1 ) );
end