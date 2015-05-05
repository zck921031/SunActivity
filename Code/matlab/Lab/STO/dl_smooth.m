%%the function to invoid hingle loss
function [dl] = dl_smooth(z, L)
    dl = - ( exp( -L*(z-1) ) ) / ( 1 + exp( -L*(z-1) ) );
end
