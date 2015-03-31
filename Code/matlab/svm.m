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

train_i = 0;
test_i = 0;
for i=1:N
	if Mt(i,1)~=3
        if ( Mt(i,1)~=1 )
            Mt(i,1) = -1;
        end
		if rem( i, 200 ) > 150
            train_i = train_i+1;
			train( train_i , :) = Mt(i, 2:D+1);
			train_label( train_i , 1) = Mt(i, 1);
        else            
            test_i = test_i+1;
			test(test_i, :) = Mt(i, 2:D+1);
			test_label(test_i,1) = Mt(i, 1);
		end
	end
end%将原始数据分为train集和test集

%figure(1);
svmStruct=svmtrain(train,train_label,'showplot',false,'boxconstraint',0.5, 'method', 'SMO');
class_train=svmclassify(svmStruct,train,'showplot',false) ;
train_rate=0;
for i=1:100
   if class_train(i,1)==train_label(i,1)
       train_rate=train_rate+1;
   end
end
confusionmat(train_label,class_train)
disp('正确率是');
train_rate/100

%figure(2);
test_rate=0;
%svmStruct=svmtrain(train,train_label,'showplot',false,'boxconstraint',1);
class_test=svmclassify(svmStruct,test,'showplot',false) ;
for i=1:100
   if class_test(i,1)==test_label(i,1)
       test_rate=test_rate+1;
   end
end
confusionmat(test_label,class_test)
disp('正确率是');
test_rate/100


