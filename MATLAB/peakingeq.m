% params
fs = 48000;          % sample rate
fc = 1000;           % cutoff

Q = 10;
dbGain = 6; 

% coeffs

w0 = 2*pi*fc/fs;
sinw0 = sin(w0);

A = 10 ^ (dbGain/40);  
% S = ( Q^2 * (A + 1/A) ) / ( 1 + Q^2 * (A + 1/A - 2) ) ; % slope

alpha = sinw0 / (2 * Q);

Hpeak = hpfilt(fs, fc, A, alpha);

[H, w] = freqz(Hpeak.num{1}, Hpeak.den{1}, 4096, fs);

figure;
semilogx(w, 20*log10(abs(H)), 'LineWidth', 1.5);
grid on;
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
title('High-pass filter response');
xlim([20 fs/2]);
ylim([-50 20]);

test_freqs = [250, 500, 1000, 1500, 5000];

for f = test_freqs
    [~, idx] = min(abs(w - f));
    mag_db = 20 * log10(abs(H(idx)));
    fprintf('%.0f Hz -> %.2f dB\n', f, mag_db);
end

% ----------------------------------------------------------

function H = hpfilt(fs, fc, A, alpha)
    w0 = 2*pi*fc/fs;
    cosw0 = cos(w0);

    b0 =  1 + alpha * A;
    b1 = -2 * cosw0;
    b2 =  1 - alpha * A;
    a0 =  1 + alpha / A;
    a1 = -2*cosw0;
    a2 =  1 - alpha / A;

    b = [b0 b1 b2] / a0;
    a = [1 a1/a0 a2/a0];

    H = tf(b, a, 1/fs);
end