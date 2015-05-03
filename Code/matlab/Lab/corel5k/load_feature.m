%
function [xTrain, yTrain, xTest, yTest] = load_feature( topk )
    %addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\Func';
    
    sets     = {  'train', 'test' };
    prefix  = 'C:\Users\zck\Desktop\metric learning\dataset\corel5k.20091111\corel5k';
    type = { 'RgbReduce.hvecs32', 'HarrisSift.hvecs', 'HarrisHue.hvecs' };
    %type = { 'RgbReduce.hvecs32', 'DenseHue.hvecs', 'HarrisHue.hvecs' };
    dict   = textread([prefix '_dictionary.txt'],'%s');

    xTrain = []; yTrain = [];
    xTest =  []; yTest  = [];
    sum( vec_read([prefix '_train_annot.hvecs']) , 1);
    [~, idx] = sort( sum( vec_read([prefix '_train_annot.hvecs']) , 1), 'descend' );
    chi = idx( topk );
    disp(['choice dict ', dict(chi) ] );
    
    for s=1:length(sets)
        str   = sets{s};
        list  = textread([prefix '_' str '_list.txt'], '%s');
        annot = uint8( vec_read([prefix '_' str '_annot.hvecs']) );
        x = [];
        for ti=1:length(type)
            tmp = double( vec_read( [prefix '_' str '_' type{ti} ] ) );
            %x = [x, tmp ];
            x = [x, do_norm( tmp ) ];
        end
        if ( 1 == s )
            xTrain = double(x);
            yTrain = annot(:, chi);
        else
            xTest  = double(x);
            yTest  = annot(:, chi);
        end
      
    end
    
end

function [ng] = do_norm(g)
%ng = g;
    ng = zeros( size(g) );
    for i = 1 : size(g, 1)
        ng(i, :) = g(i, :) / ( norm( g(i,:), 2 ) + 1e-9 );
    end
end
