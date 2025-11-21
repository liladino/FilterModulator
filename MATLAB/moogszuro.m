% --- Parameters ---
fs = 48000;          % Sample rate (Hz)
cutoff = 1000;       % Cutoff frequency (Hz)
C_res = 0.8;         % Resonance 

% Normalized cutoff frequency
wc = 2 * pi * cutoff / fs;
g = 0.9892 * wc - 0.4342 * wc^2 + 0.1381 * wc^3 - 0.0202 * wc^4;
R_factor = 1.0029 + 0.0526 * wc - 0.0926 * wc^2 + 0.0218 * wc^3;
G_res = C_res * R_factor; 

fprintf('Filter Parameters:\n');
fprintf('  Cutoff Frequency (cutoff): %.0f Hz\n', cutoff);
fprintf('  Quality Factor (C_res): %.2f\n', C_res);
fprintf('  Pole Gain (g): %.4f\n', g);
fprintf('  Feedback Factor (R): %.4f\n\n', G_res);

% --- Transfer Function ---

% Generate the Z-domain transfer function (tf) for a single MoogPole
H_pole = moog_pole(g, fs);
I = tf([1], [1], 1/fs); % Identity 

H_pole_2 = H_pole * H_pole;
H_pole_3 = H_pole_2 * H_pole;
H_pole_4 = H_pole_3 * H_pole;

H_LP = (H_pole_4 * (1 + 0.5 * G_res)) / (I + H_pole_4 * G_res);

% --- Plot ---

N = 4096; 
[H_lp, w] = freqz(H_LP.num{1}, H_LP.den{1}, N, fs);

figure;
semilogx(w, 20*log10(abs(H_lp)), 'LineWidth', 2.0, 'DisplayName', 'Low-Pass (P4)');
hold on;
grid on;
set(gca, 'XMinorGrid', 'off'); 
set(gca, 'GridLineStyle', '-');

xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
title(sprintf('Moog Filter Types: cutoff=%.0f Hz, C_res=%.1f', cutoff, C_res));
xlim([20 20000]);
ylim([-40 20]);
legend('Location', 'SouthWest');

% --- Test Frequencies ---
fprintf('Magnitude at test frequencies (LPF):\n');
test_freqs = [250, 500, 1000, 1500, 5000];
for f = test_freqs
    [~, idx] = min(abs(w - f));
    mag_db_lp = 20 * log10(abs(H_lp(idx)));
    
    fprintf('  %.0f Hz: LP=%.2f dB, HP=%.2f dB, BP=%.2f dB\n', f, mag_db_lp, mag_db_hp, mag_db_bp);
end

% ----------------------------------------------------------
function H = moog_pole(g, fs)
    % y[n] = g * (x[n] + 0.3 * x[n-1]) + (1 - g) * y[n-1]
    
    b = [g, 0.3 * g];
    a = [1, -(1 - g)];
    
    H = tf(b, a, 1/fs);
end
