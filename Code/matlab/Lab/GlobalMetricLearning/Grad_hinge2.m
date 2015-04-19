%
function [grad] = Grad_hinge2(xTr, L, Sset, Dset, u1, u2)
    [D, N] = size(xTr);
    grad = zeros( size(xTr*xTr') );
    x = L*xTr;
    for i = 1 : N
        for j = 1 : N
            t = x(:,i)-x(:,j);
            v = xTr(:,i)-xTr(:,j);
            sgn = 0;
            if 0 ~= Sset(i,j)
                sgn = u1*Sset(i,j);
            end
            if ( 0 ~= Dset(i,j) ) && ( sum( t.^2 ) < 1 )
                sgn = -u2*Dset(i,j);
            end
            grad = grad + 2*sgn * v * v';
        end
    end
    grad = L*grad;
end
