function [ Matrix ] = rotate3_euler( alpha )
% rotate3_euler - rotate around new new X axis
% nick-gier-roll-convention - z, y', x''

Matrix = [ [ 1           0           0];
           [ 0           cos(alpha)  sin(alpha)];
           [ 0          -sin(alpha)  cos(alpha)] ];
end
