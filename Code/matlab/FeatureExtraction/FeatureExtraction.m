%Input: image
%Output: vector in feature space
function Vec = FeatureExtraction(Image) 
    addpath 'Color';
    addpath 'CoMatrix';
    addpath( genpath( pwd ) );
    
    %Hist = grayHistogram(Image);
    %CoMat = CoMatrixFeature(Image);
    %Flare None Coronal Hole
    %Image = imread( ['../../AnnotationSystem/AnnotationSystem/img/', 'Coronal Hole' , '/', '1', '/0211.jpg'] );
    
    %figure(2);
    %imshow(Image);
    %GB = getGBfeatures(Image);
%     k = 6;
%     Lbp = lbp( rgb2gray(Image), [-k -k; -k 0; -k k; 0 -k; -0 k; k -k; k 0; k k], 0, 'i' );
%     
%     %plot(Lbp);
%     M =  zeros( size( Lbp ) );
%     for k = 1 : 8
%         M(:,:) = M(:,:) + ( bitand( Lbp(:,:),(2^(k-1) ) ) >= 1 );
%     end
%     %M(:,:) = ( 3 <= M(:,:) );
%     figure(3);
%     imshow( M*16 );

    [~,~,GB] = gaborfilter( rgb2gray(Image), 6, 90, 0.5, 0.5);
    
    imshow( uint8(GB) );
    
    GB = imhist( uint8(GB) )';
    GB = GB / sum(GB);
    Vec = GB;
end