function [L,Det]=lmnn4(x,y,varargin)
% function [L,Det]=lmnn3(x,y,Kg,'Parameter1',Value1,'Parameter2',Value2,...);
%
% Input:
%
% x = input matrix (each column is an input vector) 
% y = labels 
% (*optional*)  L = initial transformation matrix (e.g eye(size(x,1)))
% (*optional*) Kg = attract Kg nearest similar labeled vectos
% 
% Important Parameters:
% diagonal = (default false) If set to true, a diagonal matrix is learned  
% stepsize = (default 1e-09)
% outdim = (default: size(x,1)) output dimensionality
% maxiter = maximum number of iterations (default: 1000)
% validation = (def 0) fraction of data used as validation 
%               (e.g. 0.2 means 20% of the training data is used as val)
% validationstep = (def 15) every "valcount" steps do validation
% quiet = {0,1} surpress output (default=0)  
% mu = must be >0 and <1 - tradeoff between loss and regularizer (default mu=0.5)
% subsample = (default 0.1) percentage of constraints that are subsampled (set to 1 for exact solution)
%
% Specific parameters (for experts only):
% correction = (def 15) how many steps between each update 
%              The number of impostors are fixed for until next "correction"
% factor = (def 1.1) multiplicative factor by which the
%         "correction" gab increases
% obj = (def 1) if 1, solver solves in L, if 0, solver solves in L'*L
% thresho = (def 1e-9) cut off for change in objective function (if
%           improvement is less, stop)
% thresha = (def 1e-22) cut off for stepsize, if stepsize is
%           smaller stop
% scale = (def. 0) if 1, all data gets re-scaled s.t. average
%         distance to closest neighbor is 1
%
%
% Output:
%
% L = linear transformation xnew=L*x
%    
% Det.obj = objective function over time
% Det.nimp = number of impostors over time
% Det.pars = all parameters used in run
% Det.time = time needed for computation
% Det.iter = number of iterations
% Det.verify = verify (results of validation - if used) 
%  
%
% 

global G0;

if(nargin==0)
 help lmnn;
 return;
end;


if(~isempty(varargin) & isnumeric(varargin{1}))
  % check if neighborhood or L have been passed on
  Kg=varargin{1};
%  fprintf('Setting neighborhood to k=%i\n',Kg);
  if(length(varargin)>1 & ~ischar(varargin{2}))
   L=varargin{2};
%   fprintf('Setting initial transformation!\n');
  end;
  
  % skip Kgand L parameters
  newvarargin={};copy=0;j=1;
  for i=1:length(varargin)
    if(ischar(varargin{i})) copy=1;end;
    if(copy)newvarargin{j}=varargin{i};j=j+1;end;
  end;
  varargin=newvarargin;  
  clear('newvarargin','copy');
else
% fprintf('Neigborhood size not specified. Setting k=3\n');
 Kg=3;
end;


if(exist('L','var')~=1)
	pars.outdim=size(x,1);
    %fprintf(['Initial starting point not specified.\nStarting with PCA.\n']);  
 	L=pca(x)';
else
	pars.outdim=size(L,1);
end;
tic

% checks
D=size(L,2);
x=x(1:D,:);
if(size(x,1)>length(L)) error('x and L must have matching dimensions!\n');end;



 % set parameters
 pars.modal=1;
 pars.Lpart = [];
 pars.diagonal=0;
 pars.stepsize=1e-09;
 pars.minstepsize=0;
 pars.tempid=-1;
 pars.maxiter=3000;
 pars.factor=1.1;
 pars.correction=15;
 pars.thresho=1e-7;
 pars.thresha=1e-17;
 pars.ifraction=1;
 pars.scale=1;
 pars.obj=1;
 pars.quiet=0;
 pars.classsplit=0;
 pars.validation=0;
 pars.validation_=0;
 pars.validationstep=25;
 pars.earlystopping=10;
 pars.valrand=1;
 
 
 pars.aggressive=0;
 pars.stepgrowth=1.01;
 pars.mu=0.5;
 pars.maximp=100000;
 pars.maximp0=1000000;
 pars.treesize=50;
 pars.valindex=[];
 pars.checkup=2; %0=notree 1=tree 2=choose
 pars.subsample=min(max(2000/length(y),0.1),1);

 pars.pars=[];
 
