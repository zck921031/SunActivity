function onRecognition(hObject, eventdata, handles)
    load filenames;
    day = str2num( get(handles.day,'String') );
    wave = get(handles.wave,'Value');
    hImage = imshow( imread(['C:\Users\zck\Documents\GitHub\SunActivity\data\regional annotation\img\', Names{day, wave }] ) );
    
    set(hImage,'buttondownFcn',{@myfunc,handles}); %关键是这句，要自定义点击图像的事件的回调函数
    
    %% load Names, data, label, fb
    wavecnt = 1;
    load filenames;
    load '../FeatureExtraction/train';    
    global fb;
    load feedback;
    
    for i = 1 : size(fb,1)
        idx = find( fb{i}.care == 1 );
        data = [data; fb{i}.data(idx, :) ];
        label = [label; fb{i}.concept(idx, :)];
    end
    
    
    %% train fisher
    addpath( '../FeatureExtraction' );
    addpath( '../fisher' );
    data = data(:, 1:32 );
    label = (label == 1);
    
    %% LMNN
    cd '../lmnn';
    setpaths;
    knn=3;  % we are optimizing ver the knn=3 nearest neighbors classifier
    disp(['Automatic tuning of LMNN parameters for ' num2str(knn) '-NN classification.']); 

    xTr = data'; yTr = label';
    L0=pca(xTr)';
    [L,~] = lmnn2(xTr, yTr,3,L0,'maxiter',1000,'quiet',1,'outdim',3,'mu',1.0,'validation',0.2,'earlystopping',25,'subsample',1.0);


    %% fisher
    [w,b] = fisherbcl(data*L', label);
    
    %% run test
    day = str2num( get(handles.day,'String') );
    global images;
    images = {};
    for wave = 1 : 9
        images{wave} = imread(['C:\Users\zck\Documents\GitHub\SunActivity\data\regional annotation\img\', Names{day, wave }] );
    end
    
    global ft;
    ft = [];
    global rect;
    rect = [];
    for i = 256 : 512 : 3328
    for j = 256 : 512 : 3328
        if (i-2048+256)^2+(j-2048+256)^2 < 1700^2
            x = [];
            for k = 1 : wavecnt
                image = images{k}(j:j+512, i:i+512,:);
                %figure(1), imshow(image);
                %image = imresize(image, 0.25);                
                %figure(2), imshow(image);
                res = FeatureExtraction( image );
                %res = res(1:32);
                x = [x, res];
            end
            %cl = x*w + b;
            cl = x*L'*w + b;
            %[i, j, cl]
            if ( cl < 0  )
                rectangle('Position',[i, j, 512, 512], 'edgecolor','y');
                %figure(2), imshow(image);
            end
            ft = [ft; x];
            rect = [rect; [i,j, i+512, j+512, cl<0, 0] ];
        end
    end
    end
    if ( (size(fb,1)>=day) )
      if ( size(fb{day}.rect,1)>10 )
        rect(:,1:4) = fb{day}.rect(:,:);
        rect(:,5) = fb{day}.concept(:,:);
        rect(:,6) = fb{day}.care(:);
        for i = 1 : size(rect,1)
          if ( 1 == rect(i,6) )
            if -1 == rect(i,5)
                rectangle('Position',[rect(i,1)+32, rect(i,2)+32, 512-64, 512-64], 'edgecolor','w');
            end
            if 0 == rect(i,5)
                rectangle('Position',[rect(i,1)+32, rect(i,2)+32, 512-64, 512-64], 'edgecolor','black');
            end
            if 1 == rect(i,5)
                rectangle('Position',[rect(i,1)+32, rect(i,2)+32, 512-64, 512-64], 'edgecolor','r');
            end
          end
        end
      end
    end
    cd '../Annotation';

function myfunc(hObject, eventdata, handles)
    point=get(gca,'currentpoint');
    ax = point(1,1); ay= point(1,2);
    [ax, ay]
    global rect;
    global ft;
    for i = 1 : size(rect,1)
        if ( rect(i,1)<=ax && ax<=rect(i,3) && rect(i,2)<=ay && ay<=rect(i,4) )
            rect(i,6) = 1;
            rect(i,5) = mod(rect(i,5)+2, 3) - 1;
            if -1 == rect(i,5)
                rectangle('Position',[rect(i,1)+32, rect(i,2)+32, 512-64, 512-64], 'edgecolor','w');
            end
            if 0 == rect(i,5)
                rectangle('Position',[rect(i,1)+32, rect(i,2)+32, 512-64, 512-64], 'edgecolor','black');
            end
            if 1 == rect(i,5)
                rectangle('Position',[rect(i,1)+32, rect(i,2)+32, 512-64, 512-64], 'edgecolor','r');
            end
            %rect(i,:)
        end
        %rectangle('Position',[rect(i,1), rect(i,2), 512, 512], 'edgecolor','r');
    end
    