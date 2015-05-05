%
function [grad] = Grad_smooth_sto(xi, xj, L, u)
    if 1 == u        
        v = xi - xj;
        grad = L*v*v';
    else
        v = xi - xj;
        z = sum( (L*(xi-xj)).^2 );
        dl = dl_smooth(z, 1 );
        grad = dl * L*v*v' ;
    end    
end
