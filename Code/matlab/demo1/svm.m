clc
clear
close all
A_img=[];
F_img=[];
test=[];
test_label=[];
train=[];
train_label=[];

cd '../../data/TrainSet';
Mt = csvread('data.txt');
[N, D] = size(Mt);
D = D-2;
add = 0;

%D = 256;
%add = 256*5;

train_i = 0;
test_i = 0;
for i=1:N
    if ( Mt(i,1)~=2 )
        Mt(i,1) = -1;
    end
    if rem( i, 200 ) < 100
    %if ( 1<=i&&i<=20 || 63<=i&&i<=77 || 102<=i&&i<=108 || 138<=i&&i<=168 )
        train_i = train_i+1;
        train( train_i , :) = Mt(i, 2+add: D+1+add );
        train_label( train_i , 1) = Mt(i, 1);
    else            
        test_i = test_i+1;
        test(test_i, :) = Mt(i, 2+add: D+1+add );
        test_label(test_i,1) = Mt(i, 1);
    end
end%将原始数据分为train集和test集

%figure(1);
svmStruct=svmtrain(train,train_label,'showplot',false,'boxconstraint',0.5, 'method', 'SMO');
class_train=svmclassify(svmStruct,train,'showplot',false) ;
train_rate=0;
for i=1:size(class_train)
   if class_train(i,1)==train_label(i,1)
       train_rate=train_rate+1;
   end
end
confusionmat(train_label,class_train)
[tmp,~] = size(class_train);
disp( ['正确率是', num2str(train_rate/tmp)] );

%figure(2);
test_rate=0;
%svmStruct=svmtrain(train,train_label,'showplot',false,'boxconstraint',1);
class_test=svmclassify(svmStruct,test,'showplot',false) ;
for i=1:size(class_test)
   if class_test(i,1)==test_label(i,1)
       test_rate=test_rate+1;
   end
end
confusionmat(test_label,class_test)
[tmp,~] = size(class_test);
disp( ['正确率是', num2str(test_rate/tmp)] );


