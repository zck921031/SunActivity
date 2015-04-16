%
function [F] = G(xTr, L, Sset, Dset)
    N = size(xTr,2);
    F = 0;
    tempFS=0; tempFD=0;
    for i = 1 : N
        for j = 1 : N
            dis = sum( ( L*( xTr(:,i) - xTr(:,j) ) ).^2 );
            if 1 == Sset(i,j)
                F = F + dis;
                tempFS = tempFS + dis;
            end
            if 1 == Dset(i,j)
                F = F - dis;
                tempFD = tempFD + dis;
            end
        end
    end
end
