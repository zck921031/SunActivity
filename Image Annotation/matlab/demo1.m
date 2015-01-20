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
    img = uint8( zeros( sizeA, sizeB, D ) );
    for i=1:D
        tmp = rgb2gray(suns{i});
        img(:,:,i) = tmp(:,:);
    end
    
    figure(3);
    for i=1:D
        subplot(2,5,i);
        imshow( img(:,:,i) );
    end
    
    figure(4);
    show_hist_feature( img );
    
    figure(5);
    res = hist_feature_1(img);
    bar(res);
    
    figure(6);
    res = hist_feature_2(img, 8);
    bar(res);
    
    figure(7);
    res = hist_feature_3(img);
    bar(res);
    
end

%画出直方图看看~
function show_hist_feature(image)
    [A, B, D] = size(image);
    for i=1:D
        subplot(2,5,i);
        tmp = imhist( image(:,:,i) );
        bar(tmp);
    end
end

%颜色向量，统计每个波段图片像素值的和，可以看成统计每个波段的能量大小。即颜色一阶矩(mean)。
function ret = hist_feature_1(image)
    [A, B, D] = size(image);
    ret = zeros(1,D);
    for i=1:D
        ret(i) = sum( sum( image(:,:,i) ) );
    end
    %ret = ret./norm(ret,2);
    ret = ret./(A*B);
end

%D维-颜色直方图，量化级数为N
function ret = hist_feature_2(image, N)
    [A, B, D] = size(image);
    ret = zeros(1,N*D);
    for i=1:D
        tmp = imhist( image(:,:,i), N );
        for j=1:N
            ret( (i-1)*N + j ) = tmp(j);
        end
    end
    ret = ret./(A*B);
end

%颜色矩(mean, viarance, skewness)。
function ret = hist_feature_3(image)
    [A, B, D] = size(image);
    mu = zeros(1,D);
    sigma = zeros(1,D);
    s = zeros(1,D);
    for i=1:D
        mu(i) = sum( sum( image(:,:,i) ) )/(A*B);
    end
    gg = ( double( image(:,:,i) ) - mu(i) );
    for i=1:D
        sigma(i) = ( sum(sum( ( double( image(:,:,i) ) - mu(i) ).^2 ))/(A*B) )^(1/2);
    end
    for i=1:D
        s(i) = ( sum(sum( abs( double( image(:,:,i) ) - mu(i) ).^3 ) )/(A*B) )^(1/3);
    end
    ret = [ mu, sigma, s ];
end