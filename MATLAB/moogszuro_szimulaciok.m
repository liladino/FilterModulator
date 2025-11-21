fs = 48000;
duration = 1.0;
N = fs * duration;

%vect = [0, 0,  1,  0, 0]; % Two-pole low-pass
%vect = [0, 0,  0,  0, 1]; % Four-pole low-pass
%vect = [0, 2, -2,  0, 0]; % Two-pole band-pass
%vect = [0,  0, 4, -8, 4]; % Four-pole band-pass
%vect = [1, -2, 1,  0, 0]; % Two-pole high-pass
vect = [1, -4, 6, -4, 1]; % Four-pole high-pass

% Impulse signal
x = zeros(1, N);
x(1) = 1;

% Moog parameters
cutoff = 500; % Hz
wc = 2*pi*cutoff/fs;
g = 0.9892*wc - 0.4342*wc^2 + 0.1381*wc^3 - 0.0202*wc^4;
C_res = 1.3;
G_res = C_res * (1.0029 + 0.0526 * wc - 0.0926 * wc * wc + 0.0218 * wc * wc * wc);


% Run nonlinear simulation
[y, ~] = simulate_moog_nl(x, g, G_res, vect);

% FFT
N_fft = 8192;
Y = fft(y, N_fft);
f = linspace(0, fs/2, N_fft/2);
mag_db = 20*log10(abs(Y(1:N_fft/2)) + 1e-12);

% Plot
figure;
semilogx(f, mag_db, 'LineWidth', 2);
grid on;
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
title('Moog Ladder Filter Frequency Response (Impulse Input)');
xlim([20 20000]);
ylim([-60 10]);

% --- Moog nonlinear filter functions ---

function [y, state] = moog_pole_nl(x, state, g)
    y = g * (x + 0.3 * state.x1) + (1 - g) * state.y1;
    state.x1 = x;
    state.y1 = y;
end

function [output, pole_states] = simulate_moog_nl(input, g, G_res, egyutthatok)
    N = length(input);
    output = zeros(size(input));

    for i = 1:4
        pole_states(i).x1 = 0;
        pole_states(i).y1 = 0;
    end

    y_prev = 0;

    for n = 1:N
        xn = input(n);
        %s = xn - G_res * tanh(y_prev);

        %for i = 1:4
        %    [s, pole_states(i)] = moog_pole_nl(s, pole_states(i), g);
        %end

        a = xn - G_res * tanh(0.5 * xn + y_prev);
        
        [b, pole_states(1)] = moog_pole_nl(a, pole_states(1), g);
        [c, pole_states(2)] = moog_pole_nl(b, pole_states(2), g);
        [d, pole_states(3)] = moog_pole_nl(c, pole_states(3), g);
        [e, pole_states(4)] = moog_pole_nl(d, pole_states(4), g);
        
        output(n) = dot(egyutthatok, [a,b,c,d,e]);
        y_prev = e;
    end
end
