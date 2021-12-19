data_stream=[0 0 0 0 0 1 0 1 0 1 0 0 0 1 1 1 0 1 1 1 0 1 1 1 ];%the original data stream
frame_num = length(data_stream)/3;
if mod(length(data_stream),3)~=0
    frame_num=frame_num+1;
    if mod(length(data_stream),3)==1
        data_stream=[datas_tream 0 0];
    else 
        data_stream=[data_stream 0];
    end
end

encoded_data_stream=[];
for i = [1:frame_num]
    
    codeword=data_stream([3*i-2:3*i]);
    codeword=[codeword mod(2-data_stream(3*i-2)+data_stream(3*i-1),2)];
    codeword=[codeword mod(2-data_stream(3*i-1)+data_stream(3*i),2)];
    codeword=[codeword mod(2-data_stream(3*i-2)+data_stream(3*i),2)];
    codeword
end
