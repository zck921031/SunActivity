%
function [grad] = Grad_hinge_sto(xTr, L, i, j, u)
    grad = zeros( size(L) );
    x = L*xTr;
    t = x(:,i)-x(:,j);
    v = xTr(:,i)-xTr(:,j);
    sgn = 0;
    if 1 == u
        sgn = 1;
    end
    if ( 1 ~= u ) && ( sum( t.^2 ) < 1 )
        sgn = -1;
    end
    grad = grad + 2*sgn * L * v * v';
end
