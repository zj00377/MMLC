function [x] = ridgeregression(b, A, lambda, Iter)

L = 1/(norm(A)^2);
x = zeros(size(A,2),1);
for i=1:Iter
    x = x - L*(A'*(A*x-b)+lambda*x)/i;
end

end
