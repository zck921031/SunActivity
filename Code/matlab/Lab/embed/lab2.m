%

clc;
clear;
close all;

%% multi-modal solar image
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lab\func';

CCS = {'Sunspot', 'Coronal Hole', 'Flare'};

weight = zeros( length(CCS) , 9 );
err = zeros( 4, 30 );

for cc = 1 : length(CCS)    
    Concept = CCS{cc};
    [xTrain, yTrain, xTest, yTest] = load_feature( Concept, 1:9, 1:3);
    xTr = xTrain'; yTr = yTrain';
    xTe = xTest'; yTe = yTest';
    switch cc
        case 1
            load mmLMNN_SS_5k;
        case 2
            load mmLMNN_CH_5k;
        case 3
            load mmLMNN_flare_5k;
    end
    for modal = 1 : 9
        weight(cc, modal) = norm( L( 3*(modal-1)+1 : 3*modal, 125*(modal-1)+1 : 125*modal ) );
    end
    for tim = 1 : 30
        if ( tim > length( Det.log.L ) )
            err(cc, tim) = err(cc, tim-1);
        else
            tmp = knncl(Det.log.L{tim}, xTr, yTr, xTe, yTe, 1); 
            err (cc, tim) = tmp(2);
        end
    end
end

%% ox5
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\AnnotationSystem\AnnotationSystem\feature\ox5';
[xTrain, yTrain, xTest, yTest] =  load_feature_ox5( 1:3 );
xTr = xTrain'; yTr = yTrain';
xTe = xTest'; yTe = yTest';
load mmLMNN_ox5_3k;
for tim = 1 : 30
    if ( tim > length( Det.log.L ) )
        err(4, tim) = err(4, tim-1);
    else
        tmp = knncl(Det.log.L{tim}, xTr, yTr, xTe, yTe, 1); 
        err (4, tim) = tmp(2);
    end
end

%% show weight
figure(1);
bar( weight );
title('每个模态的重要性');
set(gca,'XTickLabel', {'Sunspot','Coronal Hole','Flare' } );
legend( {'0094','0131','0171','0193','0211','0304','0335','1600','1700'}, 2 );

% figure(1);
% bar( weight(1, :) );
% title('太阳黑子');
% set(gca,'XTickLabel', {'0094','0131','0171','0193','0211','0304','0335','1600','1700' } );
% ylabel( '||A||' );
% 
% figure(2);
% bar( weight(2, :) );
% title('太阳冕洞');
% set(gca,'XTickLabel', {'0094','0131','0171','0193','0211','0304','0335','1600','1700' } );
% ylabel( '||A||' );
% 
% 
% figure(3);
% bar( weight(3, :) );
% title('太阳耀斑');
% set(gca,'XTickLabel', {'0094','0131','0171','0193','0211','0304','0335','1600','1700' } );
% ylabel( '||A||' );

%% show iterator
acc = 1 - err;
figure(4);
%plot( err(1,:), 'r', 'LineWidth',2,'MarkerEdgeColor','r','MarkerFaceColor','g','MarkerSize',10 );  hold on;
%plot( err(2,:), 'b', 'LineWidth',2,'MarkerEdgeColor','r','MarkerFaceColor','g','MarkerSize',10 );  hold on;
%plot( err(3,:), 'g', 'LineWidth',2,'MarkerEdgeColor','r','MarkerFaceColor','g','MarkerSize',10 );  hold on;
%plot( err(4,:), 'y', 'LineWidth',2,'MarkerEdgeColor','r','MarkerFaceColor','g','MarkerSize',10 );  hold on;

plot( acc(1,:), 'r*-', 'LineWidth', 2, 'MarkerSize', 7 );  hold on;
plot( acc(2,:), 'bo-', 'LineWidth', 2, 'MarkerSize', 7  );  hold on;
plot( acc(3,:), 'gs-', 'LineWidth', 2, 'MarkerSize', 7  );  hold on;
plot( acc(4,:), 'y^-', 'LineWidth', 2, 'MarkerSize', 7  );  hold on;

% plot( acc(1,:), 'k*-', 'LineWidth', 1, 'MarkerSize', 4 );  hold on;
% plot( acc(2,:), 'ko-', 'LineWidth', 1, 'MarkerSize', 4  );  hold on;
% plot( acc(3,:), 'ks-', 'LineWidth', 1, 'MarkerSize', 4  );  hold on;
% plot( acc(4,:), 'k^-', 'LineWidth', 1, 'MarkerSize', 5  );  hold on;

ylim([0.52,1.03]);
legend( {'太阳黑子', '太阳冕洞', '太阳耀斑', 'Oxford'} );
% title('测试正确率随迭代次数的变化曲线');
xlabel('迭代次数 x 100次'), ylabel('测试正确率');

