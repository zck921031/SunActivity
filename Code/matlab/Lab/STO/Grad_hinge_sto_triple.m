%
function [grad] = Grad_hinge_sto_triple(xi, xj, xk, L)
    grad = zeros( size(L) );
    %x = L*xTr;
    s = xi - xj;
    p = xi - xk;
    if ( sum( s.^2 ) + 1 < sum( p.^2 ) ) 
        grad = zeros( size(L) );
        return ;
    end
    grad = L*s*s' - L*p*p';
end
