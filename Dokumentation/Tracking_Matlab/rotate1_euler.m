function [ Matrix ] = rotate1_euler( alpha )
% rotate1_euler - rotate around Z axis
% nick-gier-roll-convention - z, y', x''

Matrix = [ [ cos(alpha)  sin(alpha)  0];
           [-sin(alpha)  cos(alpha)  0];
           [ 0           0           1] ];
end
