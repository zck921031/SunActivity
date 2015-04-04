%read one frame by file path.
%Input: Folder path containing fileinfo.txt.
%Ouput: An image matrix sized AxBxD, where D is the number of sensor's wavelengths.
function img = read_image_by_path( folder_path )
    filename = [folder_path '\fileinfo.txt'];
    filenames = textread(filename,'%s');
    D = size(filenames);
    for i=1:D
        filename = filenames{i};
        suns{i} = imread([folder_path '\' filename]);
    end
    [sizeA, sizeB, sizeC] = size( suns{1} );
    img = uint8( zeros( sizeA, sizeB, D ) );
    for i=1:D
        tmp = rgb2gray(suns{i});
        img(:,:,i) = tmp(:,:);
    end        
end