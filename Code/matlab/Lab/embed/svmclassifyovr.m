%
function [C] = svmclassifyovr(svmStruct, xTest)
    C = zeros( length(xTest), 1 );
    for i = 1 : svmStruct.classes-1
        res = svmclassify( svmStruct.Struct{i} , xTest, 'showplot',false) ;
        C( find( res == 1 ) ) = i;
    end
    C( find( C == 0 ) ) = svmStruct.classes;
    
end