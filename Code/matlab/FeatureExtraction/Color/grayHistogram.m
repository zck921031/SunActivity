%
function Hist = grayHistogram(IMAGE)
    image = IMAGE;
    K = 16;
    if size(image, 3) > 1
        image = rgb2gray(image);
    end
    Hist = imhist(image, K);
    Hist = Hist';
    Hist = Hist / sum(Hist);
    %imshow(image);
end