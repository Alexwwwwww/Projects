% code
NMSG = 2; % number of messages
Fcutoff = [5e3 10e3];

% load the signal x and the sample frequency Fs
for c=1:NMSG
    fname = ['speech' num2str(c) '.wav'];
    [x,Fs] = audioread(fname);
    if c == 1
        N = length(x);
        signal= zeros(NMSG,N); % signal
    end
    signal(c,:)= lowpass(x',Fs,Fcutoff(c));
end
rx_signal=zeros(NMSG,length(x)); % received signal

% get the sample time and the duration of the signal
Ts = 1/Fs; % sample time
t= Ts * [0:(N-1)];

% Transmitter
% % % % Revise the following code % % % %  
Fc = [45000 20000];     % carrier frequencies for FDM

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

tx_wave = zeros(1,length(t));
for c=1:NMSG
    tx_wave = tx_wave+signal(c,:).*cos(2*pi*Fc(c)*t);
end
% % % % Do not change code below this line % % % %  

%Channel
rx_wave = channel_f3(tx_wave);

% Receiver
for c=1:NMSG
    rx = demodulate(rx_wave,Fs,Fc(c),Fcutoff(c));
    rx_signal(c,:) = rx;
end

% Plot amplitude spectra of transmitted and received signals
figure(1);clf;
subplot(2,1,1);
plotAmplitudeSpectrum(tx_wave,Fs,'Amplitude Spectrum of Transmitted Signal');
subplot(2,1,2);
plotAmplitudeSpectrum(rx_wave,Fs,'Amplitude Spectrum of Received Signal');

% Plot amplitude spectra of recovered messages
figure(2);clf;
for c=1:NMSG
    subplot(NMSG,1,c);
    plot_title = ['Amplitude Spectrum of Recovered Message ' num2str(c)];
    plotAmplitudeSpectrum(rx_signal(c,:),Fs,plot_title);
end

% Plot transmitted and recovered messages for comparison

% The received signal is half as big, so we double it to compare
% with the originally transmitted signal
rxa = 2*rx_signal;

figure(3);clf;
for c = 1:NMSG
    subplot(NMSG,1,c);
    nmse = sum((signal(c,:)-rxa(c,:)).^2)/sum((signal(c,:)).^2);
    zoom_start =200000;
    zoom_stop =210001;
    ind = zoom_start:zoom_stop;
    plot(t(ind),signal(c,ind),'b'); hold on;
    plot(t(ind),rxa(c,ind),'--r'); hold off;
    legend('Original','Recovered','Location','NorthEastOutside');
    xlabel('Time(sec)')
    ylabel('Amplitude')
    title(['Message ' num2str(c) ' Waveform (zoom) : nmse = ' num2str(nmse)]);
    axis([t(zoom_start) t(zoom_stop) -0.4 0.4]);
    grid;
end