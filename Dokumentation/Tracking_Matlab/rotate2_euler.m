function [ Matrix ] = rotate2_euler( alpha )
% rotate2_euler - rotate around new Y axis
% nick-gier-roll-convention - z, y', x''

Matrix = [ [ cos(alpha)  0          -sin(alpha)];
           [ 0           1           0];
           [ sin(alpha)  0           cos(alpha)] ];
end
