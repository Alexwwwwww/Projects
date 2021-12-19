start_ind = 1501;%%where stsrt bit appears
a=0.98;
load('channel5_output.mat');

sample_ind = start_ind+[1:1280];%%%%%%%%%%%%%%%%%%%
lti_rx_wave=rx_wave;%%substract offset, get an LTI channel
%apply an equalizer to all bits 
eq_wave(1)=lti_rx_wave(1);
for n=2:length(lti_rx_wave)
    eq_wave(n) = (lti_rx_wave(n)-a*lti_rx_wave(n-1))./(1-a);   
end
threshold = (max(eq_wave)+min(eq_wave))*0.5;
rx_bs = eq_wave(sample_ind) > threshold;
MAC=[];
%%extracting MAC from original codeword, shd b 3*48 bits
for i = [1:8:48*2]
    MAC_bi=[];
    %iterate through all (8,4,3) codewords
    %calculate all syndrom bits
    s1=mod(rx_bs(i)+rx_bs(i+1)+rx_bs(i+4),2);
    s2=mod(rx_bs(i+2)+rx_bs(i+3)+rx_bs(i+5),2);
    s3=mod(rx_bs(i)+rx_bs(i+2)+rx_bs(i+6),2);
    s4=mod(rx_bs(i+1)+rx_bs(i+3)+rx_bs(i+7),2);
    %error detecting/correcting
    if s1+s2+s3+s4==2
        if s1==s3 && s1==1
            rx_bs(i)=1-rx_bs(i);
        elseif s1==s4 && s1==1
            rx_bs(i+1)=1-rx_bs(i+1);
        elseif s2==s3 && s2==1
            rx_bs(i+2)=1-rx_bs(i+2);
        elseif s2==s4 && s4==1
            rx_bs(i+3)=1-rx_bs(i+3);
        
        end
        
    end
    %%put 4 binary codeword into MAC_bi
    MAC_bi=rx_bs(i:i+3);
    
    %%transform from binary to decimal, one hex at a time
    mac_ind=MAC_bi(1)*2^3+MAC_bi(2)*2^2+MAC_bi(3)*2^1+MAC_bi(4);
    mac_ind=dec2hex(mac_ind);
    MAC=[MAC mac_ind];
end
%%print MAC adress
MAC
%%transform from ascii code to text
tx_msg=[];
data=[];
%%error correcting and decoding
  %iterate all (3,1,3) datacode and create 'data' as all valid binary bits
for i = [48*2+1:8:1280-2]%"-2"to prevent ind num overflow
    %iterate through all (8,4,3) codewords
    %calculate all syndrom bits
    s1=mod(rx_bs(i)+rx_bs(i+1)+rx_bs(i+4),2);
    s2=mod(rx_bs(i+2)+rx_bs(i+3)+rx_bs(i+5),2);
    s3=mod(rx_bs(i)+rx_bs(i+2)+rx_bs(i+6),2);
    s4=mod(rx_bs(i+1)+rx_bs(i+3)+rx_bs(i+7),2);
    %error detecting/correcting
    if s1+s2+s3+s4==2
        if s1==s3 && s1==1
            rx_bs(i)=1-rx_bs(i);
        elseif s1==s4 && s1==1
            rx_bs(i+1)=1-rx_bs(i+1);
        elseif s2==s3 && s2==1
            rx_bs(i+2)=1-rx_bs(i+2);
        elseif s2==s4 && s4==1
            rx_bs(i+3)=1-rx_bs(i+3);
        
        end
        
    end
    %%put 4 binary codeword into MAC_bi
    data_bi=rx_bs(i:i+3);
    data=[data data_bi];
end
%%tx_lt are letters, 8 bits binary each
for i = [1:8:length(data)-2]%"-2"to prevent ind num overflow
    %%extract all letters
    tx_lt=data([i:i+7]);
    %%trans binary to decimal
    ascii=0;
   
    for a=[7:-1:0]
        ascii=ascii+tx_lt(8-a)*2^a;
    end
    tx_msg=[tx_msg ascii];
    
end
%%transform a list of decimal ascii to letters
text=char(tx_msg)