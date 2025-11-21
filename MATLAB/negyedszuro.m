% params
fs = 48000;          % sample rate
fc = 1000;           % cutoff

Q1 = 0.5411961;      % bw szuro standard ertekek
Q2 = 1.306563;

s = 2;               % resonance

% coeffs
w0 = 2*pi*fc/fs;
sinw0 = sin(w0);

alpha1 = sinw0 / (2 * Q1 * s); 
alpha2 = sinw0 / (2 * Q2 * s); 

H1 = hpfilt(fs, fc, alpha1);
H2 = hpfilt(fs, fc, alpha2);

H4 = H1 * H2;

[H, w] = freqz(H4.num{1}, H4.den{1}, 4096, fs);

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

function H = hpfilt(fs, fc, alpha)
    w0 = 2*pi*fc/fs;
    cosw0 = cos(w0);

    b0 =  (1 - cosw0)/2;
    b1 =  (1 - cosw0);
    b2 =  (1 - cosw0)/2;
    a0 =   1 + alpha;
    a1 =  -2*cosw0;
    a2 =   1 - alpha;

    b = [b0 b1 b2] / a0;
    a = [1 a1/a0 a2/a0];

    H = tf(b, a, 1/fs);
end