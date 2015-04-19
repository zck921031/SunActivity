%
function [g] = myfun1(L)
    load X_SS_Color;
    L = reshape(L, 3,144 );
    g = trace( L'*L * X);
end
