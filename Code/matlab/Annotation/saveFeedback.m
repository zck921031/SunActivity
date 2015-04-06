%
function saveFeedback(hObject, eventdata, handles)
    day = str2num( get(handles.day, 'String') );
    global rect;
    global fb;
    global images;
    x = [];
    fb{day}.rect = rect(:,1:4);
    fb{day}.concept = rect(:,5);
    fb{day}.care = rect(:,6);
    fb{day}.data = [];
    for i = 1 : size(rect)
        if 1 == rect(i,6) && 0 ~= rect(i,5)
            x = [];
            for k = 1 : 9
               image = images{k}( rect(i,2):rect(i,4), rect(i,1):rect(i,3), :);
               %figure(3), imshow(image);
               res = FeatureExtraction( image );
               x = [x, res];
            end
            fb{day}.data(i,:) = x;
        end
    end
    save feedback fb;
end