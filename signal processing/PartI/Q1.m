% Generate a random 10 bit sequence and a random SPB
% Do not change the values of bs and SPB in your code
[bs,SPB] = bit_seq_gen_final(5);  

n = 0:(length(bs)*SPB-1);      % sample index     
y = zeros(1,length(bs)*SPB);   % Vector to store the channel response 

% Modify the code below to predict the channel response 
for i=1:length(bs)
    if bs(i)==1
    y = y +step_response_final(n-(i-1)*SPB)-step_response_final(n-i*SPB);
    end
    %if bs(i)==0
    %y = y -step_response_final(n-(i-1)*SPB)+step_response_final(n-i*SPB);
    %end  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
end
% Do not modify the code below
figure(1);clf;
compare(y,bs,SPB);  % Compare the predicted and actual responses