start_ind = 1568;%%where stsrt bit appears
a=0.9;
load('channel3_output.mat');

sample_ind = start_ind+[1:1280];%%%%%%%%%%%%%%%%%%%
lti_rx_wave=rx_wave-0.23;%%substract off-set, get an LTI channel
%apply an equalizer to all bits 
eq_wave(1)=lti_rx_wave(1);
for n=2:length(lti_rx_wave)
    eq_wave(n) = (lti_rx_wave(n)-a*lti_rx_wave(n-1))./(1-a);   
end
threshold = (max(eq_wave)+min(eq_wave))*0.5;
rx_bs = eq_wave(sample_ind) > threshold;
MAC=[];
%%extracting MAC from original codeword, shd b 3*48 bits
for i = [1:12:48*3]
    MAC_bi=[];
    %iterate through all (3,1,3) codewords
    for b=[0:3:11]
        codeword=rx_bs(i+b)+rx_bs(i+b+1)+rx_bs(i+b+2)>1.5;
        MAC_bi=[MAC_bi codeword];
    end
    
%%transform from binary to decimal
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
  %iterate all (3,1,3) datacode and create 'data' as all valid data bits
for i = [48*3+1:3:1280-2]
    data_bit=rx_bs(i)+rx_bs(i+1)+rx_bs(i+2)>1.5;
    data=[data data_bit];
end
%%tx_lt are letters, 8 bits binary each
for i = [1:8:length(data)-2]
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