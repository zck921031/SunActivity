%
function [M] = project_paper(M, pars)
    k = norm(M)/pars.R;
    if k > 1
        M = M/k;
    end
    
    [v, d] = eig(M);
    M = v*max(d, 0)*v';
end