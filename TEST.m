function [predict, test]=TEST(featS, featE, trainNum, order, Input, label)

[predict(:,order), test(:,order)] = regression(featE-featS+1, 120000, trainNum, Input(featS:featE, :),  label(:,order));
rMSE(i) = norm(predict(:,order) - test(:,order))/length(test);
rMSE(i) = sqrt(rMSE(i));
%[h, p(i)] = ttest(predict(:,order), test(:,order));

%p = sum(p)/20;
end