pars=extractpars(varargin,pars);


if isstruct(pars.pars), pars=pars.pars;end;
if(~pars.quiet),fprintf('LMNN stable version 2.6.0\n');end;
if pars.diagonal, pars.obj=2;L=eye(size(L)); end;
%disp( pars.obj );

L = fixL(L, pars);


% verification dataset
%i=randperm(size(x,2));
if(pars.validation<0 | pars.validation>1)
    error('validation parameter should be >0 and <1. Thanks.');
end;
earlycounter=0;
if isempty(pars.valindex),
    [itr,ite]=makesplits(y,1-pars.validation_,1,pars.classsplit,Kg+1,pars.valrand);
else
    pars.validation_=0.2;
    ite=find(ismember(1:length(y),pars.valindex));
    itr=find(~ismember(1:length(y),pars.valindex));
end;

if pars.validation>0,
 pars.validation_=pars.validation;
 pars.validation=0;
 [~,Det]=lmnn3(x,y,pars);
 pars.maxiter=Det.bestiter;
 pars.validation_=0;
 fprintf('Setting maxiter to %i ',pars.maxiter);
end;
xv=x(:,ite);
yv=y(:,ite);
x=x(:,itr);
y=y(itr);



%% Protection against k too small
un=unique(y);
for i=1:length(un)    
    ii=(y==un(i));
    if sum(ii)<=Kg, % remove all points from that label        
        y(ii)=[];
        x(:,ii)=[];
        ii=(yv==un(i));
        yv(ii)=[];
        xv(:,ii)=[];
        fprintf('Removing all instances of label %i, as there are fewer than %i\n',un(i),Kg);
    end;        
end;

if length(unique(y))<2,
    error('At least two classes must have more inputs than the neighborhood size.');
end;


verify=[];besterr=inf;
clear('xo','yo');
lowesterr=inf;
verify=zeros(1,pars.maxiter);
bestL=L;

if(~pars.quiet)
pars
end;


% Initializationip
[D,N]=size(x);
G0=sparse([],[],[],N,N);
if(~pars.quiet),fprintf('%i input vectors with %i dimensions\n',N,D);end;

[gen,NN]=getGenLS(x,y,Kg,pars);
obj=zeros(1,pars.maxiter);
nimp=zeros(1,pars.maxiter);

if(~pars.quiet) fprintf('Total number of genuine pairs: %i\n',size(gen,2));end;

%dfG=vec(SOD(x,gen(1,:),gen(2,:)));
dfG=vec(SODWkqwAx(x,sparse(gen(1,:),gen(2,:),1,N,N)));


if(pars.scale)
 Lx=L*x;
 sc=sqrt(mean(sum(((Lx-Lx(:,NN(end,:)))).^2,1)));
 L=2.*L./sc;
end;

df=zeros(D^2,1);
correction=1;
ifraction=pars.ifraction;
stepsize=pars.stepsize;


% flush gradient
df=zeros(size(dfG));
imp=zeros(2,0);

