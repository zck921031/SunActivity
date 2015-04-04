%Calculate Color Moment Feature By image
%Input: An Image Matrix.
%Ouput: A 3xD-dimension vector. i.e. [ mu_i, sigma_i, s_i]_{i=1}^{D}
function ret = get_color_moment_feature_by_image(image)
    [A, B, D] = size(image);
    mu = zeros(1,D);
    sigma = zeros(1,D);
    s = zeros(1,D);
    for i=1:D
        mu(i) = sum( sum( image(:,:,i) ) )/(A*B);
    end
    for i=1:D
        sigma(i) = ( sum(sum( ( double( image(:,:,i) ) - mu(i) ).^2 ))/(A*B) )^(1/2);
    end
    for i=1:D
        s(i) = sum(sum( ( double( image(:,:,i) ) - mu(i) ).^3 ) )/(A*B);
        lable = 1;
        if ( s(i)<0 ) 
            lable = -1;
        end
        s(i) = abs( s(i) )^(1/3) * lable;
    end
    ret = [];
    for i=1:D
        ret = [ret mu(i), sigma(i), s(i)];
    end
    %ret = [ mu, sigma, s ];
end