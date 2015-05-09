%
function [svmStruct] = svmtrainovr(xTrain, yTrain)
    svmStruct.classes = length( unique(yTrain) );
    for i = 1 : svmStruct.classes-1
        yTemp = ( yTrain == i );
        svmStructi = svmtrain( xTrain, yTemp, 'showplot',false );
        svmStruct.Struct{i} = svmStructi;
    end
    % svmStruct1=svmtrain( xTrain, yTrain, 'showplot',false );
    % C = svmclassify(svmStruct1, xTrain, 'showplot',false) ;
    % err_svm(cc, 1) = 1 - sum( C==yTrain ) / size(xTrain,1) ;
    % C = svmclassify(svmStruct1, xTest, 'showplot',false) ;
    % err_svm(cc, 2) = 1 - sum( C==yTest ) / size(xTest,1);
    % C = svmclassify(svmStruct1, xTest, 'showplot',false) ;
    % disp( ['svm train正确率是' num2str(  err_svm(cc, 1) ) ] );
    % disp( ['svm test 正确率是' num2str(  err_svm(cc, 2) ) ] );
end