%% Main Loop
Det.log.L = {};
Det.log.usedtime = {};
for iter=1:pars.maxiter
 if rem(iter, 100) == 0
     Det.log.L{ uint8( iter/100 ) } = L;
     Det.log.usedtime{ uint8( iter/100 ) } = toc;
 end
 % save old position
 Lold=L;dfold=df;
 G0old=G0;
 % perform gradient step     
 if(iter>1)L=step(L,mat((dfG.*pars.mu+df.*(1-pars.mu))),stepsize,pars);end;
 %
 if(~pars.quiet)fprintf('%i.',iter);end; 
 % map data x->lx
 Lx=L*x;
 % compute distance to target neighbors
 Ni=zeros(Kg,N);
 for nnid=1:Kg
  Ni(nnid,:)=(sum((Lx-Lx(:,NN(nnid,:))).^2,1)+1);
 end;

 % check validation data set for early stopping
 if(pars.validation_>0 && (mod(iter,pars.validationstep)==0 | iter==1))
    verify(iter)=knncl([],Lx,y,L*xv,yv,Kg,'train',0);
    fprintf('kNN validation error: %2.2f ',verify(iter)*100);
    if(verify(iter)<=besterr) 
         fprintf('<= %2.2f   :-) %i/%i\n',besterr*100,earlycounter,pars.earlystopping);besterr=verify(iter);bestL=L;Det.bestiter=iter;
         earlycounter=0;
    else
         fprintf('> %2.2f   :-( %i/%i\n',besterr*100,earlycounter,pars.earlystopping);earlycounter=earlycounter+1;
    end;
    if(pars.earlystopping>0 & earlycounter>pars.earlystopping),fprintf('Validation error is no longer improving!\n');break;end;
 end;

 % update working set occasionally
 correction=correction-1;
 if correction==0 | mod(iter,500)==0,
  % every now and so often recompute the gradient from scratch (as inaccuracies accumulate)
  if iter>2 & (obj(iter-1)<0 |  mod(iter,500)==1),
      G0=G0.*0;
      df=zeros(size(dfG));
      %imp=zeros(2,0);
      fprintf('Flushing gradient!\n');
   end;

  %%%<<<
  os=pars.subsample;if isempty(nnz(G0)==0), pars.subsample=max(0.5,os);end;
  Vio=checkup(L,x,y,NN(Kg,:),pars);  
  pars.subsample=os;clear('os');


  Vio=setdiff(Vio',imp','rows')';
  if(pars.maximp<inf)
    i=randperm(size(Vio,2));
    Vio=Vio(:,i(1:min(pars.maximp*(iter>1)+pars.maximp0*(iter==1),size(Vio,2))));
  end;
  ol=size(imp,2);
  [imp i1 i2]=unique([imp Vio].','rows');
  imp=imp.';
  if(~pars.quiet)fprintf('Added %i constraints to active set (%i total).\n',size(imp,2)-ol,size(imp,2));end;  
  %%% <<<  
  correction=pars.correction;
 end;
 
 
 % compute gradient
 [impostors,df]=computeGradient(Kg,Lx,x,NN,Ni,df,imp);
 obj(iter)=(dfG.*pars.mu+df.*(1-pars.mu))'*vec(L'*L)+impostors.*(1-pars.mu);
 nimp(iter)=impostors;
 delta=obj(iter)-obj(max(iter-1,1));
 if(~pars.quiet)fprintf(['  Obj:%2.2f Nimp:%i Delta:%2.4f max(G): %2.4f             \n   '],obj(iter),nimp(iter),delta,max(max(abs(df))));end;

 % increase stepsize if it makes good progress, otherwise decrease
 if(iter>1 & delta>0 & correction~=pars.correction) 
  stepsize=stepsize*0.5;
  if(~pars.quiet)fprintf('***correcting stepsize***\n');end;
  if(stepsize<pars.minstepsize) stepsize=pars.minstepsize;end;
  if(~pars.aggressive)
   L=Lold;
   df=dfold;
   G0=G0old;
   obj(iter)=obj(iter-1);
  end;
 else 
  if(correction~=pars.correction)stepsize=stepsize*pars.stepgrowth;end;
 end;

 % check if converged
 if (iter>10) & (max(abs(diff(obj(iter-3:iter))))<pars.thresho*obj(iter)  | stepsize<pars.thresha)
    if iter<20, % special case: stuck because initial stepsize was too small
      stepsize=stepsize*10;
      continue;
    end;
    if(pars.correction-correction>=5) 
       correction=1;
       continue;
    end;
    switch(pars.obj)
       case 0
        if(~pars.quiet)fprintf('Stepsize too small. No more progress!\n');end;
        break;
       case 1
        if(~pars.quiet)fprintf('Stepsize too small. No more progress!\n');end;
        % break;
        pars.obj=0;
        pars.correction=15;
        stepsize=pars.stepsize;
        correction=1;
        G0=G0.*0;
        df=zeros(size(dfG));
        imp=zeros(2,0);
        if(~pars.quiet | 1) 
          if(~pars.quiet) fprintf('\nVerifying solution! %i\n',obj(iter)); end;
        end;
    end;
  end;
