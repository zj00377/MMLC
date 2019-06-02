clear all;
file = dir('/home/qingyang/Desktop/ADNI_HIPPO/*.csv');
t1=0; t2=0; t3=0;
tic;
for i=1:length(file)
   temp=csvread(['/home/qingyang/Desktop/ADNI_HIPPO/',file(i).name]);
   tic;
   if temp(30001, 1) == 2
       t1=t1+1;
      AD(:, :, t1)=temp(1:30000, 1:3);
   elseif temp(30001, 1) == 0
       t2=t2+1;
      MCI(:, :, t2)=temp(1:30000, 1:3);
   elseif temp(30001, 1) == 1
       t3=t3+1;
      CU(:, :, t3)=temp(1:30000, 1:3);
   end
end
toc;
