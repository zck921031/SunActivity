%
function [L] = pair_sto(xTr, yTr, L, batch )


    [D, N] = size(xTr);

    iter = 0;
    while( iter<5 )
        %F = G_hinge2(xTr, L, Sset, Dset, 1, 1);
    %     F_next = F;
    %     L_next = L;
        sp = min(N, batch);
        pi = randperm(N);
        pj = randperm(N);
        tr = L*xTr;
        dist = zeros(N,1);
        for i = 1 : N
            for j = 1 : N
                if i ~= j
                    dist(i) = sum( ( tr(i) - tr(j) ).^2 );
                else
                    dist(i) = 1e25;
                end
            end
            grad = zeros( size(L) );
            [~, pj] = sort(dist);
            for jj = 1 : sp
                j = pj(jj);
                if yTr(i) == yTr(j)
                    grad = grad + Grad_hinge_sto(xTr(:,i), xTr(:,j), L, 1);
                else
                    grad = grad + Grad_hinge_sto(xTr(:,i), xTr(:,j), L, -1);
                    %grad = grad + Grad_smooth_sto(xTr(:,i), xTr(:,j), L, -1);
                end
            end
            L = L - grad*1e-5;
        end
        
        iter = iter+1;
        
        %[~,~,~,mAP,ac,ac2] = ClassAVGPR(xTrain, yTrain, xTrain, yTrain, L );
        %disp( ['ÑµÁ·¼¯ iters: ', num2str(iter),' F: ', num2str(F), ' norm: ', num2str( norm(L,2) ) , ' mAP: ' , num2str(mAP),' ac: ', num2str(ac) , ' ac2: ', num2str(ac2) ]  );
        %[~,~,~,mAP,ac,ac2] = ClassAVGPR(xTrain, yTrain, xTest, yTest, L );
        %disp( ['²âÊÔ¼¯ iters: ', num2str(iter),' F: ', num2str(F), ' norm: ', num2str( norm(L,2) ) , ' mAP: ' , num2str(mAP),' ac: ', num2str(ac) , ' ac2: ', num2str(ac2) ]  );

    end


%     subplot(1,2,1);
%     x = L*xTr;
%     plot3( x(1, find(yTr==1) )', x(2, find(yTr==1) )', x(3, find(yTr==1) )' , 'ro' );hold on;
%     plot3( x(1, find(yTr~=1) )', x(2, find(yTr~=1) )', x(3, find(yTr~=1) )' , 'b*' );hold on;
% 
%     subplot(1,2,2);
%     plot3( x(1, find(yTr==1) )', x(2, find(yTr==1) )', x(3, find(yTr==1) )' , 'go' );hold on;
%     plot3( x(1, find(yTr~=1) )', x(2, find(yTr~=1) )', x(3, find(yTr~=1) )' , 'g*' );hold on;
%     x = L*xTe;
%     plot3( x(1, find(yTe==1) )', x(2, find(yTe==1) )', x(3, find(yTe==1) )' , 'ro' );hold on;
%     plot3( x(1, find(yTe~=1) )', x(2, find(yTe~=1) )', x(3, find(yTe~=1) )' , 'b*' );hold on;
% 
% 
%     addpath('../func');
%     figure(2);
%     ClassAVGPlot(xTrain, yTrain, xTest, yTest, eye( size(xTest, 2), size(xTest, 2) ), 'b');
%     ClassAVGPlot(xTrain, yTrain, xTest, yTest, L, 'r');
% 
%         sum1 = 0;
%         for i = 1 : size(xTr, 2)
%             for j = 1 : size(xTr, 2)
%                 if ( Sset(i,j) )
%                     sum1 = sum1 + norm( L*( xTr(:,i)-xTr(:,j) ) ).^2;
%                 end
%             end
%         end
%         disp(sum1);