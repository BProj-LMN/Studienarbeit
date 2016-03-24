function [ translated ] = translateRay( ray, translation )
% translateRay - translate a point pair (ray) by a column vector
% (translation)

translated = [ray(:,1)+translation  ray(:,2)+translation ];
end
