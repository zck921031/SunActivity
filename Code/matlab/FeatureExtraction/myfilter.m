function J = myfilter(I)
    if size(I, 3) > 1
        I = rgb2gray(I);
    end
    J=medfilt2(I,[10 10]);  %��ֵ�˲�
    h1=ones(5,5)/25;
    J=imfilter(J,h1);       %��ֵ�˲�
end