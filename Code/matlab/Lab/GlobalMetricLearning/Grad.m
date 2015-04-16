%
function [grad] = Grad(xTr, L, Sset, Dset)
    [D, N] = size(xTr);
    grad = zeros( size(L) );
    for i = 1 : N
        for j = 1 : N
            sgn = 0;
            if 1 == Sset(i,j)
                sgn = 1;
            end
            if 1 == Dset(i,j)
                sgn = -1;
            end
            x = xTr(:,i)-xTr(:,j);
            grad = grad + 2*sgn * L * x * x';
        end
    end
end
