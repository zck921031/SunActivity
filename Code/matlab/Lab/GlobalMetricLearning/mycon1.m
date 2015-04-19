%
function [C, Ceq] = mycon1(L)
    Ceq = sum( L.^2 ) - 1;
end
