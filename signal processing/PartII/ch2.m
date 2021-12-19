start_ind = 1641;
a=0.8;
load('channel2_output.mat');

sample_ind = start_ind+[1:1280];
lti_rx_wave=rx_wave-0.23;

for n=sample_ind
    eq_wave(n) = (lti_rx_wave(n)-a*lti_rx_wave(n-1))./(1-a);   
end
threshold = (max(eq_wave)+min(eq_wave))*0.5;
rx_bs = eq_wave(sample_ind) > threshold;
%%extracting MAC from original codeword

MAC=[];
for i = [1:4:48]
    
    codeword=rx_bs([i:i+3]);
%%transform from binary to decimal
    mac_ind=codeword(1)*2^3+codeword(2)*2^2+codeword(3)*2^1+codeword(4);
    mac_ind=dec2hex(mac_ind);
    MAC=[MAC mac_ind];
end
%%print MAC adress
MAC
%%transform from ascii code to text
tx_msg=[];
for i = [49:8:1280]
    tx_lt=rx_bs([i:i+7]);
    %%trans binary to decimal
    ascii=0;
    for a=[7:-1:0]
        ascii=ascii+tx_lt(8-a)*2^a;
        
    end
    tx_msg=[tx_msg ascii];
    
end
text=char(tx_msg)