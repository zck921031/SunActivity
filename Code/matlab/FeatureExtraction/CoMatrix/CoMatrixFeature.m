function ret = CoMatrixFeature(image)
    ret = [];
    if( size(image,3) > 1 )
        img = rgb2gray(image);
    else
        img = image;
    end
    dx = [ 0, -4, -4, -4];
    dy = [ 4,  4,  0, -4];
    for angle=1:4
        glcm=graycomatrix(img, 'Offset', [dx(angle) dy(angle)]);
        STAT=graycoprops(glcm,'all');
        ret = [ret, STAT.Contrast, STAT.Correlation, STAT.Energy, STAT.Homogeneity];
    end
end