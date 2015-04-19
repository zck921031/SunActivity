%
function [grad] = Grad_hinge(xTr, L, Sset, Dset)
    [D, N] = size(xTr);
    grad = zeros( size(L) );
    x = L*xTr;
    for i = 1 : N
        for j = 1 : N
            t = x(:,i)-x(:,j);
            v = xTr(:,i)-xTr(:,j);
            sgn = 0;
            if 1 == Sset(i,j)
                sgn = 1;
            end
            if ( 1 == Dset(i,j) ) && ( sum( t.^2 ) < 1 )
                sgn = -1;
            end
            grad = grad + 2*sgn * L * v * v';
        end
    end
end
