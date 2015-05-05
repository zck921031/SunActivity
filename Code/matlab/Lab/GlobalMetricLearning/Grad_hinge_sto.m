%
function [grad] = Grad_hinge_sto(xi, xj, L, u)
    grad = zeros( size(L) );
    t = L*(xi - xj);
    v = xi - xj;
    sgn = 0;
    if 1 == u
        sgn = 1;
    end
    if ( 1 ~= u ) && ( sum( t.^2 ) < 1 )
        sgn = -1;
    end
    grad = grad + 2*sgn * L * v * v';
end
