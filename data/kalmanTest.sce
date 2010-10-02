clc; clear all;
A = [1.1269   -0.4940    0.1129, 
     1.0000         0         0, 
          0    1.0000         0];
B = [-0.3832
      0.5919
      0.5191];
Q = eye(3,3);
R = 1;

C = [1 0 0];

P = 2* eye(3,3)

for i=1:1000
  P = A*P*A'+Q;
  S = C*P*C'+R;
  K = P*C'*inv(S);
  P = (eye(3,3)-K*C)*P;
end
P
K
 
