%
function [sum] = F_smooth(x, y, pars)
    sum = 0;
    len = size(x, 2);
    for i=1:len
        for j=1:len
            if y(i)~=y(j)
                continue;
            end
            for k=1:len
                if y(i) == y(k)
                    continue;
                end
                sum = sum + l_smooth( norm( x(:, i)-x(:,k) )^2  - norm( x(:, i)-x(:,j) )^2, pars.L );        
            end
        end
    end

end

