% clc;

clear;

addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\func';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\oasis';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn\helperfunctions';
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\lmnn';

% {'Sunspot', 'Coronal Hole', 'Flare'}
Concept = 'Coronal Hole';
[ xTr, yTr, xTe, yTe ] = load_feature( Concept, 1:9, 1:3);
% cd 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\NUS';

xTr = xTr'; yTr = yTr';
xTe = xTe'; yTe = yTe';

[D, N] = size(xTr);

tic
addpath 'C:\Users\zck\Documents\GitHub\SunActivity\Code\matlab\Lab\NUS';
parms.use_matlab = 1;
parms.do_save = 0;
L = eye(D,D);
for cc = 1 : 10
    parms.num_steps = 1e3;
    obj = oasis(xTr', yTr', parms, L'*L );
    
    L = LU(obj.W);

    errRAW = knncl( [], L*xTr, yTr, L*xTe, yTe, 1);
    disp( ['pair_sto_con knn train error rate: ' num2str(errRAW(1) ) ] );
    disp( ['pair_sto_con knn test  error rate: ' num2str(errRAW(2) ) ] );
end
usedtime = toc;

disp( ['Elapsed time is ' num2str(usedtime) ' seconds' ] );

save OASIS_CH L usedtime;