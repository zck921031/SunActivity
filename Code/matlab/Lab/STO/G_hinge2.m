%
function [F] = G_hinge2(xTr, L, Sset, Dset, u1, u2)
    N = size(xTr,2);
    F = 0;
    tempFS=0; tempFD=0;
    x = L*xTr;
    for i = 1 : N
        for j = 1 : N
            dis = 0;
            if 0 ~= Sset(i,j)
                dis = sum( ( x(:,i) - x(:,j) ).^2 );
                F = F + u1*Sset(i,j)*dis;
                tempFS = tempFS + dis;
            end
            if 0 ~= Dset(i,j)
                dis = sum( ( x(:,i) - x(:,j) ).^2 );
                dis = max(1-dis, 0);
                F = F + u2*Dset(i,j)*dis;
                tempFD = tempFD + dis;
            end
        end
    end
end