end;


if iter==pars.maxiter, if(~pars.quiet),fprintf('MAXIMUM Number of iterations reached. Terminating without convergence.\n');end;end;
% Output
Det.obj=obj(1:iter);
Det.nimp=nimp(1:iter);
Det.pars=pars;
Det.time=toc;
Det.iter=iter;

Det.verify=verify;

if(pars.validation_>0)
 Det.minL=L;
 L=bestL;  
 Det.verify=verify;
end;


function  [impostors,df]=computeGradient(Kg,Lx,x,NN,Ni,df,imp)    
    impostors=0;
    g0=cdist(Lx,imp(1,:),imp(2,:));
    g1=Ni(:,imp(1,:)); 
    g2=Ni(:,imp(2,:)); 

    [~,n]=size(Lx);
    global G0;    
    A0=sparse([],[],[],n,n,max(ceil(nnz(G0)*1.1),200000));    
    for nnid=Kg:-1:1 
        A1=sparse(imp(1,:),imp(2,:),g0<g1(nnid,:),n,n);
        A2=sparse(imp(1,:),imp(2,:),g0<g2(nnid,:),n,n); %activec constraints
        A0=A0-A1-A2+sparse(1:n,NN(nnid,:),sum(A2,1)+sum(A1,2)',n,n);
    end;    
    [~,~,pp]=find(A0);
    impostors=sum(pp(pp>0));
    %df=df+vec(SODWkqwAx(x,A0-G0));
    
    Q=(A0-G0)+(A0-G0)';
    ii=find(sum(Q~=0));
    Q=Q(ii,ii);
    X=x(:,ii);
    df=df+vec(-(X*(Q-spdiags(sum(Q)',0,length(Q),length(Q))))*X');    
    
    G0=A0;

    

function L=step(L,G,stepsize,pars);
% do step in gradient direction
% if(size(L,1)~=size(L,2)) pars.obj=1;end;

switch(pars.obj)
  case 0    % updating Q
     Q=L'*L;
     Q=Q-stepsize.*G;
    
     L = zeros( size(L) );
     for cc = 1 : length(pars.Lpart)
        Qt = Q( pars.Lpart(cc, 1):pars.Lpart(cc,2), pars.Lpart(cc, 1):pars.Lpart(cc,2) );         
        [Lt, dd] = eig(Qt);
        dd=real(diag(dd));
        Lt = real(Lt);    
        j=find(dd<1e-10);
        dd(j)=0;
        [~, ii] = sort(-dd);
        outdim = pars.Lpart(cc, 2) - pars.Lpart(cc, 1) + 1;
        ii = ii(1:outdim);
        Lt = Lt(:, ii );
        dd = dd(ii);
        Lt = Lt .* repmat( sqrt(dd)', size(Lt,1), 1);
        L( pars.Lpart(cc,1):pars.Lpart(cc,2), pars.Lpart(cc,3):pars.Lpart(cc,4) ) = Lt';
     end
     % decompose Q
%      [L,dd]=eig(Q);
%      dd=real(diag(dd));
%      L=real(L);
%      % reassemble Q (ignore negative eigenvalues)
%      j=find(dd<1e-10);
%      if(~isempty(j)) 
%          if(~pars.quiet)fprintf('[%i]',length(j));end;
%      end;
%      dd(j)=0;
%      [temp,ii]=sort(-dd);
%      L=L(:,ii);
%      dd=dd(ii);
%      L=(L*diag(sqrt(dd)))';
     % disp( rank(L) );
     % disp( norm(fixL(L, pars) - L) );
     % L = fixL(L, pars);
     
   case 1   % updating L
     G=2.*(L*G);
     L=L-stepsize.*G;
     
     L = fixL(L, pars);
     
     return;
  case 2 % diagonal L
     Q=L'*L;
	 Q=Q-stepsize.*G;
	 Q=diag(Q);
 	 L=diag(sqrt(max(Q,0)));
     return;
  otherwise
   error('Objective function has to be 0,1,2\n');
end;


function [mmL] = fixL(L, pars)
%  srow = pars.outdim / pars.modal;
%  scol = size(L, 2) / pars.modal;     
%  mmL  = zeros( size(L) );
%  for modal = 1 : pars.modal
%     for i = srow * ( modal - 1 ) + 1 : srow * modal
%         for j = scol * ( modal - 1 ) + 1 : scol * modal
%             mmL(i,j) = L(i,j);
%         end
%     end
%  end

 mmL  = zeros( size(L) );
 for cc = 1 : size(pars.Lpart,1)
     mmL( pars.Lpart(cc,1):pars.Lpart(cc,2), pars.Lpart(cc,3):pars.Lpart(cc,4) ) = L( pars.Lpart(cc,1):pars.Lpart(cc,2), pars.Lpart(cc,3):pars.Lpart(cc,4) );
%      for i = pars.outdim*(cc-1)+1 : pars.outdim*cc
%          for j = pars.Lpart(cc, 1) : pars.Lpart(cc, 2)
%              mmL(i,j) = L(i,j);
%          end
%      end
 end



function [gen,NN]=getGenLS(x,y,Kg,pars);
if(~pars.quiet);fprintf('Computing nearest neighbors ...\n');end; 
[D,N]=size(x);

un=unique(y);
Gnn=zeros(Kg,N);
for c=un
 if(~pars.quiet) fprintf('%i nearest genuine neighbors for class %i:',Kg,c);end;
 i=find(y==c);
 nn=LSKnn(x(:,i),x(:,i),2:Kg+1,pars);
 Gnn(:,i)=i(nn);
 if(~pars.quiet)fprintf('\r');end;
end;
if(~pars.quiet),fprintf('\n');end;
NN=Gnn;
gen1=vec(Gnn(1:Kg,:)')';
gen2=vec(repmat(1:N,Kg,1)')';
gen=[gen1;gen2];



function imp=checkup(L,x,y,NN,pars,~)
persistent treetime notreetime;

if pars.subsample<1,
    imp=checkupnotree(L,x,y,NN,pars);
    return;
end;    
if(nargin==6)
    treetime=-1;
    notreetime=-1;
end;
if(~pars.quiet)fprintf('Updating working set.\n');end;
t1=toc;
if(pars.checkup==1 | (pars.checkup==2 & treetime<notreetime))
  imp=checkupmtree(L,x,y,NN,pars);treetime=toc-t1;
else
  imp=checkupnotree(L,x,y,NN,pars);notreetime=toc-t1;
end;
% if there are too many constraints - subsample
if(size(imp,2)>pars.maximp0)
 ip=randperm(size(imp,2));
 ip=ip(1:pars.maximp0);
 imp=imp(:,ip);
 fprintf('Too many constraints - subsampling %i\n',pars.maximp0)
end;

 

function imp=checkupmtree(L,x,y,NN,pars)
if(~pars.quiet);fprintf('[Tree] Computing nearest neighbors ...\n');end;
[D,N]=size(x);

mL=max(L');
L=L(find(mL),:);
Lx=L*x;
Ni=sum((Lx-Lx(:,NN)).^2,1)+2;
un=unique(y);

% build up ball trees
for c=1:length(un)
 classindex{c}=find(y==un(c));
 forest{c}.tree=buildmtreemex(Lx(:,classindex{c}),pars.treesize);
end;
imp=[];
for c=1:length(un)-1
if(~pars.quiet)fprintf('All impostors for class %i    \r',c);end;
 for c2=c+1:length(un)
     try
      limps=findNimex(forest{c2}.tree,Lx(:,classindex{c2}),Lx(:,classindex{c}),Ni(classindex{c2}),Ni(classindex{c}));         
     catch
         fprintf('The bizarre error happened!\n');
         fprintf('Check class index, c2 etc\n');
         fprintf('Line "findNimex(forest..." in lmnn2\n');
         keyboard;
     end;
      
%    keyboard;
 	if(size(limps,2)>pars.maximp)
  	 ip=randperm(size(limps,2));
  	 ip=ip(1:pars.maximp);
  	 limps=limps(:,ip);
  	end;
	limps=[classindex{c}(limps(1,:));classindex{c2}(limps(2,:))];
    imp=[imp limps];
   end;
end;
try
 imp=unique(sort(imp)','rows')';
catch
 fprintf('Sorry, probably ran out of memory!');
 keyboard;  
end;



function imp=checkupnotree(L,x,y,NN,pars)
if(~pars.quiet) fprintf('Computing nearest neighbors ...\n');end;
Lx=L*x;
Ni=sum((Lx-Lx(:,NN)).^2,1)+2;
un=unique(y);
imp=[];

%parfor c=un(1:end-1)
for c=un(1:end-1)
 if(~pars.quiet)fprintf('All nearest impostor neighbors for class %i :',c);end;
 i=find(y==c);
 index=find(y>c);
 %keyboard;
 %% experimental
 ir=randperm(length(i));ir=ir(1:ceil(length(ir)*pars.subsample));
 ir2=randperm(length(index));ir2=ir2(1:ceil(length(ir2)*pars.subsample));
 index=index(ir2);
 i=i(ir);
 %% experimental

 limps=LSImps2(Lx(:,index),Lx(:,i),Ni(index),Ni(i),pars);
 if(size(limps,2)>pars.maximp)
  ip=randperm(size(limps,2));
  ip=ip(1:pars.maximp);
  limps=limps(:,ip);
 end;
 imp=[imp [i(limps(2,:));index(limps(1,:))]];

 if(~pars.quiet)fprintf('\r');end;
end;

try
 imp=unique(sort(imp)','rows')';
catch
 fprintf('Sorry, probably ran out of memory!');
 keyboard;  
end;



function limps=LSImps2(X1,X2,Thresh1,Thresh2,pars);
B=2000;
N2=size(X2,2);
limps=[];
for i=1:B:N2
  BB=min(B,N2-i);
  newlimps=findimps3Dm(X1,X2(:,i:i+BB), Thresh1,Thresh2(i:i+BB));
  if(~isempty(newlimps) & newlimps(end)==0)    
    [~,endpoint]=min(min(newlimps));
    newlimps=newlimps(:,1:endpoint-1);
  end;
  newlimps=unique(newlimps','rows')';
  newlimps(2,:)=newlimps(2,:)+i-1;
  limps=[limps newlimps];
  if(~pars.quiet)fprintf('(%i%%) ',round((i+BB)/N2*100)); end;
end;
if(~pars.quiet)fprintf(' [%i]              ',size(limps,2));end;




function NN=LSKnn(X1,X2,ks,pars);
B=750;
[D,N]=size(X2);
NN=zeros(length(ks),N);
DD=zeros(length(ks),N);

for i=1:B:N
  BB=min(B,N-i);
  if(~pars.quiet)  fprintf('.');end;
  Dist=distance(X1,X2(:,i:i+BB));
  if(~pars.quiet)  fprintf('.');end;
  [~,nn]=mink(Dist,max(ks));
  clear('Dist');
  if(~pars.quiet)  fprintf('.');end;
  NN(:,i:i+BB)=nn(ks,:);
  clear('nn');
  if(~pars.quiet),fprintf('(%i%%) ',round((i+BB)/N*100)); end;
end;
  



