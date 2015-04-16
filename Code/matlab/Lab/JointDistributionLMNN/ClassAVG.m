s = zeros(2,1);
x = xTrain; y = yTrain;
%L = eye( size(xTrain,2) );
for i = 1 : size(xTrain,1)
    dis = 0;
    for j = 1 : size(xTrain,1)
        if ( yTrain(j) == 1 )
            dis = dis + norm( (xTrain(i,:)-xTrain(j,:))*L' );
        end
    end
    if ( y(i) == 1 ) 
        s(1) = s(1) + dis;
    else
        s(2) = s(2) + dis;
    end
end

%disp(s);


x = xTest; y = yTest;
x = xTrain; y = yTrain;
dist = zeros( size(x,1), 1);
err = zeros( size(x, 1), 1 );
for i = 1 : size(x, 1)
    for j = 1 : size(xTrain, 1)
        if ( yTrain(j) == 1 )
            dist(i) = dist(i) + norm( (x(i,:) - xTrain(j,:))*L' );
        end
    end
end    
d = sort(dist);
for k = 1 : size(d, 1)
    for i = 1 : size(x, 1)
        dis = dist(i);
        if ~( (dis<=d(k)&&y(i)==1) || (dis>d(k)&&y(i)~=1) )
            err(k) = err(k)+1;
        end
    end    
end
figure(7);
plot( err / size(x,1) ); hold on;

disp( min(err)/ size(x,1)  );