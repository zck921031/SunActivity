%Demo1
function demo1()
    close;
    clc;
    clear;
    path = '../SampleData/';
    filenames = textread([path 'fileinfo.txt'],'%s');

    D = size(filenames);
    figure(1);
    for i=1:D
        filename = filenames{i};
        suns{i} = imread([path filename]);
        subplot(2,5,i);
        imshow( suns{i} );
    end
    
    %figure(2);
    %imshow( rgb2gray(suns{9}) );
    
    [sizeA, sizeB, sizeC] = size( suns{1} );
    img = zeros( sizeA, sizeB, D );
    for i=1:D
        tmp = rgb2gray(suns{i});
        img(:,:,i) = tmp(:,:);
    end
    
    figure(3);
    for i=1:D
        subplot(2,5,i);
        imshow( uint8( img(:,:,i) ) );
    end
    
    figure(4);
    for i=1:D        
        subplot(2,5,i);
        res = hist_feature( uint8(img(:,:,i)) );
        bar(res);        
    end
    
end


function [ret]=hist_feature(image)
    ret = imhist(image);
end