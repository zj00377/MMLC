function [trainfeatures, trainclass, testfeatures, testclass]=ramdomfeatures( AD, MCI, CU, samDim, ratio, sample, label)

    sample_AD = zeros(AD, samDim+1);
    sample_MCI = zeros(MCI, samDim+1);
    sample_CU = zeros(CU, samDim+1);
    
    for i = 1:AD
        sample_AD(i, 1:samDim) = sample(i,:);
        sample_AD(i, samDim+1) = label(i);
    end
    for i = 1:MCI
        sample_MCI(i, 1:samDim) = sample(i+AD,:);
        sample_MCI(i, samDim+1) = label(i+AD);
    end
    for i = 1:CU
        sample_CU(i, 1:samDim) = sample(i+AD+MCI,:);
        sample_CU(i, samDim+1) = label(i+AD+MCI);
    end
 
    sample_AD = sample_AD(randperm(AD),:);
    sample_MCI = sample_MCI(randperm(MCI),:);
    sample_CU = sample_CU(randperm(CU),:);
   
    for i = 1:int16(AD*ratio)
        trainfeatures(i, 1:samDim) = sample_AD(i, 1:samDim);
        trainclass(i) = sample_AD(i, samDim+1);
    end
    for i = 1+int16(AD*ratio):AD
        testfeatures(i-int16(AD*ratio), 1:samDim) = sample_AD(i, 1:samDim);
        testclass(i-int16(AD*ratio)) = sample_AD(i, samDim+1);
    end

    for i = 1:int16(MCI*ratio)
        trainfeatures(i+int16(AD*ratio), 1:samDim) = sample_MCI(i, 1:samDim);
        trainclass(i+int16(AD*ratio)) = sample_MCI(i, samDim+1);
    end
    for i = 1+int16(MCI*ratio):MCI
        testfeatures(i-int16(MCI*ratio)+AD-int16(AD*ratio), 1:samDim) = sample_MCI(i, 1:samDim);
        testclass(i-int16(MCI*ratio)+AD-int16(AD*ratio)) = sample_MCI(i, samDim+1);
    end
    
    for i = 1:int16(CU*ratio)
        trainfeatures(i+int16(AD*ratio)+int16(MCI*ratio), 1:samDim) = sample_CU(i, 1:samDim);
        trainclass(i+int16(AD*ratio)+int16(MCI*ratio)) = sample_CU(i, samDim+1);
    end
    for i = 1+int16(CU*ratio):CU
        testfeatures(i-int16(CU*ratio)+AD-int16(AD*ratio)+MCI-int16(MCI*ratio), 1:samDim) = sample_CU(i, 1:samDim);
        testclass(i-int16(CU*ratio)+AD-int16(AD*ratio)+MCI-int16(MCI*ratio)) = sample_CU(i, samDim+1);
    end

    trainclass = trainclass';
    testclass = testclass';
end
