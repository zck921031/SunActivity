% test fisher linear discriminant
function [ ac ] = fisher_test( xTe, yTe, w, b )
    y = xTe*w + b < 0;
    ac = sum(y == yTe) / size(y,1);
    % disp( [ 'fisher��ȷ��: ',  num2str( ac ) ]  );
end