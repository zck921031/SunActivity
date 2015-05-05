%%the function to invoid hingle loss
function [l] = l_smooth(z, L)
    l = 1/L * log( 1 + exp( -L*(z-1) ) );
end
