%
function [F] = G_hinge(xTr, L, Sset, Dset)
    N = size(xTr,2);
    F = 0;
    tempFS=0; tempFD=0;
    x = L*xTr;
    for i = 1 : N
        for j = 1 : N
            dis = sum( ( x(:,i) - x(:,j) ).^2 );
            if 1 == Sset(i,j)
                F = F + dis;
                tempFS = tempFS + dis;
            end
            if 1 == Dset(i,j)
                dis = max(1-dis, 0);
                F = F + dis;
                tempFD = tempFD + dis;
            end
        end
    end
end
