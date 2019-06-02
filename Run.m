function [predict, test, result] = Run(AD, MCI, CU, Feature, label)
    result = 10;
    for i=1:10
        [ p, t, rMSE(i)] = regression(AD, MCI, CU, 0.9, Feature, label, 'Ridge');
        if rMSE(i)<result
            result = rMSE(i);
            predict = p;
            test = t;
        end
    end
    mean(result)
    std(rMSE)
end