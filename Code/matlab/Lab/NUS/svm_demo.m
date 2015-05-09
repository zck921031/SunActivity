%
opt.MaxIter = 500000;
[ xTrain, yTrain, xTest, yTest ] = load_feature_nus500();

svmStruct = svmtrain(xTrain, yTrain, 'options', opt);

C = svmclassify(svmStruct, xTrain);
1 - sum( C==yTrain ) / length(yTrain)

C = svmclassify(svmStruct, xTest);
1 - sum( C==yTest )  / length(yTest)
