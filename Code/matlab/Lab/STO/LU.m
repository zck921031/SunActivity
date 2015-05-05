%
function [L] = LU(Q)
     [L,dd]=eig(Q);
     dd=real(diag(dd));
     L=real(L);
     % reassemble Q (ignore negative eigenvalues)
     j=find(dd<1e-12);
     dd(j)=0;
     [~, ii]=sort(-dd);
     L=L(:,ii);
     dd=dd(ii);
     L=(L*diag(sqrt(dd)))';   
end