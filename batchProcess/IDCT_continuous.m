function B = IDCT_continuous(k,t)
% k: the number of time sequence
% n: the number of frequency signal(base)

% B = sqrt(1/k);

i = 2:k;
B = [sqrt(1/k), sqrt(2/k)*cos((2*t+1)*(i-1)*pi/2/k)];

% C = sqrt(1/k);
% for i = 2 : k
%     C = [C sqrt(2/k)*cos((2*t+1)*(i-1)*pi/2/k)];
% end
% assert(all(B == C));

% D = dctmtx(k)
