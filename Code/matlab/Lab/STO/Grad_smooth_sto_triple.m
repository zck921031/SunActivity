%
function [grad] = Grad_smooth_sto_triple(xi, xj, xk, M, pars)    
    z = (xi-xk)'*M*(xi-xk) - (xi-xj)'*M*(xi-xj);
    % l = l_smooth(z, pars.L );
    dl = dl_smooth(z, pars.L );
    grad = dl*( (xi-xk)*(xi-xk)' - (xi-xj)*(xi-xj)' );
end
