function ClassAVGbcl(xTrain, yTrain, xTest, yTest, L, color)
    dist = zeros( size(xTest,1), 1);
    distN = zeros( size(xTest,1), 1);
    err = zeros( size(xTest, 1), 1 );
    for i = 1 : size(xTest, 1)
        for j = 1 : size(xTrain, 1)
            if ( yTrain(j) == 1 )
                dist(i) = dist(i) + sum( ( (xTest(i,:) - xTrain(j,:))*L' ).^2 );
            else
                distN(i) = distN(i) + sum( ( (xTest(i,:) - xTrain(j,:))*L' ).^2 );
            end
        end
    end
    d = sort(dist);
    for k = 1 : size(d, 1)
        for i = 1 : size(xTest, 1)
            dis = dist(i);
            if ~( (dis<=d(k)&&yTest(i)==1) || (dis>d(k)&&yTest(i)~=1) )
                err(k) = err(k)+1;
            end
        end    
    end
    plot( err / size(xTest, 1), color ); hold on;
    
    Right = 0;
    Psize = sum( yTrain == 1 );
    Nsize = sum( yTrain ~= 1 );
    for i = 1 : size(xTest, 1)
        if ( dist(i)/Psize < distN(i)/Nsize )
            c = 1;
        else
            c = 0;
        end
        if ( c==yTest(i) )
            Right = Right+1;
        end
        %[dist(i)/Psize,  distN(i)/Nsize]
    end
    disp( [ '类平均法正确率: ', num2str( Right/ size(xTest, 1) ) ] );

end