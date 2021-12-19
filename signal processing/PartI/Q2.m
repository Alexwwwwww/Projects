% use the last two characters of your last name
tx_msg = 'NG'; % e.g. WANG
bs = text2bitseq(tx_msg);   

% Modify the code below so that framed_bs contains the bit stream after
% framing following the communication protocol described in question 2
framed_bs=[];
for i=1:5:(length(bs)-5)
    
    framed_bs =[framed_bs 1 bs([i:i+4]) 0];

end
framed_bs=[framed_bs 1 bs([i+5:length(bs)]) zeros(1,4) 0];
% do not modify code below
figure(1);clf;
subplot(211);
stem(bs);
axis([0 20 -0.1 1.1]);title('bs');
set(gca,'XTick',[0:4:20]);
set(gca,'YTick',[0:1]);
grid;
subplot(212);
stem(framed_bs);   
axis([0 30 -0.1 1.1]);title('framed\_bs');
set(gca,'XTick',[0:5:30]);
set(gca,'YTick',[0:1]);
grid;
xlabel('Bit Index');
      
