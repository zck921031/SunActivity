function ret = get_texture_feature_by_image(image)
    ret = [];
    [sizeA, sizeB, sizeD] = size(image);
    dx = [ 0, -1, -1, -1];
    dy = [ 1,  1,  0, -1]; 
    for idx=1:sizeD
        img = image(:,:,idx);
        for angle=1:4
            glcm=graycomatrix(img, 'Offset', [dx(angle) dy(angle)]);
            STAT=graycoprops(glcm,'all');
            ret = [ret, STAT.Contrast, STAT.Correlation, STAT.Energy, STAT.Homogeneity];
        end
    end
end