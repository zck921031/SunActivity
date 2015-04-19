%
function [grad] = Grad_hinge2(xTr, L, Sset, Dset, u1, u2)
    [D, N] = size(xTr);
    g = zeros( N,N );
    x = L*xTr;
    for i = 1 : N
        for j = 1 : N
            t = x(:,i)-x(:,j);
            sgn = 0;
            if 0 ~= Sset(i,j)
                sgn = u1*Sset(i,j);
            end
            if ( 0 ~= Dset(i,j) ) && ( sum( t.^2 ) < 1 )
                sgn = -u2*Dset(i,j);
            end
            g(i,i) = g(i,i)+sgn;   g(j,j) = g(j,j)+sgn;
            g(i,j) = g(i,j)-sgn;   g(j,i) = g(j,i)-sgn;
        end
    end
    grad = zeros( size(L) );
    for i = 1 : N
        tmp = repmat( g(i,:), D, 1).*xTr;
        tmp = sum(tmp, 2)';
        grad = grad + x(:,i)*tmp;
    end
    grad = 2*grad;
    
%     grad = zeros( size(L) );
%     for i = 1 : N
%         tmp = zeros(1, D);
%         for j = 1 : N
%             tmp = tmp +  g(i,j)*xTr(:, j)';
%         end
%         grad = grad + x(:,i)*tmp;
%     end
%     grad = 2*grad;
end
