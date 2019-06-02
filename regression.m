function [ predict, testclass, rMSE] = regression(AD, MCI, CU, ratio, sample, label, Method)
   % cross-validation
    [sample, label, AD, MCI, CU] = removeZeros(AD, MCI, CU, size(sample,2), size(sample,1), sample, label);
    [trainfeatures, trainclass, testfeatures, testclass] = ramdomfeatures( AD, MCI, CU, size(sample,2), ratio, sample, label);
    %lasso-initail
    z = 0.005;
    load opts.mat;                                
    %lasso
    if strcmp(Method, 'Lasso')
        [x, funVal, ValueL]=LeastR(trainfeatures, trainclass, z, opts);
    else
        x=ridgeregression(trainclass, trainfeatures, z*max(trainfeatures'*trainclass), opts.maxIter);
    end
    
    predict = testfeatures*x;
    rMSE = sqrt(norm(testclass - predict)^2/size(testclass,1))
end


 