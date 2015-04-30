%demo.m
datasets = { 'corel5k' };
sets     = { 'test', 'train' };
impaths  = { 'C:\Users\zck\Desktop\metric learning\dataset\corel5k.20091111' };

T=10;

for db=1:length(datasets),
   ds     = [impaths{db} '\' datasets{db}];
   dict   = textread([ds '_dictionary.txt'],'%s');
   impath = impaths{db};

   for s=1:length(sets),
      str   = sets{s};

      list  = textread([ds '_' str '_list.txt'],'%s');
      annot = logical(vec_read([ds '_' str '_annot.hvecs']));

      u     = randperm(length(list));
      for i=1:T,
         n     = u(i);
         %/imshow([impath list{n} '.jpg']);
         words = dict(annot(n,:));
         %text(0,-10,[ sprintf('%d/%d: ',i,T) sprintf(' %s',(words{:}))]);
         %pause(2) 
      end
   end
end