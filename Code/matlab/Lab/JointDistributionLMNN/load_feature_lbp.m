%load feature
function [xTrain, yTrain, xTest, yTest] = load_feature(concept)
    waveNames = {'0094','0131','0171','0193','0211','0304','0335','1600','1700'};
    featureNames = {'colorHist', 'siftHist', 'lbpHist'};
    addpath(['H:\±ÏÒµÉè¼Æ\', concept, '\feature']);
    x = [];
    y = csvread('concept.txt');
    for wave = 1 : 9
        waveName = waveNames{wave};
        for featureCnt = 3 : 3
            featureName = featureNames{featureCnt};
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
        else
            mp(i) = 1;
        end
    end
    xTrain = x( find( mp == 0 ), :);
    yTrain = y( find( mp == 0 ) );
    xTest  = x( find( mp == 1 ), :);
    yTest  = y( find( mp == 1 ) );
end